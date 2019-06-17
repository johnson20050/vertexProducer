#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <vector>
#include <memory>
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdGeneral.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdToJPsiKpi.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdToJPsiKstar1430.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdToJPsiKstar892.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBsGeneral.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBsToJPsiKK.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipBsToJPsiPhi.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToJPsiL0.h"
#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToJPsipK.h"
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

   enum mcChannels
   { 
       channelLbL0=0, channelLbToJPsipK=1, channelLbToPcK=2,
       channelBsGeneral=3, channelBsToJPsiKK=4, channelBsToJPsiPhi=5,
       channelBdGeneral=6, channelBdToJPsiKpi=7, channelBdToJPsiKstar1430=8, channelBdToJPsiKstar892=9,
       totChannels };
};

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------

GenEventSelector::GenEventSelector(const edm::ParameterSet& iConfig):
    _mcMatchToken( consumes<MCparticleList>(iConfig.getParameter<edm::InputTag>("mcmatchsrc"))),
    _targetPIDs( iConfig.getParameter<std::vector<int>>("targetPIDs") )
{
    int mcChannel = iConfig.getParameter<int>("mcChannel");
    switch ( mcChannel )
    {
        case channelLbL0:
            _mcDaugDetail = new familyRelationShipLbToJPsiL0(); break;
        case channelLbToJPsipK:
            _mcDaugDetail = new familyRelationShipLbToJPsipK(); break;
        case channelLbToPcK:
            _mcDaugDetail = new familyRelationShipLbToJPsipK(); break;
        case channelBsGeneral:
            _mcDaugDetail = new familyRelationShipBsGeneral(); break;
        case channelBsToJPsiKK:
            _mcDaugDetail = new familyRelationShipBsToJPsiKK(); break;
        case channelBsToJPsiPhi:
            _mcDaugDetail = new familyRelationShipBsToJPsiPhi(); break;
        case channelBdGeneral:
            _mcDaugDetail = new familyRelationShipBdGeneral(); break;
        case channelBdToJPsiKpi:
            _mcDaugDetail = new familyRelationShipBdToJPsiKpi(); break;
        case channelBdToJPsiKstar1430:
            _mcDaugDetail = new familyRelationShipBdToJPsiKstar1430(); break;
        case channelBdToJPsiKstar892:
            _mcDaugDetail = new familyRelationShipBdToJPsiKstar892(); break;
        default:
            std::cerr<<"GenEventSelector::constructor : ----ERROR---- mcChannel illegal. please check the value in python file."<<std::endl;
            exit(128);
    }

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
   desc.add<edm::InputTag>("mcmatchsrc", edm::InputTag(""));
   desc.add<std::vector<int>>("targetPIDs", std::vector<int>());
   desc.add<int>("mcChannel", totChannels);
   desc.setUnknown();
   descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenEventSelector);
#ifndef __familyRelationShip_h__
#define __familyRelationShip_h__
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"

class familyRelationShip
{
public:
    familyRelationShip(int nDaug );
    virtual ~familyRelationShip();
    virtual void fillDaugIdx() = 0;
    static int encodeBOOL(int idx) { return 1 << idx; }
    static bool passCut(int recorder, int idx) { return (recorder>>idx)%2; }
    int daugLayer(int iDaug);
    unsigned getDaughterIdxOnLayer( int iDaug, int layer );
    unsigned getNDaug() const { return _nDaug; }
    //define how to do truth matching
    static bool truthMatching(const reco::Candidate& cand, const reco::Candidate& mcParticle);
    static bool truthMatching(const reco::Track& trk, const reco::Candidate& mcParticle);

    // just for testing
    static double deltaR_Significance(const reco::Track& trk, const reco::Candidate& mcParticle);
    static double deltaR_Value(const reco::Track& trk, const reco::Candidate& mcParticle);


    // if the gen particle was the particle we need, pass this function.
    bool isTargetGenParticleInDecayChannel(const reco::GenParticle& mc);
    

    void setupDaugPID( std::vector<unsigned> pidList ) { finalStatePID = pidList; return; }
protected:
    unsigned *daugLayer1Idx;
    unsigned *daugLayer2Idx;
    unsigned *daugLayer3Idx;
    unsigned _nDaug;
private:
    std::vector<unsigned> finalStatePID;
};

#endif
