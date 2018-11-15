// -*- C++ -*-
//
// Package:    V0Producer
// Class:      V0Producer
//
/**\class V0Producer V0Producer.h RecoVertex/V0Producer/interface/V0Producer.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Brian Drell
//         Created:  Fri May 18 22:57:40 CEST 2007
// $Id: V0Producer.h,v 1.7 2009/03/10 22:58:47 drell Exp $
//
//

#ifndef __compCandAnalyzer_H__
#define __compCandAnalyzer_H__

// system include files
#include <memory>

// user include files

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "vertexProducer/vertexProducer/interface/ccLoader.h"

class compCandAnalyzer : public edm::EDAnalyzer
{
public:
    explicit compCandAnalyzer(const edm::ParameterSet&);
    ~compCandAnalyzer();

    //virtual void beginJob() ;
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() ;

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );
private:
    edm::Service<TFileService> fs;

    unsigned nParticles;

    ccLoader** loaders;
    TFileDirectory** dirs;

};

#endif
