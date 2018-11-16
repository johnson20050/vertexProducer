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

#ifndef __MUMUVERTEXINGPRODUCER_H__
#define __MUMUVERTEXINGPRODUCER_H__

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"

#include "vertexProducer/vertexProducer/interface/mumuFitter.h"

class mumuVertexingProducer : public edm::EDProducer
{
public:
    explicit mumuVertexingProducer(const edm::ParameterSet&);
    ~mumuVertexingProducer();

private:
    //virtual void beginJob() ;
    virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    mumuFitter mumuFitting;

};

#endif
