// -*- C++ -*-
//
// modified from V0Fitter. It is a cmssw code used in RECO and AOD. To find Lambda0 and Kshort
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

#ifndef __MUMUFITTER_H__
#define __MUMUFITTER_H__

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
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <string>
#include <fstream>

typedef pat::Muon   myMuon;
typedef std::vector<myMuon>  myMuonList;
typedef reco::Track myTrack;
typedef std::vector<myTrack> myTrackList;

class mumuFitter
{
public:
    mumuFitter(const edm::ParameterSet& theParams,
               edm::ConsumesCollector&& iC );
    ~mumuFitter();

    // Switching to L. Lista's reco::Candidate infrastructure for V0 storage
    const reco::VertexCompositeCandidateCollection& getCands(unsigned i) const;

private:
    // STL vector of VertexCompositeCandidate that will be filled with VertexCompositeCandidates by fitAll()
    // due to the characteristics of std::vector. It is not able to use some function in array.
    // Ensure what are you doing before you use some function like "push_back", "erase",
    // which changes the size of the array.
    reco::VertexCompositeCandidateCollection** tktkCands;
    reco::VertexCompositeCandidate** tmpContainerToTkTkCands;
    void enlargeAllContainer();
    void fillInContainer();

    // size to tmp container. If the number of candidates spill out the tmp contaier, it need to find larger container automatically.
    unsigned* nCandsSize;
    unsigned tmpContainerSize;

    // Tracker geometry for discerning hit positions
    const TrackerGeometry* trackerGeom;

    const MagneticField* magField;

    edm::EDGetTokenT<   myMuonList   > selMuonsToken;
    edm::EDGetTokenT< reco::BeamSpot > beamspotToken;


    // variables to be decided from python file
    double** optD;
    bool**   optB;
    std::string** optS;
    unsigned nParticles;

    enum parD
    {
        pTkMass, nTkMass, tktkMass, tktkMassCut, vtxSigCut,
        totNumD
    };
    enum parB
    {
        totNumB
    };
    enum parS
    {
        candName, pTkName, nTkName,
        totNumS
    };

    double chi2Cut, rVtxCut, tkDCACut, innerHitPosCut;
    bool useRefTrax;
    std::string vtxFitter;


public:
    // Helper method that does the actual fitting using the KalmanVertexFitter
    void fitAll(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    double findV0MassError(const GlobalPoint &vtxPos, std::vector<reco::TransientTrack> dauTracks);

    // Applies cuts to the VertexCompositeCandidates after they are fitted/created.
    //void applyPostFitCuts();

    // Stuff for debug file output.
    std::ofstream mPiPiMassOut;

    inline void initFileOutput()
    {
        mPiPiMassOut.open("mPiPi.txt", std::ios::app);
    }
    inline void cleanupFileOutput()
    {
        mPiPiMassOut.close();
    }
    std::string getCandName(unsigned i) { return optS[i][candName]; }
    unsigned getNParticles() { return nParticles; }
    bool nothingToWritten(unsigned i) { return nCandsSize[i] == 0 ? true : false; }
    void clearSomething();
};

#endif
