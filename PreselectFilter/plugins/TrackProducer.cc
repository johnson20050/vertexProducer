#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include <vector>
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
//#include "BPHAnalysis/SpecificDecay/interface/usefulFuncs.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToPcK.h"

//typedef pat::GenericParticle myTrack;
typedef reco::Track myTrack;
typedef std::vector < myTrack > myTrackList;
typedef pat::Muon myMuon;
typedef std::vector < myMuon > myMuonList;
typedef reco::GenParticle MCparticle;
typedef std::vector<reco::GenParticle> MCparticleList;


//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class TrackProducer:public edm::stream::EDFilter <>
{
public:
    explicit TrackProducer(const edm::ParameterSet &);
    ~TrackProducer();
    static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);

    // Public for use !
    bool IsTargetTrack(unsigned tkIdx) const;
    bool IsMuonTrack(unsigned tkIdx, const myMuonList & muList, int& cutRecord) const;
    bool IsVetoTrack(unsigned tkIdx, int& cutRecord) const;
    bool IsSelectedTrack(unsigned tkIdx, int& cutRecord) const;
private:
    virtual bool filter(edm::Event &, const edm::EventSetup &) override;
    bool hasBfield() const { return ownBfield; }
    bool hasTkGeometry() const { return ownTkGeometry; }
    bool hasBS() const { return ownBS; }

    // Data Members
    const edm::EDGetTokenT<reco::BeamSpot>  _bsToken;
    const edm::EDGetTokenT < myMuonList >   _muonToken;
    const edm::EDGetTokenT < myTrackList >  _tkToken;
    const edm::EDGetTokenT<MCparticleList>  _mcMatchToken;

    edm::Handle<reco::BeamSpot> _bsHandle;
    edm::Handle < myMuonList >  _muonHandle;
    edm::Handle < myTrackList > _tkHandle;
    edm::Handle<MCparticleList> _mcMatchHandle;
    edm::ESHandle<MagneticField> bFieldHandle;
    edm::ESHandle<GlobalTrackingGeometry> globTkGeomHandle;

    familyRelationShip* mcDaugDetail;
    bool _useMC;

    reco::BeamSpot beamspot;
    bool ownBfield, ownTkGeometry, ownBS;

    // Cuts
    double tkChi2Cut;
    int tkNhitsCut;
    double impactParameterSigCut;
    std::vector<reco::TrackBase::TrackQuality> qualities;

};

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------

TrackProducer::TrackProducer(const edm::ParameterSet & iConfig):
    _bsToken( consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("bssrc"))),
    _muonToken(consumes < myMuonList > (iConfig.getParameter < edm::InputTag > ("muonsrc"))),
    _tkToken(consumes < myTrackList > (iConfig.getParameter < edm::InputTag > ("tracksrc"))),
    _mcMatchToken(consumes<MCparticleList>(iConfig.getParameter<edm::InputTag>("mcmatchsrc"))),
    _useMC(iConfig.getParameter<bool>("useMC") ),
    ownBfield(false), ownTkGeometry(false), ownBS(false)
{
    produces < myTrackList > ();
    produces<std::vector<int>>("TrackPreselectionEfficiencyBoolInt");
    // Second, initialize post-fit cuts
    tkChi2Cut = iConfig.getParameter < double >("tkChi2Cut");
    tkNhitsCut = iConfig.getParameter < int >("tkNhitsCut");
    impactParameterSigCut = iConfig.getParameter < double >("impactParameterSigCut");
    std::vector < std::string > qual = iConfig.getParameter < std::vector < std::string > >("trackQualities");
    for (unsigned int ndx = 0; ndx < qual.size(); ndx++)
        qualities.push_back(reco::TrackBase::qualityByName(qual[ndx]));
    if ( _useMC )
    {
        mcDaugDetail = new familyRelationShipLbToPcK();
        produces<myTrackList>("TrackPreselectionMatchedTracks");
    }
    return;
}

TrackProducer::~TrackProducer()
{
    if ( _useMC )
        delete mcDaugDetail;
    return;
}

//------------------------------------------------------------------------------
//   Main Control Flow
//------------------------------------------------------------------------------
bool TrackProducer::filter(edm::Event & iEvent, const edm::EventSetup & iSetup)
{
    iEvent.getByToken( _bsToken, _bsHandle );
    iEvent.getByToken(_muonToken, _muonHandle);
    iEvent.getByToken(_tkToken, _tkHandle);
    iSetup.get<IdealMagneticFieldRecord>().get(bFieldHandle);
    iSetup.get<GlobalTrackingGeometryRecord>().get(globTkGeomHandle);
    if ( _useMC )
        iEvent.getByToken( _mcMatchToken, _mcMatchHandle );

    beamspot = *_bsHandle;
    const myMuonList muList = *_muonHandle;
    ownBfield = bFieldHandle.isValid();
    ownTkGeometry = globTkGeomHandle.isValid();
    ownBS = _bsHandle.isValid();

    std::unique_ptr < myTrackList > selectedTracks(new myTrackList);
    std::unique_ptr < myTrackList > matchedTracks(new myTrackList);
    std::unique_ptr < std::vector<int>> trackTriggerResult( new std::vector<int> );
    selectedTracks->reserve(_tkHandle->size());
    matchedTracks->reserve(5);
    trackTriggerResult->reserve(_tkHandle->size());

    // Object selection
    for ( unsigned idx = 0; idx < _tkHandle->size(); ++idx )
    {
        int cutRecord = 0;
        const myTrack& tk = _tkHandle.product()->at(idx);
        
        // if MC target is found, let it to be -1. final result will be minus.
        // of it it plus.
        int isTarget = 1;
        
        if ( _useMC )
            if ( IsTargetTrack(idx) )
                isTarget = -1;

        if (IsVetoTrack(idx, cutRecord)) goto recordCutArea;
        if (IsMuonTrack(idx, muList, cutRecord)) goto recordCutArea;
        if (IsSelectedTrack(idx, cutRecord))
        {
            cutRecord += 1<<0;
            selectedTracks->push_back(tk);
            if ( isTarget < 0 )
                matchedTracks->push_back(tk);
        }

recordCutArea:
        trackTriggerResult->push_back(cutRecord*isTarget);
    }

    iEvent.put(std::move(selectedTracks));
    if ( _useMC )
        iEvent.put(std::move(matchedTracks), "TrackPreselectionMatchedTracks");
    iEvent.put(std::move(trackTriggerResult), "TrackPreselectionEfficiencyBoolInt");
    return true;
}

//------------------------------------------------------------------------------
//   Selection Criterias
//------------------------------------------------------------------------------

bool TrackProducer::IsTargetTrack(unsigned tkIdx) const
{
    if ( !_useMC ) return false;
    const myTrack& tk = _tkHandle.product()->at(tkIdx);

    for ( const MCparticle& mc : *(_mcMatchHandle.product()) )
    {
        if ( fabs(mc.pdgId()) != 5122 ) continue;
        if ( !mcDaugDetail->isTargetGenParticleInDecayChannel(mc) ) continue;

        const reco::Candidate* mcPtr = &mc;
        const reco::Candidate* daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(2); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(2,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( daugPtr )
            if ( mcDaugDetail->truthMatching(tk, *daugPtr) ) return true;
        mcPtr = &mc;
        daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(3); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(3,layerIdx) );
            mcPtr = daugPtr;
        }
        if ( daugPtr )
            if ( mcDaugDetail->truthMatching(tk, *daugPtr) ) return true;
    }

    return false;
}

bool TrackProducer::IsVetoTrack(unsigned tkIdx, int& cutRecord) const
{
    cutRecord += 1 << 1; // orig number
    return false;
}

bool TrackProducer::IsMuonTrack(unsigned tkIdx, const myMuonList & muList, int& cutRecord) const
{
    cutRecord += 1 << 3; // orig number
    bool isMuonTrack = false;
    const myTrack& tk = _tkHandle.product()->at(tkIdx);
    cutRecord += 1 << 4; // pass isMuonTrack
    for (const myMuon & mu:muList)
    {
        if (mu.track().isNonnull())
            continue;
        if (fabs(tk.pt() - mu.track()->pt()) < 0.001 &&
                fabs(tk.eta() - mu.track()->eta()) < 0.001 && fabs(tk.phi() - mu.track()->phi()) < 0.001)
        {
            isMuonTrack = true;
            cutRecord -= 1 << 4; // 'reject' isMuonTrack
            break;
        }
    }
    return isMuonTrack;
}

bool TrackProducer::IsSelectedTrack(unsigned tkIdx, int& cutRecord) const
{
    //if (tk.pt()<0.8)                                return false;
    //if (tk.track()->hitPattern().numberOfValidStripHits()<4) return false;
    //if (tk.track()->hitPattern().numberOfValidPixelHits()<1) return false;
    
    cutRecord += 1 << 6; // orig number
    reco::TrackRef tkRef( _tkHandle, tkIdx );
    
    if ( !tkRef.isNonnull() ) return false;
    cutRecord += 1 << 7; // pass non null
    if ( !tkRef.isAvailable() ) return false;
    cutRecord += 1 << 8; // pass available

    // if 'trackQualities' are set, check the trackRef quality. Or skip such check.
    bool quality_ok = true;
    if (qualities.size() != 0)
    {
        quality_ok = false;
        for (unsigned int ndx_ = 0; ndx_ < qualities.size(); ndx_++)
            if (tkRef->quality(qualities[ndx_]))
            {
                quality_ok = true;
                break;
            }
    }
    if (!quality_ok) return false;
    cutRecord += 1 << 9; // pass quality
    if (tkRef->normalizedChi2() > tkChi2Cut) return false;
    cutRecord += 1 << 10; // pass nChi2
    if (tkRef->numberOfValidHits() < tkNhitsCut) return false;
    cutRecord += 1 << 11; // pass numberOfValidHits
    //reco::TransientTrack tmpTk(*tkRef, bFieldHandle.product(), globTkGeomHandle);

    FreeTrajectoryState initialFTS = trajectoryStateTransform::initialFreeState(*tkRef, bFieldHandle.product());
    TSCBLBuilderNoMaterial blsBuilder;
    TrajectoryStateClosestToBeamLine tscb(blsBuilder(initialFTS, beamspot));

    if (!tscb.isValid()) return false;
    cutRecord += 1 << 12; // success to build tractory state

    // cut for displaced track
    if (tscb.transverseImpactParameter().significance() < impactParameterSigCut) return false;
    cutRecord += 1 << 13; // pass impact parameter

    return true;
}

//------------------------------------------------------------------------------
//   EDM Plugin requirements
//------------------------------------------------------------------------------
void TrackProducer::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(TrackProducer);
