// -*- C++ -*-
//
// modified from V0Fitter. It is a cmssw code used in RECO and AOD. To find Lambda0 and Kshort
// Package:    V0Producer
// Class:      V0Fitter
//
/**\class V0Fitter V0Fitter.cc RecoVertex/V0Producer/src/V0Fitter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Brian Drell
//         Created:  Fri May 18 22:57:40 CEST 2007
// $Id: V0Fitter.cc,v 1.55 2011/11/12 01:39:27 drell Exp $
//
//

#include "vertexProducer/vertexProducer/interface/fourTracksFitter.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>
#include <typeinfo>
#include <memory>

#include "vertexProducer/vertexProducer/plugins/fourTracksFromVCCProducer.h"


// Constructor and (empty) destructor
fourTracksFitter::fourTracksFitter(const edm::ParameterSet & theParameters,
                       edm::ConsumesCollector&& iC ) :
    tktkCands(nullptr), tmpContainerToTkTkCands(nullptr), nCandsSize(0), tmpContainerSize(100)
{
    using std::string;
    beamspotToken = iC.consumes<reco::BeamSpot>( theParameters.getParameter<edm::InputTag>("beamspotLabel") );
    tktkPairToken = iC.consumes<reco::VertexCompositeCandidateCollection>( theParameters.getParameter<edm::InputTag>("tktkCandLabel") );
    mumuPairToken = iC.consumes<reco::VertexCompositeCandidateCollection>( theParameters.getParameter<edm::InputTag>("mumuCandLabel") );


    // ------> Initialize parameters from PSet. ALL TRACKED, so no defaults.
    // First set bits to do various things:
    //  -decide whether to use the KVF track smoother, and whether to store those
    //     tracks in the reco::Vertex

    // initialize informations for each candidates.
    optD = new double[totNumD];
    optB = new bool[totNumB];
    optS = new std::string[totNumS];

    optS[candName]   = theParameters.getParameter < string > (string("candName"));
    optS[muPosName]  = theParameters.getParameter < string > (string("muPosName"));
    optS[muNegName]  = theParameters.getParameter < string > (string("muNegName"));
    optS[tkPosName]  = theParameters.getParameter < string > (string("tkPosName"));
    optS[tkNegName]  = theParameters.getParameter < string > (string("tkNegName"));

    optD[muPosMass]  = theParameters.getParameter < double > (string("muPosMass"));
    optD[muNegMass]  = theParameters.getParameter < double > (string("muNegMass"));
    optD[tkPosMass]  = theParameters.getParameter < double > (string("tkPosMass"));
    optD[tkNegMass]  = theParameters.getParameter < double > (string("tkNegMass"));

    optD[muPosSigma] = theParameters.getParameter < double > (string("muPosSigma"));
    optD[muNegSigma] = theParameters.getParameter < double > (string("muNegSigma"));
    optD[tkPosSigma] = theParameters.getParameter < double > (string("tkPosSigma"));
    optD[tkNegSigma] = theParameters.getParameter < double > (string("tkNegSigma"));

    // selection after vertexing
    optD[FDSigCut_mumutktkToBS]  = theParameters.getParameter<double>("FDSigCut_mumutktkToBS");
    optD[vtxprobCut]             = theParameters.getParameter<double>("vtxprobCut");
    optD[mCandMassCut]           = theParameters.getParameter<double>("mCandMassCut");
    optD[MCandMassCut]           = theParameters.getParameter<double>("MCandMassCut");
    optD[mumuMassConstraint]     = theParameters.getParameter<double>("mumuMassConstraint");

    // selection before vertexing
    optD[FDSigPreCut_tktkTomumu] = theParameters.getParameter<double>("FDSigPreCut_tktkTomumu");
    optD[Cosa2dPreCut_tktkTomumu]= theParameters.getParameter<double>("Cosa2dPreCut_tktkTomumu");
    optD[ptPreCut_muPos]         = theParameters.getParameter<double>("PtPreCut_muPos");
    optD[ptPreCut_muNeg]         = theParameters.getParameter<double>("PtPreCut_muNeg");
    optD[ptPreCut_tkPos]         = theParameters.getParameter<double>("PtPreCut_tkPos");
    optD[ptPreCut_tkNeg]         = theParameters.getParameter<double>("PtPreCut_tkNeg");
    optD[mMassPreCut_mumu]       = theParameters.getParameter<double>("mMassPreCut_mumu");
    optD[MMassPreCut_mumu]       = theParameters.getParameter<double>("MMassPreCut_mumu");
    optD[mMassPreCut_tktk]       = theParameters.getParameter<double>("mMassPreCut_tktk");
    optD[MMassPreCut_tktk]       = theParameters.getParameter<double>("MMassPreCut_tktk");
    
    
    return;
}

fourTracksFitter::~fourTracksFitter()
{
    printf("~fourTracksFitter start\n");
    delete[] optD;
    delete[] optB;
    delete[] optS;


    delete[] tmpContainerToTkTkCands;
    delete tktkCands;
    printf("~fourTracksFitter end\n");
    return;
}


// Get methods
const reco::VertexCompositeCandidateCollection& fourTracksFitter::getFourTkCands() const
{
    if ( !tktkCands )
    {
        printf("fourTracksFitter::getCands() : no tktkCands returned, please check\n");
        exit(1);
    }
    return *(tktkCands);
}

void fourTracksFitter::enlargeContainer()
{
    // new container size;
    tmpContainerSize *= 2;
    reco::VertexCompositeCandidate* tmp =  new reco::VertexCompositeCandidate[tmpContainerSize];

        for ( unsigned j=0; j<nCandsSize; ++j )
            tmp[j] = tmpContainerToTkTkCands[j];
        // delete original container.
        delete[] tmpContainerToTkTkCands;
        tmpContainerToTkTkCands = tmp;
    return;
}
void fourTracksFitter::fillInContainer()
{
        reco::VertexCompositeCandidateCollection* tmpCollection
            = new reco::VertexCompositeCandidateCollection( tmpContainerToTkTkCands, tmpContainerToTkTkCands + nCandsSize);
        tktkCands = tmpCollection;
        delete[] tmpContainerToTkTkCands;
        tmpContainerToTkTkCands = nullptr;
    return;
}
void fourTracksFitter::clearAndInitializeContainer()
{
        nCandsSize = 0;
        tmpContainerSize = 100;
        delete tktkCands;
        delete[] tmpContainerToTkTkCands;

        tmpContainerToTkTkCands = new reco::VertexCompositeCandidate[tmpContainerSize];
        tktkCands = nullptr;
        return;

}

double fourTracksFitter::FDSig( const reco::VertexCompositeCandidate& cand1, const reco::VertexCompositeCandidate& cand2 )
{
    typedef math::Error<3>::type CovarianceMatrix;
    typedef math::XYZPoint Point;
    const Point& vtxCand1 = cand1.vertex();
    const Point& vtxCand2 = cand2.vertex();
    const CovarianceMatrix covCand1 = cand1.vertexCovariance();
    const CovarianceMatrix covCand2 = cand2.vertexCovariance();
    
    typedef ROOT::Math::SMatrix< double, 3, 3, ROOT::Math::MatRepSym<double,3> > SMatrixSym3D;
    typedef ROOT::Math::SVector< double, 3 > SVector3;

    SMatrixSym3D totalCov = covCand1 + covCand2;
    SVector3 distanceVector( vtxCand1.x()-vtxCand2.x(),vtxCand1.y()-vtxCand2.y(), 0. ); 
    double rVtxMag = ROOT::Math::Mag(distanceVector);
    double sigmaRvtxMag = sqrt( ROOT::Math::Similarity(totalCov, distanceVector) ) / rVtxMag;
    return rVtxMag/sigmaRvtxMag;
}
double fourTracksFitter::FDSig( const reco::VertexCompositeCandidate& cand1, const reco::BeamSpot& bSpot )
{
    typedef math::Error<3>::type CovarianceMatrix;
    typedef math::XYZPoint Point;
    const Point& vtxCand1 = cand1.vertex();
    const Point& vtxBSpot = bSpot.position();
    const CovarianceMatrix covCand1 = cand1.vertexCovariance();
    const CovarianceMatrix covBSpot = bSpot.rotatedCovariance3D();
    
    typedef ROOT::Math::SMatrix< double, 3, 3, ROOT::Math::MatRepSym<double,3> > SMatrixSym3D;
    typedef ROOT::Math::SVector< double, 3 > SVector3;

    SMatrixSym3D totalCov = covCand1 + covBSpot;
    SVector3 distanceVector( vtxCand1.x()-vtxBSpot.x(),vtxCand1.y()-vtxBSpot.y(), 0. ); 
    double rVtxMag = ROOT::Math::Mag(distanceVector);
    double sigmaRvtxMag = sqrt( ROOT::Math::Similarity(totalCov, distanceVector) ) / rVtxMag;
    return rVtxMag/sigmaRvtxMag;
}
double fourTracksFitter::Cosa2d( const reco::VertexCompositeCandidate& cand1, const reco::VertexCompositeCandidate& cand2 )
{
    typedef math::Error<3>::type CovarianceMatrix;
    typedef math::XYZPoint Point;
    const Point& vtxCand1 = cand1.vertex();
    const Point& vtxCand2 = cand2.vertex();
    math::XYZVector momCand1 = cand1.momentum();

    GlobalVector posVec( vtxCand1.x()-vtxCand2.x(), vtxCand1.y()-vtxCand2.y(), 0. );
    GlobalVector momVec( momCand1.x()             , momCand1.y()             , 0. );
    return posVec.dot(momVec) / posVec.mag() / momVec.mag();
}

//void fourTracksFitter::recordParingSources(const edm::Event & iEvent, const edm::EventSetup & iSetup)
//{
//    if ( recorded ) return;
//    // Handles for tracks, B-field, and tracker geometry
//    edm::Handle < reco::VertexCompositeCandidateCollection > theMuMuPairHandle;
//    edm::Handle < reco::VertexCompositeCandidateCollection > theTkTkPairHandle;
//
//    iEvent.getByToken(mumuPairToken, theMuMuPairHandle);
//    iEvent.getByToken(tktkPairToken, theTkTkPairHandle);
//    if (!theTkTkPairHandle.isValid()) return;
//    if (!theMuMuPairHandle.isValid()) return;
//    if (!theMuMuPairHandle->size()) return;
//    if (!theTkTkPairHandle->size()) return;
//    recordMuMuCands.reserve( theMuMuPairHandle->size() );
//    recordTkTkCands.reserve( theTkTkPairHandle->size() );
//
//    // load mumuCandidate & tktkCandidate to do the vertexing
//    for ( const VertexCompositeCandidate& mumuCand : *(theMuMuPairHandle.product()) )
//    {
//        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muPosName]) );
//        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muNegName]) );
//        recordMuMuCands.emplace_back(mumuCand);
//
//    }
//        
//    for ( const VertexCompositeCandidate& tktkCand : *(theTkTkPairHandle.product()) )
//    {
//        const reco::RecoChargedCandidate* tkPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkPosName]) );
//        const reco::RecoChargedCandidate* tkNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkNegName]) );
//        recordTkTkCands.emplace_back(tktkCand);
//    }
//
//    recorded = true;
//    return;
//}
//
//void clearRecoredSources()
//{
//    recordMuMuCands.clear();
//    recordTkTkCands.clear();
//    recorded = false;
//    return;
//}
//
//
//fourTracksFitter::recorded = false;
//fourTracksFitter::recordMuMuCands;
//fourTracksFitter::recordTkTkCands;
