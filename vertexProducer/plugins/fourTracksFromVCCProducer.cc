// -*- C++ -*-
//
// Package:    V0Producer
// Class:      V0Producer
//
/**\class V0Producer V0Producer.cc MyProducers/V0Producer/src/V0Producer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Brian Drell
//         Created:  Fri May 18 22:57:40 CEST 2007
// $Id: V0Producer.cc,v 1.11 2009/12/18 20:45:10 wmtan Exp $
//
//


// system include files
#include <memory>
#include <string>

#include "vertexProducer/vertexProducer/plugins/fourTracksFromVCCProducer.h"
#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod1.h"
#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod2.h"
#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod3.h"

// Constructor
fourTracksFromVCCProducer::fourTracksFromVCCProducer(const edm::ParameterSet& iConfig)
{
    // Registering V0 Collections
    const std::vector< edm::ParameterSet >& subConfigs
        = iConfig.getParameter < std::vector < edm::ParameterSet >> ("recoOptions");
    myFitter.reserve( subConfigs.size() );
    fourTracksFitter::initializeEvent( iConfig, consumesCollector() );
    fourTracksFitter::UseMC( iConfig.getParameter<bool>("useMC") );

    for ( const auto& subConfig : subConfigs )
    {
        const std::string& candName = subConfig.getParameter<std::string>("candName");
        char efficiencyName[60];
        sprintf( efficiencyName, "fourTkVertexingEfficiencyBoolInt%s", candName.c_str() );
        produces< std::vector<int> >( efficiencyName );
        produces< reco::VertexCompositeCandidateCollection >(candName.c_str());

        switch ( subConfig.getParameter<int>("fittingMethod") )
        {
            case 1:
                myFitter.push_back( new fourTracksFittingMethod1(subConfig, consumesCollector()) ); break;
            case 2:
                myFitter.push_back( new fourTracksFittingMethod2(subConfig, consumesCollector()) ); break;
            case 3:
                myFitter .push_back( new fourTracksFittingMethod3(subConfig, consumesCollector()) ); break;
            default:
                break;
                printf("fourTracksFromVCCProducer::Constructor() : fitting method number not allowed!\n");
        }
    }
    return;
}

// (Empty) Destructor
fourTracksFromVCCProducer::~fourTracksFromVCCProducer()
{
    for ( const auto& fitter : myFitter )
        delete fitter;
    return;
}


//
// Methods
//

// Producer Method
void fourTracksFromVCCProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    fourTracksFitter::clearRecoredSources();
    fourTracksFitter::recordParingSources( iEvent, iSetup );
    fourTracksFitter::excludeKnownParticles( iEvent, iSetup );
    // Create V0Fitter object which reconstructs the vertices and creates
    for ( const auto& fitter : myFitter )
    {
        fitter->fitAll( iEvent, iSetup );

        // save efficiency in event
        std::unique_ptr< std::vector<int> > triggerResult( new std::vector<int> );
        const std::vector<int>&  trigRes = fitter->getCutResList();
        triggerResult->reserve( trigRes.size() );
        std::copy( trigRes.begin(), trigRes.end(), std::back_inserter(*triggerResult) );
        
        char efficiencyName[60];
        sprintf( efficiencyName, "fourTkVertexingEfficiencyBoolInt%s", fitter->getFourTkCandName().c_str() );
        iEvent.put( std::move(triggerResult), efficiencyName );
    }

    for ( const auto& fittingRes : myFitter )
    {

        // Create shared_ptr for each collection to be stored in the Event
        std::unique_ptr< reco::VertexCompositeCandidateCollection >
            fourTksCandList( new reco::VertexCompositeCandidateCollection );
        if ( fittingRes->nothingToWritten() )
        {
            iEvent.put( std::move(fourTksCandList), fittingRes->getFourTkCandName() );
            continue;
        }
        const reco::VertexCompositeCandidateCollection& fourTksCandCollection = fittingRes->getFourTkCands();
        fourTksCandList->reserve( fourTksCandCollection.size() );
        std::copy( fourTksCandCollection.begin(), fourTksCandCollection.end(), std::back_inserter(*fourTksCandList) );

        // Write the collections to the Event
        iEvent.put( std::move(fourTksCandList), fittingRes->getFourTkCandName() );
    }


    fourTracksFitter::clearEvent();
    return;
}


void fourTracksFromVCCProducer::beginJob()
{
}


void fourTracksFromVCCProducer::endJob()
{
}

void fourTracksFromVCCProducer::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("beamspotLabel", edm::InputTag(""));
    desc.add<edm::InputTag>("tktkCandLabel", edm::InputTag(""));
    desc.add<edm::InputTag>("mumuCandLabel", edm::InputTag(""));
    desc.add<edm::InputTag>("genMatchLabel", edm::InputTag(""));
    desc.add<bool>("useMC", false);

    edm::ParameterSetDescription dpar;
    dpar.add<int>( "fittingMethod", 0 );

    dpar.add<std::string>("candName", "");
    dpar.add<std::string>("mumuName", "");
    dpar.add<std::string>("tktkName", "");

    dpar.add<std::string>("muPosName","");
    dpar.add<std::string>("muNegName","");
    dpar.add<std::string>("tkPosName","");
    dpar.add<std::string>("tkNegName","");
    
    dpar.add<double     >("muPosMass", -1.);
    dpar.add<double     >("muNegMass", -1.);
    dpar.add<double     >("tkPosMass", -1.);
    dpar.add<double     >("tkNegMass", -1.);

    dpar.add<double     >("muPosSigma", -1.);
    dpar.add<double     >("muNegSigma", -1.);
    dpar.add<double     >("tkPosSigma", -1.);
    dpar.add<double     >("tkNegSigma", -1.);

    dpar.add<double     >("FDSigPreCut_tktkTomumu", -1.);
    dpar.add<double     >("Cosa2dPreCut_tktkTomumu",-1.);
    dpar.add<double     >("FDSigCut_mumutktkToBS",  -1.);
    dpar.add<double     >("vtxprobCut",             -1.);
    dpar.add<double     >("mCandMassCut",           -1.);
    dpar.add<double     >("MCandMassCut",         9999.);
    dpar.add<double     >("mumuMassConstraint",     -1.);

    dpar.add<double     >("PtPreCut_muPos", -1.);
    dpar.add<double     >("PtPreCut_muNeg", -1.);
    dpar.add<double     >("PtPreCut_tkPos", -1.);
    dpar.add<double     >("PtPreCut_tkNeg", -1.);
    dpar.add<double     >("mMassPreCut_mumu", -1.);
    dpar.add<double     >("MMassPreCut_mumu", 9999.);
    dpar.add<double     >("mMassPreCut_tktk", -1.);
    dpar.add<double     >("MMassPreCut_tktk", 9999.);


    std::vector<edm::ParameterSet> dpars;
    desc.addVPSet("recoOptions", dpar, dpars);
    descriptions.add("fourTracksFromVCCProducer", desc);
    return;
}

//define this as a plug-in
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_FWK_MODULE(fourTracksFromVCCProducer);
