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

//typedef pat::GenericParticle myTrack;
typedef reco::Track myTrack;
typedef std::vector < myTrack > myTrackList;
typedef pat::Muon myMuon;
typedef std::vector < myMuon > myMuonList;


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
    //bool IsSelectedTrack(const myTrack &) const;
    bool IsSelectedTrack(unsigned tkIdx) const;
    bool IsVetoTrack(unsigned tkIdx) const;
    bool IsMuonTrack(unsigned tkIdx, const myMuonList & muList) const;
private:
    virtual bool filter(edm::Event &, const edm::EventSetup &) override;
    bool hasBfield() const { return ownBfield; }
    bool hasTkGeometry() const { return ownTkGeometry; }
    bool hasBS() const { return ownBS; }

    // Data Members
    const edm::EDGetTokenT<reco::BeamSpot>  _bsToken;
    const edm::EDGetTokenT < myMuonList >   _muonToken;
    const edm::EDGetTokenT < myTrackList >  _tkToken;

    edm::Handle<reco::BeamSpot> _bsHandle;
    edm::Handle < myMuonList >  _muonHandle;
    edm::Handle < myTrackList > _tkHandle;
    edm::ESHandle<MagneticField> bFieldHandle;
    edm::ESHandle<GlobalTrackingGeometry> globTkGeomHandle;
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
    ownBfield(false), ownTkGeometry(false), ownBS(false)
{
    produces < myTrackList > ();
    // Second, initialize post-fit cuts
    tkChi2Cut = iConfig.getParameter < double >("tkChi2Cut");
    tkNhitsCut = iConfig.getParameter < int >("tkNhitsCut");
    impactParameterSigCut = iConfig.getParameter < double >("impactParameterSigCut");
    std::vector < std::string > qual = iConfig.getParameter < std::vector < std::string > >("trackQualities");
    for (unsigned int ndx = 0; ndx < qual.size(); ndx++)
        qualities.push_back(reco::TrackBase::qualityByName(qual[ndx]));
}

TrackProducer::~TrackProducer()
{
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

    beamspot = *_bsHandle;
    const myMuonList muList = *_muonHandle;
    ownBfield = bFieldHandle.isValid();
    ownTkGeometry = globTkGeomHandle.isValid();
    ownBS = _bsHandle.isValid();

    std::unique_ptr < myTrackList > selectedTracks(new myTrackList);
    selectedTracks->reserve(_tkHandle->size());

    // Object selection
    //for (const myTrack & tk:*(_tkHandle.product()))
    for ( unsigned idx = 0; idx < _tkHandle->size(); ++idx )
    {
        const myTrack& tk = _tkHandle.product()->at(idx);
        if (IsVetoTrack(idx)) continue;
        if (IsMuonTrack(idx, muList)) continue;
        if (IsSelectedTrack(idx))
            selectedTracks->push_back(tk);
    }
    if (selectedTracks->size() < 2)
        return false;


    iEvent.put(std::move(selectedTracks));
    return true;
}

//------------------------------------------------------------------------------
//   Selection Criterias
//------------------------------------------------------------------------------

bool TrackProducer::IsSelectedTrack(unsigned tkIdx) const
{
    //if (tk.pt()<0.8)                                return false;
    //if (tk.track()->hitPattern().numberOfValidStripHits()<4) return false;
    //if (tk.track()->hitPattern().numberOfValidPixelHits()<1) return false;
    
    reco::TrackRef tkRef( _tkHandle, tkIdx );
    
    if ( !tkRef.isNonnull() ) return false;
    if ( !tkRef.isAvailable() ) return false;

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
    if (tkRef->normalizedChi2() > tkChi2Cut) return false;
    if (tkRef->numberOfValidHits() < tkNhitsCut) return false;
    //reco::TransientTrack tmpTk(*tkRef, bFieldHandle.product(), globTkGeomHandle);

    FreeTrajectoryState initialFTS = trajectoryStateTransform::initialFreeState(*tkRef, bFieldHandle.product());
    TSCBLBuilderNoMaterial blsBuilder;
    TrajectoryStateClosestToBeamLine tscb(blsBuilder(initialFTS, beamspot));

    if (!tscb.isValid()) return false;

    // cut for displaced track
    if (tscb.transverseImpactParameter().significance() < impactParameterSigCut) return false;

    return true;
}

bool TrackProducer::IsVetoTrack(unsigned tkIdx) const
{
    return false;
}

bool TrackProducer::IsMuonTrack(unsigned tkIdx, const myMuonList & muList) const
{
    bool isMuonTrack = false;
    const myTrack& tk = _tkHandle.product()->at(tkIdx);
    for (const myMuon & mu:muList)
    {
        if (mu.track().isNonnull())
            continue;
        if (fabs(tk.pt() - mu.track()->pt()) < 0.00001 &&
                fabs(tk.eta() - mu.track()->eta()) < 0.00001 && fabs(tk.phi() - mu.track()->phi()) < 0.00001)
        {
            isMuonTrack = true;
            break;
        }
    }
    return isMuonTrack;
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
