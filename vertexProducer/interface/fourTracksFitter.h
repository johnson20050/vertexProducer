// -*- C++ -*-
//
// modified from V0Fitter. It is a cmssw code used in RECO and AOD. To find mumutktk combinations.
// This fourTracksFitter class is a abstract class. It is not able to be used directly.
// Its usage is presented in fourTrackFromVCCProducer.cc.
//
// Package:    V0Producer
// Class:      V0Fitter
//
/**\class V0Fitter V0Fitter.h RecoVertex/V0Producer/interface/V0Fitter.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Brian Drell
//         Created:  Fri May 18 22:57:40 CEST 2007
// $Id: V0Fitter.h,v 1.22 2010/06/19 03:24:33 drell Exp $
//
//

#ifndef __fourTracksFitter_H__
#define __fourTracksFitter_H__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
//#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include <string>
#include <fstream>
#include <stdio.h>
#include <memory>

class fourTracksFitter
{
public:
    fourTracksFitter(const edm::ParameterSet& theParams,
                edm::ConsumesCollector&& iC );
    virtual ~fourTracksFitter();

    // Switching to L. Lista's reco::Candidate infrastructure for V0 storage
    const reco::VertexCompositeCandidateCollection& getFourTkCands() const;

protected:
    // STL vector of VertexCompositeCandidate that will be filled with VertexCompositeCandidates by fitAll()
    // due to the characteristics of std::vector. It is not able to use some function in array.
    // Ensure what are you doing before you use some function like "push_back", "erase",
    // which changes the size of the array.
    reco::VertexCompositeCandidateCollection* tktkCands;
    reco::VertexCompositeCandidate* tmpContainerToTkTkCands;
    void enlargeContainer();
    void fillInContainer();

    // size to tmp container. If the number of candidates spill out the tmp contaier, it need to find larger container automatically.
    unsigned nCandsSize;
    unsigned tmpContainerSize;



    // variables to be decided from python file
    double* optD;
    bool*   optB;
    std::string* optS;

    double chi2Cut, rVtxCut, tkDCACut, innerHitPosCut;
    bool useRefTrax;
    std::string vtxFitter;

    enum parD
    {
        FDSigPreCut_tktkTomumu, Cosa2dPreCut_tktkTomumu, 
        FDSigCut_mumutktkToBS, vtxprobCut, 
        mMassPreCut_mumu, MMassPreCut_mumu, mMassPreCut_tktk, MMassPreCut_tktk,
        mCandMassCut, MCandMassCut, 
        ptPreCut_muPos, ptPreCut_muNeg, ptPreCut_tkPos, ptPreCut_tkNeg,
        muPosMass, muNegMass, tkPosMass, tkNegMass, muPosSigma, muNegSigma, tkPosSigma, tkNegSigma,
        mumuMassConstraint, 
        totNumD
    };
    enum parB
    {
        totNumB
    };
    enum parS
    {
        candName, mumuName, tktkName, tkPosName, tkNegName, muPosName, muNegName,
        totNumS
    };

    static bool recorded;
    static edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> mumuPairToken;
    static edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> tktkPairToken;
    static edm::EDGetTokenT<reco::BeamSpot                          > beamspotToken;
    static std::unique_ptr<edm::Handle<reco::BeamSpot>                          > theBeamSpotHandlePtr;
    static std::unique_ptr<edm::Handle<reco::VertexCompositeCandidateCollection>> theMuMuPairHandlePtr;
    static std::unique_ptr<edm::Handle<reco::VertexCompositeCandidateCollection>> theTkTkPairHandlePtr;
    static std::unique_ptr<edm::ESHandle<MagneticField>                         > bFieldHandlePtr;
    static std::unique_ptr<edm::ESHandle<GlobalTrackingGeometry>                > globTkGeomHandlePtr;

public:
    static void initializeEvent( const edm::ParameterSet& theParameters, edm::ConsumesCollector&& iC );
    static void clearEvent();
    static void clearRecoredSources();
    static void recordParingSources( const edm::Event& iEvent, const edm::EventSetup& iSetup );

    // Helper method that does the actual fitting using the KalmanVertexFitter
    virtual void fitAll(const edm::Event& iEvent, const edm::EventSetup& iSetup) = 0;

    std::string getFourTkCandName() { return optS[candName]; }
    bool nothingToWritten() { return nCandsSize == 0 ? true : false; }
    void clearAndInitializeContainer();

    double FDSig(const reco::VertexCompositeCandidate& cand1, const reco::VertexCompositeCandidate& cand2);
    double FDSig(const reco::VertexCompositeCandidate&  cand1, const reco::BeamSpot& bSpot);
    double Cosa2d( const reco::VertexCompositeCandidate& cand1, const reco::VertexCompositeCandidate& cand2 );
};


#endif
