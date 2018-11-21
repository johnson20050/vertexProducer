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

#ifndef __fourTracksFromVCCProducer_H__
#define __fourTracksFromVCCProducer_H__

// system include files
#include <memory>
#include <vector>

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

#include "vertexProducer/vertexProducer/interface/fourTracksFitter.h"

class fourTracksFromVCCProducer : public edm::EDProducer
{
public:
    explicit fourTracksFromVCCProducer(const edm::ParameterSet&);
    ~fourTracksFromVCCProducer();

    //virtual void beginJob() ;
    virtual void beginJob() override;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() override;
    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

private:
    std::vector<fourTracksFitter*> myFitter;
};

#endif
