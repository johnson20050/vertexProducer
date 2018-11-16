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

#include "vertexProducer/vertexProducer/plugins/compCandAnalyzer.h"

// Constructor
compCandAnalyzer::compCandAnalyzer(const edm::ParameterSet& iConfig)
{

    // Registering V0 Collections
    const std::vector< edm::ParameterSet >& subConfigs
        = iConfig.getParameter < std::vector < edm::ParameterSet >> ("recoOptions");
    nParticles = subConfigs.size();
    loaders = new ccLoader* [nParticles];
    dirs.reserve( nParticles );

    for ( unsigned idx = 0; idx < nParticles; ++idx )
    {
        std::string  name = subConfigs[idx].getParameter<std::string>("candName");
        loaders[idx] = new ccLoader( subConfigs[idx], consumesCollector() );
        TFileDirectory tmpDir = fs->mkdir(subConfigs[idx].getParameter<std::string>("candName"));
        tmpDir.make<TH1F>("dummyHist","", 10, 0., 10.);
        dirs.push_back( tmpDir );

        loaders[idx]->setOutput( &dirs[idx] );
        loaders[idx]->createHistos();
    }
    return;
}

// (Empty) Destructor
compCandAnalyzer::~compCandAnalyzer()
{
    for ( unsigned idx = 0; idx < nParticles; ++idx )
    {
        delete loaders[idx];
    }

    delete loaders;
}

void compCandAnalyzer::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
    edm::ParameterSetDescription desc;
    edm::ParameterSetDescription dpar;
    dpar.add<std::string>( "beamspotLabel", "" );
    dpar.add<std::string>( "patLabel", "" );
    dpar.add<std::string>( "vccLabel", "" );
    dpar.add<std::string>( "candName", "" );

    std::vector<edm::ParameterSet> dpars;
    desc.addVPSet( "recoOptions", dpar, dpars );
    descriptions.add( "compCandAnalyzer", desc );
    return; 
}


//
// Methods
//

// Producer Method
void compCandAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    // Create V0Fitter object which reconstructs the vertices and creates
    for ( unsigned idx = 0; idx < nParticles; ++idx )
    {
        loaders[idx]->calc( iEvent, iSetup );
    }

    return;
}


void compCandAnalyzer::beginJob()
{
}


void compCandAnalyzer::endJob()
{
}

//define this as a plug-in
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_FWK_MODULE(compCandAnalyzer);
