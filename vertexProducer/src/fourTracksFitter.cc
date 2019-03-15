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

namespace
{
    const float kaMASS = 0.493667;
    const float piMASS = 0.13957018;
    const float muMASS = 0.1056583715;
}

// Constructor and (empty) destructor
fourTracksFitter::fourTracksFitter(const edm::ParameterSet & theParameters,
                       edm::ConsumesCollector&& iC ) :
    tktkCands(nullptr), tmpContainerToTkTkCands(nullptr), nCandsSize(0), tmpContainerSize(100)
{
    using std::string;


    // ------> Initialize parameters from PSet. ALL TRACKED, so no defaults.
    // First set bits to do various things:
    //  -decide whether to use the KVF track smoother, and whether to store those
    //     tracks in the reco::Vertex

    // initialize informations for each candidates.
    optD = new double[totNumD];
    optB = new bool[totNumB];
    optS = new std::string[totNumS];

    optS[candName]   = theParameters.getParameter < string > (string("candName"));
    optS[tktkName]   = theParameters.getParameter < string > (string("tktkName"));
    optS[mumuName]   = theParameters.getParameter < string > (string("mumuName"));

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
    mcDaugDetail = new familyRelationShipLbToPcK();
    
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

    delete mcDaugDetail;
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
bool fourTracksFitter::IsTargetJPsi(const reco::Track& trk1, const reco::Track& trk2, const std::vector<reco::GenParticle>& mcList)
{
    if ( !useMC ) return false;

    bool trk1Match = false;
    bool trk2Match = false;
    for ( const reco::GenParticle& mc : mcList )
    {
        if ( abs(mc.pdgId()) != 5122 ) continue;
        if ( !mcDaugDetail->isTargetMother(mc) ) continue;
        trk1Match = trk2Match = false;

        const reco::Candidate* mcPtr = &mc;
        const reco::Candidate* daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(0); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(0,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( !daugPtr ) continue;
        if ( abs(daugPtr->pdgId()) != 13 ) continue;
        //if ( mcPtr->mother()->pdgId() != 443 ) printf("fourTracksFitter::IsTargetJPsi() ------ Warning, jpsi not found!, you find %d, with mom %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId());
        if ( daugPtr->mother()->pdgId() != 443 )
        {
            //printf("fourTracksFitter::IsTargetJPsi()  1 ------ Warning, jpsi not found!, you find %d, with mom %d and grandma %d fuckyou grandma %d, fuckyoufuckyou grandmu %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId(), mcPtr->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->mother()->pdgId());
            continue;
        }

        if ( mcDaugDetail->truthMatching(trk1, *daugPtr) ) trk1Match = true;
        else
        if ( mcDaugDetail->truthMatching(trk2, *daugPtr) ) trk2Match = true;


        mcPtr = &mc;
        daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(1); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(1,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( !daugPtr ) continue;
        if ( (daugPtr->pdgId()) != 13 ) continue;
        //if ( mcPtr->mother()->pdgId() != 443 ) printf("fourTracksFitter::IsTargetJPsi() ------ Warning, jpsi not found!, you find %d, with mom %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId());
        if ( daugPtr->mother()->pdgId() != 443 )
        {
            //printf("fourTracksFitter::IsTargetJPsi()  2 ------ Warning, jpsi not found!, you find %d, with mom %d and grandma %d fuckyou grandma %d, fuckyoufuckyou grandmu %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId(), mcPtr->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->mother()->pdgId());
            continue;
        }

        if ( mcDaugDetail->truthMatching(trk1, *daugPtr) ) trk1Match = true;
        else
        if ( mcDaugDetail->truthMatching(trk2, *daugPtr) ) trk2Match = true;
        if ( trk1Match && trk2Match )
            return true;
    }

    return false;
}

bool fourTracksFitter::IsTargetCand(const reco::Track& trk3, const reco::Track& trk4, const std::vector<reco::GenParticle>& mcList)
{
    if ( !useMC ) return false;

    bool trk3Match = false;
    bool trk4Match = false;
    for ( const reco::GenParticle& mc : mcList )
    {
        if ( abs(mc.pdgId()) != 5122 ) continue;
        if ( !mcDaugDetail->isTargetMother(mc) ) continue;
        trk3Match = trk4Match = false;
        

        const reco::Candidate* mcPtr = &mc;
        const reco::Candidate* daugPtr = nullptr;
        //const reco::Candidate* daugPtr1 = nullptr; const reco::Candidate* daugPtr2 = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(2); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(2,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( !daugPtr ) continue;
        if ( abs(daugPtr->pdgId()) != 2212 && abs(daugPtr->pdgId()) != 321 )
        {
            //printf("fourTracksFitter::IsTargetCand()  1 ------ Warning, Kaon/Proton not found!, you find %d, with mom %d and grandma %d fuckyou grandma %d, fuckyoufuckyou grandmu %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId(), mcPtr->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->mother()->pdgId());
            continue;
        }


        if ( mcDaugDetail->truthMatching(trk3, *daugPtr) ) trk3Match = true;
        else
        if ( mcDaugDetail->truthMatching(trk4, *daugPtr) ) trk4Match = true;
        mcPtr = &mc;
        //daugPtr1 = daugPtr;
        daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(3); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(3,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( !daugPtr ) continue;
        if ( abs(daugPtr->pdgId()) != 2212 && abs(daugPtr->pdgId()) != 321 )
        { 
            //printf("fourTracksFitter::IsTargetCand()  2 ------ Warning, Kaon/Proton not found!, you find %d, with mom %d and grandma %d fuckyou grandma %d, fuckyoufuckyou grandmu %d------\n", mcPtr->pdgId(), mcPtr->mother()->pdgId(), mcPtr->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->pdgId(), mcPtr->mother()->mother()->mother()->mother()->pdgId());
            continue;
        }
        //daugPtr2 = daugPtr;
        if ( mcDaugDetail->truthMatching(trk3, *daugPtr) ) trk3Match = true;
        else
        if ( mcDaugDetail->truthMatching(trk4, *daugPtr) ) trk4Match = true;
        //printf("tktk matching : (trk1,trk2) = (%d,%d) , mom = (%d,%d)\n", daugPtr1->pdgId(), daugPtr2->pdgId(), daugPtr1->mother()->pdgId(), daugPtr2->mother()->pdgId() );
        if ( trk3Match&&trk4Match )
            return true;
    }

    return false;
}

        

bool fourTracksFitter::overlap( const reco::RecoChargedCandidate& c1, const reco::RecoChargedCandidate& c2 )
{
    //bool checkOverlap = [&c1, &c2]( reco::TrackRef obj1, reco::TrackRef obj2 ) 
    auto checkOverlap = []( reco::TrackRef obj1, reco::TrackRef obj2 ) 
    { return ( !obj1.isNull() && !obj2.isNull() && obj1==obj2 ); };
    if ( checkOverlap(c1.track(), c2.track()) ) return true;
    if ( checkOverlap(c1.track(), c2.standAloneMuon()) ) return true;
    if ( checkOverlap(c1.track(), c2.combinedMuon()) ) return true;

    return false;
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

void fourTracksFitter::recordParingSources(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
    if ( recorded ) return;

    iEvent.getByToken(mumuPairToken, *(theMuMuPairHandlePtr.get()));
    iEvent.getByToken(tktkPairToken, *(theTkTkPairHandlePtr.get()));
    if ( useMC )
        iEvent.getByToken(genMatchToken, *(theGenMatchHandlePtr.get()));

    iSetup.get < IdealMagneticFieldRecord > ().get(*(bFieldHandlePtr.get()));
    iSetup.get < GlobalTrackingGeometryRecord > ().get(*(globTkGeomHandlePtr.get()));
    if (!(*theMuMuPairHandlePtr).isValid()) return;
    if (!(*theTkTkPairHandlePtr).isValid()) return;
    if (!(*theMuMuPairHandlePtr)->size()) return;
    if (!(*theTkTkPairHandlePtr)->size()) return;

    recorded = true;
    return;
}

void fourTracksFitter::clearRecoredSources()
{
    recorded = false;
    usedCandidateMuMu.clear();
    usedCandidateTkTk.clear();
    
    theBeamSpotHandlePtr.release();
    theMuMuPairHandlePtr.release();
    theTkTkPairHandlePtr.release();
    globTkGeomHandlePtr.release();
    theGenMatchHandlePtr.release();
    bFieldHandlePtr.release();
    theBeamSpotHandlePtr.reset(new edm::Handle<reco::BeamSpot >());
    theMuMuPairHandlePtr.reset(new edm::Handle<reco::VertexCompositeCandidateCollection>());
    theTkTkPairHandlePtr.reset(new edm::Handle<reco::VertexCompositeCandidateCollection>());
    theGenMatchHandlePtr.reset(new edm::Handle<std::vector<reco::GenParticle>>());
    globTkGeomHandlePtr.reset(new edm::ESHandle<GlobalTrackingGeometry>());
    bFieldHandlePtr.reset(new edm::ESHandle<MagneticField>());
    return;
}
void fourTracksFitter::excludeKnownParticles( const edm::Event& iEvent, const edm::EventSetup& iSetupe )
{
    if ( !recorded ) return;
    usedCandidateMuMu.clear();
    usedCandidateTkTk.clear();
    const reco::VertexCompositeCandidateCollection& mumuCands = *(theMuMuPairHandlePtr->product());
    const reco::VertexCompositeCandidateCollection& tktkCands = *(theTkTkPairHandlePtr->product());
    const MagneticField* magField = bFieldHandlePtr->product();
    const edm::ESHandle<GlobalTrackingGeometry>& globTkGeomHandle = *(globTkGeomHandlePtr.get());



    // load mumuCandidate & tktkCandidate to do the vertexing
    for ( unsigned mumuIdx = 0; mumuIdx != mumuCands.size(); ++mumuIdx )
    {
        const reco::VertexCompositeCandidate& mumuCand = mumuCands[mumuIdx];
        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuPos") );
        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuNeg") );
        
        
        for ( unsigned tktkIdx = 0; tktkIdx != tktkCands.size(); ++tktkIdx )
        //for ( int tktkIdx = 0; tktkIdx < tktkCands.size(); ++tktkIdx )
        {
            const reco::VertexCompositeCandidate& tktkCand = tktkCands[tktkIdx];
            const reco::RecoChargedCandidate* tkPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter("PiPos") );
            const reco::RecoChargedCandidate* tkNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter("PiNeg") );


            // build trackRef & TransientTracks
            reco::TrackRef muPosTkRef =  muPosCandPtr->track();
            reco::TrackRef muNegTkRef =  muNegCandPtr->track();
            reco::TrackRef tkPosTkRef =  tkPosCandPtr->track();
            reco::TrackRef tkNegTkRef =  tkNegCandPtr->track();

            reco::TransientTrack muPosTransTk( *muPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack muNegTransTk( *muNegTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkPosTransTk( *tkPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkNegTransTk( *tkNegTkRef, magField, globTkGeomHandle );

            // for check
            GlobalPoint tktkVTX ( tktkCand.vertex().x(),tktkCand.vertex().y(),  tktkCand.vertex().z() );
            GlobalPoint mumuVTX ( mumuCand.vertex().x(),mumuCand.vertex().y(),  mumuCand.vertex().z() );
            GlobalVector mpMom = muPosTransTk.trajectoryStateClosestToPoint(mumuVTX).momentum();
            GlobalVector mnMom = muNegTransTk.trajectoryStateClosestToPoint(mumuVTX).momentum();
            GlobalVector tpMom = tkPosTransTk.trajectoryStateClosestToPoint(tktkVTX).momentum();
            GlobalVector tnMom = tkNegTransTk.trajectoryStateClosestToPoint(tktkVTX).momentum();
            if ( fabs(mpMom.mag()-tpMom.mag()) < 0.001 ) continue;
            if ( fabs(mnMom.mag()-tnMom.mag()) < 0.001 ) continue;

            KinematicParticleFactoryFromTransientTrack pFactory;
            std::vector<RefCountedKinematicParticle> mumutktkCand;
            mumutktkCand.reserve(4);
            float m1sig = muMASS*1e-6;
            float m2sig = muMASS*1e-6;
            float m3sig = kaMASS*1e-6;
            float m4sig = piMASS*1e-6;
            mumutktkCand.push_back( pFactory.particle(muPosTransTk,muMASS, 0., 0., m1sig) );
            mumutktkCand.push_back( pFactory.particle(muNegTransTk,muMASS, 0., 0., m2sig) );
            mumutktkCand.push_back( pFactory.particle(tkPosTransTk,kaMASS, 0., 0., m3sig) );
            mumutktkCand.push_back( pFactory.particle(tkNegTransTk,piMASS, 0., 0., m4sig) );

            RefCountedKinematicTree fourTracksKineTree;
            double jpsiMass = 3.096916;
            MultiTrackKinematicConstraint* mumuConstr = new TwoTrackMassKinematicConstraint( jpsiMass );
            KinematicConstrainedVertexFitter kcvFitter;
            fourTracksKineTree = kcvFitter.fit( mumutktkCand, mumuConstr );
            if ( !fourTracksKineTree->isValid() ) continue;
            fourTracksKineTree->movePointerToTheTop();

            RefCountedKinematicParticle candKineParticle = fourTracksKineTree->currentParticle();
            RefCountedKinematicVertex   candKineVertex   = fourTracksKineTree->currentDecayVertex();
            if ( !candKineVertex->vertexIsValid() ) continue;

            reco::Particle::LorentzVector candP4( 
                    candKineParticle->currentState().kinematicParameters().momentum().x(),
                    candKineParticle->currentState().kinematicParameters().momentum().y(),
                    candKineParticle->currentState().kinematicParameters().momentum().z(),
                    candKineParticle->currentState().kinematicParameters().energy() );

            // start to get daughter information.
            fourTracksKineTree->movePointerToTheTop();
            fourTracksKineTree->movePointerToTheFirstChild();
            RefCountedKinematicParticle pMuMomRefitted = fourTracksKineTree->currentParticle();
            reco::Particle::LorentzVector pMuCandP4(
                        pMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        pMuMomRefitted->currentState().kinematicParameters().energy() );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle nMuMomRefitted = fourTracksKineTree->currentParticle();
            reco::Particle::LorentzVector nMuCandP4(
                        nMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        nMuMomRefitted->currentState().kinematicParameters().energy() );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle pTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::Particle::LorentzVector pTkCandP4(
                        pTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        pTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        pTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        pTkMomRefitted->currentState().kinematicParameters().energy() );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle nTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::Particle::LorentzVector nTkCandP4(
                        nTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        nTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        nTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        nTkMomRefitted->currentState().kinematicParameters().energy() );
            reco::Particle::LorentzVector tktkCandP4 = pTkCandP4 + nTkCandP4;

            // build Bd->JPsi + K*(892)
            if ( tktkCandP4.mass()>0.7 )
                if ( tktkCandP4.mass()<1.1 )
                    if ( candP4.mass()>5.2 )
                        if ( candP4.mass()<5.35 )
                            fillPair( mumuIdx, tktkIdx );

            // build Bd->JPsi + K*(1432)
            if ( tktkCandP4.mass()>1.3 )
                if ( tktkCandP4.mass()<1.5 )
                    if ( candP4.mass()>5.2 )
                        if ( candP4.mass()<5.35 )
                            fillPair( mumuIdx, tktkIdx );

            
            // build Bd->JPsi + K*(892)_bar
            pTkCandP4.SetE(sqrt(pTkCandP4.P2()+piMASS*piMASS));
            nTkCandP4.SetE(sqrt(nTkCandP4.P2()+kaMASS*kaMASS));
            tktkCandP4 = pTkCandP4+nTkCandP4;
            candP4 = pMuCandP4+nMuCandP4+tktkCandP4;
            if ( tktkCandP4.mass()>0.7 )
                if ( tktkCandP4.mass()<1.1 )
                    if ( candP4.mass()>5.2 )
                        if ( candP4.mass()<5.35 )
                            fillPair( mumuIdx, tktkIdx );

            // build Bd->JPsi + K*(1432)_bar
            if ( tktkCandP4.mass()>0.7 )
                if ( tktkCandP4.mass()<1.1 )
                    if ( candP4.mass()>5.2 )
                        if ( candP4.mass()<5.35 )
                            fillPair( mumuIdx, tktkIdx );

        } // tktkPair loop end
    } // mumuPair loop end
}

void fourTracksFitter::initializeEvent( const edm::ParameterSet& theParameters, edm::ConsumesCollector&& iC )
{
    beamspotToken = iC.consumes<reco::BeamSpot>( theParameters.getParameter<edm::InputTag>("beamspotLabel") );
    tktkPairToken = iC.consumes<reco::VertexCompositeCandidateCollection>( theParameters.getParameter<edm::InputTag>("tktkCandLabel") );
    mumuPairToken = iC.consumes<reco::VertexCompositeCandidateCollection>( theParameters.getParameter<edm::InputTag>("mumuCandLabel") );
    genMatchToken = iC.consumes<std::vector<reco::GenParticle>>( theParameters.getParameter<edm::InputTag>("genMatchLabel") );
    return;
}
void fourTracksFitter::clearEvent()
{
    return;
}
bool fourTracksFitter::usedPair(unsigned mumuIdx, unsigned tktkIdx ) const
{
    for ( unsigned recMuMuIdx : usedCandidateMuMu )
        if ( recMuMuIdx == mumuIdx )
            return true;
    for ( unsigned recTkTkIdx : usedCandidateTkTk )
        if ( recTkTkIdx == tktkIdx )
            return true;
    return false;
}

void fourTracksFitter::fillPair( unsigned mumuIdx, unsigned tktkIdx )
{
    bool muTag = true;
    bool tkTag = true;
    for ( unsigned mIdx : usedCandidateMuMu )
        if ( mumuIdx == mIdx )
            muTag = false;
    for ( unsigned tIdx : usedCandidateTkTk )
        if ( tktkIdx == tIdx )
            tkTag = false;
    if ( muTag ) usedCandidateMuMu.emplace_back(muTag);
    if ( tkTag ) usedCandidateTkTk.emplace_back(tkTag);
    return;
}

bool fourTracksFitter::recorded = false;
bool fourTracksFitter::useMC = false;
// Handles for tracks, B-field, and tracker geometry
std::unique_ptr<edm::Handle<reco::BeamSpot                          >> fourTracksFitter::theBeamSpotHandlePtr;
std::unique_ptr<edm::Handle<reco::VertexCompositeCandidateCollection>> fourTracksFitter::theMuMuPairHandlePtr;
std::unique_ptr<edm::Handle<reco::VertexCompositeCandidateCollection>> fourTracksFitter::theTkTkPairHandlePtr;
std::unique_ptr<edm::Handle<std::vector<reco::GenParticle>>          > fourTracksFitter::theGenMatchHandlePtr;
std::unique_ptr<edm::ESHandle<GlobalTrackingGeometry>                > fourTracksFitter::globTkGeomHandlePtr;
std::unique_ptr<edm::ESHandle<MagneticField>                         > fourTracksFitter::bFieldHandlePtr;
edm::EDGetTokenT< reco::VertexCompositeCandidateCollection > fourTracksFitter::mumuPairToken;
edm::EDGetTokenT< reco::VertexCompositeCandidateCollection > fourTracksFitter::tktkPairToken;
edm::EDGetTokenT< std::vector<reco::GenParticle>           > fourTracksFitter::genMatchToken;
edm::EDGetTokenT< reco::BeamSpot                           > fourTracksFitter::beamspotToken;
std::vector<unsigned> fourTracksFitter::usedCandidateMuMu;
std::vector<unsigned> fourTracksFitter::usedCandidateTkTk;
