#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "DataFormats/PatCandidates/interface/Muon.h"
#include <vector>

typedef pat::Muon myMuon;
typedef std::vector<myMuon> myMuonList;


//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class MuonProducer : public edm::stream::EDFilter<> {
public:
   explicit MuonProducer(const edm::ParameterSet&);
   ~MuonProducer();
   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   // Public for use !
   bool IsSelectedMuon( const myMuon& mu ) const ;
   bool IsVetoMuon( const myMuon& mu ) const;
private:
   virtual bool filter(edm::Event&, const edm::EventSetup&) override;

   // Data Members
   const edm::EDGetTokenT<myMuonList>       _muonToken;
   edm::Handle<myMuonList>                  _muonHandle;
};

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------

MuonProducer::MuonProducer(const edm::ParameterSet& iConfig):
    _muonToken( consumes<myMuonList>(iConfig.getParameter<edm::InputTag>("muonsrc")))
{
   produces<myMuonList>();
}

MuonProducer::~MuonProducer()
{
}

//------------------------------------------------------------------------------
//   Main Control Flow
//------------------------------------------------------------------------------
bool MuonProducer::filter( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken( _muonToken, _muonHandle);

    std::unique_ptr<myMuonList> selectedMuons( new myMuonList );
    selectedMuons->reserve( _muonHandle->size() );

    // Object selection
    for ( const myMuon& mu : *( _muonHandle.product() ) )
    {
        if ( IsVetoMuon(mu) ) continue;
        if ( IsSelectedMuon(mu) )
            selectedMuons->push_back(mu);
    }

    iEvent.put( std::move(selectedMuons) );
    return true;
}

//------------------------------------------------------------------------------
//   Selection Criterias
//------------------------------------------------------------------------------

bool MuonProducer::IsSelectedMuon( const myMuon& mu ) const
{
   if (mu.pt()<4.0)                                return false;
   if (mu.track()->hitPattern().numberOfValidStripHits()<4) return false;
   if (mu.track()->hitPattern().numberOfValidPixelHits()<1) return false;
   return true;
}

bool MuonProducer::IsVetoMuon( const myMuon& mu ) const
{
   if (mu.track()->normalizedChi2()>5)             return false;
   if (mu.p()>200 || mu.pt()>200)                  return false;
   if (fabs(mu.eta()) > 2.5)                       return false;
   return false;
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
