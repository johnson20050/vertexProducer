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

#include "vertexProducer/vertexProducer/plugins/mumuVertexingProducer.h"

// Constructor
mumuVertexingProducer::mumuVertexingProducer(const edm::ParameterSet& iConfig) :
    mumuFitting( iConfig, consumesCollector() )
{
    // Registering V0 Collections
    const std::vector< edm::ParameterSet >& subConfigs
        = iConfig.getParameter < std::vector < edm::ParameterSet >> ("recoOptions");
    for ( const auto& subConfig : subConfigs )
        produces< reco::VertexCompositeCandidateCollection >( subConfig.getParameter<std::string>("candName").c_str());
    produces< std::vector<int> >( "mumuVertexingEfficiencyBoolInt" );

    return;
}

// (Empty) Destructor
mumuVertexingProducer::~mumuVertexingProducer()
{

}


//
// Methods
//

// Producer Method
void mumuVertexingProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    // Create V0Fitter object which reconstructs the vertices and creates
    mumuFitting.fitAll( iEvent, iSetup );

    std::unique_ptr< std::vector<int> > mumuTriggerResult( new std::vector<int> );
    const std::vector<int>&  trigRes = mumuFitting.getCutResList();
    mumuTriggerResult->reserve( trigRes.size() );
    std::copy( trigRes.begin(), trigRes.end(), std::back_inserter(*mumuTriggerResult) );
    iEvent.put( std::move(mumuTriggerResult), "mumuVertexingEfficiencyBoolInt" );

    for ( unsigned i = 0; i < mumuFitting.getNParticles(); ++i )
    {

        // Create shared_ptr for each collection to be stored in the Event
        std::unique_ptr< reco::VertexCompositeCandidateCollection >
            tktkCandList( new reco::VertexCompositeCandidateCollection );
        if ( mumuFitting.nothingToWritten(i) )
        {
            iEvent.put( std::move(tktkCandList), mumuFitting.getCandName(i) );
            continue;
        }

        const reco::VertexCompositeCandidateCollection& tktkCandCollection = mumuFitting.getCands(i);
        tktkCandList->reserve( tktkCandCollection.size() );
        std::copy( tktkCandCollection.begin(), tktkCandCollection.end(), std::back_inserter(*tktkCandList) );

        // Write the collections to the Event
        iEvent.put( std::move(tktkCandList), mumuFitting.getCandName(i) );
    }

    return;
}


void mumuVertexingProducer::beginJob()
{
}


void mumuVertexingProducer::endJob()
{
}

//define this as a plug-in
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_FWK_MODULE(mumuVertexingProducer);
