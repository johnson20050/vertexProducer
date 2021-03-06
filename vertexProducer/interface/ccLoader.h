// ccLoader is used to draw lots of histograms by EDAnalyzer.
// it is a quickly drawing by load labels.
// It can load 'VertexCompositeCandidateCollection' and 'pat::CompositeCandidateCollection'
// but not it is completed for vcc. pat part is incompleted.
// And there is no cut in this code. If you need cut.
// It is recommended to write a tree and cut.
#ifndef __ccLoader_H__
#define __ccLoader_H__

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "DataFormats/Common/interface/Ref.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
//#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include <string>
#include <fstream>
#include <stdio.h>
#include "TH1F.h"
#include "TNtupleD.h"


class ccLoader
{
public:
    ccLoader(const edm::ParameterSet& theParams,
               edm::ConsumesCollector&& iC );
    ~ccLoader();

    // Switching to L. Lista's reco::Candidate infrastructure for V0 storage
    const reco::VertexCompositeCandidateCollection& getCands(unsigned i) const;

private:

    bool useVCC;
    bool usePAT;
    bool useBS;
    std::string vccLabel;
    std::string patLabel;
    std::string beamspotLabel;
    edm::EDGetTokenT< reco::VertexCompositeCandidateCollection > vccToken;
    edm::EDGetTokenT< pat::CompositeCandidateCollection        > patToken;
    edm::EDGetTokenT< reco::BeamSpot > beamspotToken;

    // variables to be decided from python file
    double* optD;
    bool*   optB;
    std::string* optS;
    unsigned nParticles;
    TNtupleD* nt;

    enum parD
    {
        pTkMass, nTkMass, tktkMass, tktkMassCut, vtxSigCut,
        mMassRegion, MMassRegion,

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

    TFileDirectory* _dir;
    std::map<std::string,TH1*> histoMap;

    void createHisto(const std::string& name, int nbin, float hmin, float hmax);
    void fillHisto(const std::string& name, float x);

    bool isKshort( reco::VertexCompositeCandidate& cand );
    bool isKstar ( reco::VertexCompositeCandidate& cand );
    bool isBd    ( reco::VertexCompositeCandidate& cand );
    bool isBs    ( reco::VertexCompositeCandidate& cand );

public:
    // Helper method that does the actual fitting using the KalmanVertexFitter
    void calc(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    void setOutput( TFileDirectory* dir );
    void createHistos();
};

#endif
