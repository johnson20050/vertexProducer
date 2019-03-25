#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <vector>
#include <memory>
#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToPcK.h"

typedef reco::GenParticle MCparticle;
typedef std::vector<reco::GenParticle> MCparticleList;


//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class GenEventSelector : public edm::stream::EDFilter<> {
public:
   explicit GenEventSelector(const edm::ParameterSet&);
   ~GenEventSelector();
   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
   virtual bool filter(edm::Event&, const edm::EventSetup&) override;

   // Data Members
   const edm::EDGetTokenT<MCparticleList>   _mcMatchToken;
   edm::Handle<MCparticleList>              _mcMatchHandle;
   familyRelationShip* _mcDaugDetail;
   
   std::vector<int> _targetPIDs;
};

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------

GenEventSelector::GenEventSelector(const edm::ParameterSet& iConfig):
    _mcMatchToken( consumes<MCparticleList>(iConfig.getParameter<edm::InputTag>("mcmatchsrc"))),
    _targetPIDs( iConfig.getParameter<std::vector<int>>("targetPIDs") )
{
   _mcDaugDetail = new familyRelationShipLbToPcK();
   return;
}

GenEventSelector::~GenEventSelector()
{
    delete _mcDaugDetail;
    return;
}

//------------------------------------------------------------------------------
//   Main Control Flow
//------------------------------------------------------------------------------
bool GenEventSelector::filter( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken( _mcMatchToken, _mcMatchHandle );

    // Object selection
    for ( const MCparticle mc : *(_mcMatchHandle.product()) )
    {
        int pid = mc.pdgId();
        bool reject = true;
        for ( int targetID : _targetPIDs )
        {
            if ( targetID == 0 )
                return true;
            if ( targetID == pid )
                reject = false;
        }
        if ( reject )
            continue;

        if ( _mcDaugDetail->isTargetGenParticleInDecayChannel(mc) )
            return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//   Selection Criterias
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//   EDM Plugin requirements
//------------------------------------------------------------------------------
void GenEventSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
   //The following says we do not know what parameters are allowed so do no validation
   // Please change this to state exactly what you do use, even if it is no parameters
   edm::ParameterSetDescription desc;
   desc.setUnknown();
   descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenEventSelector);
