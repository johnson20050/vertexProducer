#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <vector>
#include <memory>
#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToPcK.h"

typedef pat::Muon myMuon;
typedef std::vector<myMuon> myMuonList;
typedef reco::GenParticle MCparticle;
typedef std::vector<reco::GenParticle> MCparticleList;


//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class MuonProducer : public edm::stream::EDFilter<> {
public:
   explicit MuonProducer(const edm::ParameterSet&);
   ~MuonProducer();
   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   // Public for use !
   bool IsTargetMuon( const myMuon& mu ) const;
   bool IsVetoMuon( const myMuon& mu, int& cutRecord ) const;
   bool IsSelectedMuon( const myMuon& mu, int& cutRecord ) const ;
private:
   virtual bool filter(edm::Event&, const edm::EventSetup&) override;

   // Data Members
   const edm::EDGetTokenT<myMuonList>       _muonToken;
   const edm::EDGetTokenT<MCparticleList>   _mcMatchToken;
   edm::Handle<myMuonList>                  _muonHandle;
   edm::Handle<MCparticleList>              _mcMatchHandle;
   familyRelationShip* mcDaugDetail;
   
   bool _useMC;
};

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------

MuonProducer::MuonProducer(const edm::ParameterSet& iConfig):
    _muonToken( consumes<myMuonList>(iConfig.getParameter<edm::InputTag>("muonsrc"))),
    _mcMatchToken( consumes<MCparticleList>(iConfig.getParameter<edm::InputTag>("mcmatchsrc"))),
    _useMC( iConfig.getParameter<bool>("useMC") )
{
   produces<myMuonList>();
   if ( _useMC )
   {
      mcDaugDetail = new familyRelationShipLbToPcK();
//      std::cout << "muonProducer : mc used\n";
   }

   produces<std::vector<int>>("MuonPreselectionEfficiencyBoolInt");
   return;
}

MuonProducer::~MuonProducer()
{
    if ( _useMC )
        delete mcDaugDetail;
}

//------------------------------------------------------------------------------
//   Main Control Flow
//------------------------------------------------------------------------------
bool MuonProducer::filter( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken( _muonToken, _muonHandle );
    if ( _useMC )
        iEvent.getByToken( _mcMatchToken, _mcMatchHandle );

    std::unique_ptr<myMuonList> selectedMuons( new myMuonList );
    std::unique_ptr<std::vector<int>> muonTriggerResult( new std::vector<int> );
    selectedMuons->reserve( _muonHandle->size() );
    muonTriggerResult->reserve( _muonHandle->size() );

    // Object selection
    for ( const myMuon& mu : *( _muonHandle.product() ) )
    {
        int cutRecord = 0;

        // if MC target is found, let it to be -1. final result will be minus.
        // or result is plus.
        int isTarget = 1;
        if ( _useMC )
            if ( IsTargetMuon(mu) )
                isTarget = -1;

        if ( IsVetoMuon(mu, cutRecord) )
            goto recordCutArea;
        if ( IsSelectedMuon(mu,cutRecord) )
        {
            cutRecord+= 1<<0;
            selectedMuons->push_back(mu);
        }
        recordCutArea:
            muonTriggerResult->push_back(cutRecord*isTarget);
    }

    //unsigned keepEvent = selectedMuons->size();
    iEvent.put( std::move(selectedMuons) );
    iEvent.put( std::move(muonTriggerResult), "MuonPreselectionEfficiencyBoolInt" );
    
    //if ( keepEvent )
        return true;
    //else
    //    return false;
}

//------------------------------------------------------------------------------
//   Selection Criterias
//------------------------------------------------------------------------------
bool MuonProducer::IsTargetMuon( const myMuon& mu ) const
{
//    std::cout << "MuonProducer::IstargetMuon() 01\n";
    if ( !_useMC ) return false;
//    std::cout << "MuonProducer::IstargetMuon() 02\n";
    if ( !_mcMatchHandle.isValid() ) return false;
//    std::cout << "MuonProducer::IstargetMuon() 03\n";

    for ( const MCparticle& mc : *(_mcMatchHandle.product()) )
    {
//    std::cout << "MuonProducer::IstargetMuon() 04.1\n";
        if ( fabs(mc.pdgId()) != 5122 ) continue;
//    std::cout << "MuonProducer::IstargetMuon() 04.2\n";
        if ( !mcDaugDetail->isTargetMother(mc) ) continue;
//    std::cout << "MuonProducer::IstargetMuon() 04.3\n";

        const reco::Candidate* mcPtr = &mc;
        const reco::Candidate* daugPtr = nullptr;
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(0); ++layerIdx )
        {
//    std::cout << "MuonProducer::IstargetMuon() 04.31\n";
//    std::cout << "MuonProducer::IstargetMuon() 04.32\n";
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(0,layerIdx) );
            mcPtr = daugPtr;
//    std::cout << "MuonProducer::IstargetMuon() 04.33\n";
        }
//    std::cout << "MuonProducer::IstargetMuon() 04.4\n";
//    std::cout << "MuonProducer::IstargetMuon() 04.5\n";
        if ( !daugPtr ) continue;
//    std::cout << "MuonProducer::IstargetMuon() 04.6\n";
        if ( mcDaugDetail->truthMatching(mu, *daugPtr) ) return true;
//    std::cout << "MuonProducer::IstargetMuon() 04.7\n";
        mcPtr = &mc;
        daugPtr = nullptr;
//    std::cout << "MuonProducer::IstargetMuon() 04.8\n";
        for ( int layerIdx = 0; layerIdx < mcDaugDetail->daugLayer(1); ++layerIdx )
        {
            daugPtr = mcPtr->daughter( mcDaugDetail->getDaughterIdxOnLayer(1,layerIdx) );
            mcPtr = daugPtr;
        }
//    std::cout << "MuonProducer::IstargetMuon() 04.9\n";
        if ( !daugPtr ) continue;
        if ( mcDaugDetail->truthMatching(mu, *daugPtr) ) return true;
//    std::cout << "MuonProducer::IstargetMuon() 04.10\n";
    }

//    std::cout << "MuonProducer::IstargetMuon() end\n";
    return false;
}

bool MuonProducer::IsVetoMuon( const myMuon& mu, int& cutRecord ) const
{
   cutRecord += 1 << 1; // orig number
   if (mu.track()->normalizedChi2()>5)             return true;
   cutRecord += 1 << 3; // pass nChi2
   if (mu.p()>200 || mu.pt()>200)                  return true;
   cutRecord += 1 << 4; // pass p, pt
   if (fabs(mu.eta()) > 2.5)                       return true;
   cutRecord += 1 << 5; // pass eta & final result
   return false;
}

bool MuonProducer::IsSelectedMuon( const myMuon& mu, int& cutRecord ) const
{
   //if (mu.track()->hitPattern().numberOfValidStripHits()<4) return false;
   //if (mu.track()->hitPattern().numberOfValidPixelHits()<1) return false;
   //
   //updata cut value from 
   //https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2?fbclid=IwAR00mz2e1K0lEFNRuD00Nehbs81TmM3c8kS7IMCgIH8ZU0IWMfamPrJP7ng#Tight_Muon
   cutRecord += 1 << 7; // orig number
   if (!muon::isGoodMuon(mu, muon::TMOneStationTight) ) return false;
   cutRecord += 1 <<  9; // pass isGoodMuon
   if ( mu.innerTrack()->hitPattern().pixelLayersWithMeasurement() < 0 ) return false;
   cutRecord += 1 << 10; // pass pixelLayerWithmeansurement
   if ( mu.innerTrack()->hitPattern().trackerLayersWithMeasurement() < 5 ) return false;
   cutRecord += 1 << 11; // pass trackerLayerWithMeasurement
   if (!mu.innerTrack()->quality(reco::TrackBase::highPurity) ) return false;
   cutRecord += 1 << 12; // pass quality & final result
   if (mu.pt()<4.0)                                return false;
   cutRecord += 1 << 13; // pass pt & final result
   
   return true;
}

//------------------------------------------------------------------------------
//   EDM Plugin requirements
//------------------------------------------------------------------------------
void MuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
   //The following says we do not know what parameters are allowed so do no validation
   // Please change this to state exactly what you do use, even if it is no parameters
   edm::ParameterSetDescription desc;
   desc.setUnknown();
   descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonProducer);
