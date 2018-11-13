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

#include "vertexProducer/vertexProducer/plugins/tktkVertexingProducer.h"

// Constructor
tktkVertexingProducer::tktkVertexingProducer(const edm::ParameterSet& iConfig) :
    tktkFitting( iConfig, consumesCollector() )
{

    // Registering V0 Collections
    const std::vector< edm::ParameterSet >& subConfigs
        = iConfig.getParameter < std::vector < edm::ParameterSet >> ("recoOptions");
    for ( const auto& subConfig : subConfigs )
        produces< reco::VertexCompositeCandidateCollection >( subConfig.getParameter<std::string>("candName").c_str());

    return;
}

// (Empty) Destructor
tktkVertexingProducer::~tktkVertexingProducer()
{

}


//
// Methods
//

// Producer Method
void tktkVertexingProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    // Create V0Fitter object which reconstructs the vertices and creates
    tktkFitting.fitAll(iEvent, iSetup);


    for ( unsigned i = 0; i < tktkFitting.getNParticles(); ++i )
    {

        // Create shared_ptr for each collection to be stored in the Event
        //std::shared_ptr< reco::VertexCompositeCandidateCollection >
        std::unique_ptr< reco::VertexCompositeCandidateCollection >
        tktkCandList( new reco::VertexCompositeCandidateCollection );
        //if ( tktkCandCollection.size() == 0 )
        if ( tktkFitting.nothingToWritten(i) )
        {
            iEvent.put( std::move(tktkCandList), tktkFitting.getCandName(i) );
            continue;
        }

        const reco::VertexCompositeCandidateCollection& tktkCandCollection = tktkFitting.getCands(i);
        tktkCandList->reserve( tktkCandCollection.size() );
        std::copy( tktkCandCollection.begin(), tktkCandCollection.end(), std::back_inserter(*tktkCandList) );

        // Write the collections to the Event
        iEvent.put( std::move(tktkCandList), tktkFitting.getCandName(i) );
    }

    return;
}


void tktkVertexingProducer::beginJob()
{
}


void tktkVertexingProducer::endJob()
{
}

//define this as a plug-in
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_FWK_MODULE(tktkVertexingProducer);
