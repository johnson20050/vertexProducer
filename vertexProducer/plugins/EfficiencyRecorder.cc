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

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "vertexProducer/vertexProducer/plugins/EfficiencyRecorder.h"
#include "TMath.h"

#include <TH1.h>
#include <TFile.h>

// system include files
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>

#include <memory>
#include <string>

using namespace std;

// Constructor
EfficiencyRecorder::EfficiencyRecorder(const edm::ParameterSet& iConfig) :
    tkPSelectEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("tkPSelectEffLabel"))),
    muPSelectEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("muPSelectEffLabel"))),
    tksVertexEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("tksVertexEffLabel"))),
    musVertexEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("musVertexEffLabel"))),
    totFourTkEffToken(consumes<int             >(iConfig.getParameter<edm::InputTag>("totFourTkEffLabel"))),
	pL0BCandsEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("pL0BCandsEffLabel"))),
	nL0BCandsEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("nL0BCandsEffLabel"))),
	LbL0CandsEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("LbL0CandsEffLabel"))),
	LbLoCandsEffToken(consumes<std::vector<int>>(iConfig.getParameter<edm::InputTag>("LbLoCandsEffLabel")))
{
	usetkPSelectEff = !(iConfig.getParameter<edm::InputTag>("tkPSelectEffLabel") == edm::InputTag(""));
	usemuPSelectEff = !(iConfig.getParameter<edm::InputTag>("muPSelectEffLabel") == edm::InputTag(""));
	usetksVertexEff = !(iConfig.getParameter<edm::InputTag>("tksVertexEffLabel") == edm::InputTag(""));
	usemusVertexEff = !(iConfig.getParameter<edm::InputTag>("musVertexEffLabel") == edm::InputTag(""));
	usetotFourTkEff = !(iConfig.getParameter<edm::InputTag>("totFourTkEffLabel") == edm::InputTag(""));
	usepL0BCandsEff = !(iConfig.getParameter<edm::InputTag>("pL0BCandsEffLabel") == edm::InputTag(""));
	usenL0BCandsEff = !(iConfig.getParameter<edm::InputTag>("nL0BCandsEffLabel") == edm::InputTag(""));
	useLbL0CandsEff = !(iConfig.getParameter<edm::InputTag>("LbL0CandsEffLabel") == edm::InputTag(""));
	useLbLoCandsEff = !(iConfig.getParameter<edm::InputTag>("LbLoCandsEffLabel") == edm::InputTag(""));

    effTk       = fs->make<TH1I>("effTk"        ,"effTk",30,0,30);
    effMu       = fs->make<TH1I>("effMu"        ,"effMu",30,0,30);
    effMuMuVtx  = fs->make<TH1I>("effMuMuVtx"   ,"effMuMuVtx",30,0,30);
    effTkTkVtx  = fs->make<TH1I>("effTkTkVtx"   ,"effTkTkVtx",30,0,30);
    effFourTkVtx= fs->make<TH1I>("effFourTkVtx" ,"effFourTkVtx",30,0,30);
    effpL0BVtx  = fs->make<TH1I>("effpL0BVtx"   ,"effpL0BVtx",30,0,30);
    effnL0BVtx  = fs->make<TH1I>("effnL0BVtx"   ,"effnL0BVtx",30,0,30);
    effLbL0Vtx  = fs->make<TH1I>("effLbL0Vtx"   ,"effLbL0Vtx",30,0,30);
    effLbLoVtx  = fs->make<TH1I>("effLbLoVtx"   ,"effLbLoVtx",30,0,30);

    effTkMatched       = fs->make<TH1I>("effTkMatched"        ,"effTkMatched",30,0,30);
    effMuMatched       = fs->make<TH1I>("effMuMatched"        ,"effMuMatched",30,0,30);
    effMuMuVtxMatched  = fs->make<TH1I>("effMuMuVtxMatched"   ,"effMuMuVtxMatched",30,0,30);
    effTkTkVtxMatched  = fs->make<TH1I>("effTkTkVtxMatched"   ,"effTkTkVtxMatched",30,0,30);
    effFourTkVtxMatched= fs->make<TH1I>("effFourTkVtxMatched" ,"effFourTkVtxMatched",30,0,30);
    effpL0BVtxMatched  = fs->make<TH1I>("effpL0BVtxMatched"   ,"effpL0BVtxMatched",30,0,30);
    effnL0BVtxMatched  = fs->make<TH1I>("effnL0BVtxMatched"   ,"effnL0BVtxMatched",30,0,30);
    effLbL0VtxMatched  = fs->make<TH1I>("effLbL0VtxMatched"   ,"effLbL0VtxMatched",30,0,30);
    effLbLoVtxMatched  = fs->make<TH1I>("effLbLoVtxMatched"   ,"effLbLoVtxMatched",30,0,30);
	return;
}

// (Empty) Destructor
EfficiencyRecorder::~EfficiencyRecorder()
{
	return;
}

void EfficiencyRecorder::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
	edm::ParameterSetDescription desc;
	desc.add < edm::InputTag > ("tkPSelectEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("muPSelectEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("tksVertexEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("musVertexEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("totFourTkEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("pL0BCandsEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("nL0BCandsEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("LbL0CandsEffLabel", edm::InputTag(""));
	desc.add < edm::InputTag > ("LbLoCandsEffLabel", edm::InputTag(""));
	descriptions.add("process.EffeciencyRecorder", desc);
	return;
}

// Producer Method
void EfficiencyRecorder::analyze(const edm::Event& ev, const edm::EventSetup& es)
{
    if ( usetkPSelectEff ) // {{{
    {
        edm::Handle<std::vector<int>> tkPSelectEffHandle;
        
        ev.getByToken(tkPSelectEffToken, tkPSelectEffHandle);
        if ( tkPSelectEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = tkPSelectEffHandle->cbegin();
            std::vector<int>::const_iterator iend = tkPSelectEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effTk->Fill(idx);
                        if ( matched )
                            effTkMatched->Fill(idx);
                    }
            }
        }
    } // usetkPSelectEff end }}}

    if ( usemuPSelectEff ) // {{{
    {
        edm::Handle<std::vector<int>> muPSelectEffHandle;
        
        ev.getByToken(muPSelectEffToken, muPSelectEffHandle);
        if ( muPSelectEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = muPSelectEffHandle->cbegin();
            std::vector<int>::const_iterator iend = muPSelectEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effMu->Fill(idx);
                        if ( matched )
                            effMuMatched->Fill(idx);
                    }
            }
        }
    } // usemuPSelectEff end }}}

    if ( usetksVertexEff ) // {{{
    {
        edm::Handle<std::vector<int>> tksVertexEffHandle;
        
        ev.getByToken(tksVertexEffToken, tksVertexEffHandle);
        if ( tksVertexEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = tksVertexEffHandle->cbegin();
            std::vector<int>::const_iterator iend = tksVertexEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effTkTkVtx->Fill(idx);
                        if ( matched )
                            effTkTkVtxMatched->Fill(idx);
                    }
            }
        }
    } // usetksVertexEff end }}}

    if ( usemusVertexEff ) // {{{
    {
        edm::Handle<std::vector<int>> musVertexEffHandle;
        
        ev.getByToken(musVertexEffToken, musVertexEffHandle);
        if ( musVertexEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = musVertexEffHandle->cbegin();
            std::vector<int>::const_iterator iend = musVertexEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effMuMuVtx->Fill(idx);
                        if ( matched )
                            effMuMuVtxMatched->Fill(idx);
                    }
            }
        }
    } // usemusVertexEff end }}}

    if ( usepL0BCandsEff ) // {{{
    {
        edm::Handle<std::vector<int>> pL0BCandsEffHandle;
        
        ev.getByToken(pL0BCandsEffToken, pL0BCandsEffHandle);
        if ( pL0BCandsEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = pL0BCandsEffHandle->cbegin();
            std::vector<int>::const_iterator iend = pL0BCandsEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effpL0BVtx->Fill(idx);
                        if ( matched )
                            effpL0BVtxMatched->Fill(idx);
                    }
            }
        }
    } // usepL0BCandsEff end }}}

    if ( usenL0BCandsEff ) // {{{
    {
        edm::Handle<std::vector<int>> nL0BCandsEffHandle;
        
        ev.getByToken(nL0BCandsEffToken, nL0BCandsEffHandle);
        if ( nL0BCandsEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = nL0BCandsEffHandle->cbegin();
            std::vector<int>::const_iterator iend = nL0BCandsEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effnL0BVtx->Fill(idx);
                        if ( matched )
                            effnL0BVtxMatched->Fill(idx);
                    }
            }
        }
    } // usenL0BCandsEff end }}}

    if ( useLbL0CandsEff ) // {{{
    {
        edm::Handle<std::vector<int>> LbL0CandsEffHandle;
        
        ev.getByToken(LbL0CandsEffToken, LbL0CandsEffHandle);
        if ( LbL0CandsEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = LbL0CandsEffHandle->cbegin();
            std::vector<int>::const_iterator iend = LbL0CandsEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effLbL0Vtx->Fill(idx);
                        if ( matched )
                            effLbL0VtxMatched->Fill(idx);
                    }
            }
        }
    } // useLbL0CandsEff end }}}

    if ( useLbLoCandsEff ) // {{{
    {
        edm::Handle<std::vector<int>> LbLoCandsEffHandle;
        
        ev.getByToken(LbLoCandsEffToken, LbLoCandsEffHandle);
        if ( LbLoCandsEffHandle.isValid() )
        {
            std::vector<int>::const_iterator iter = LbLoCandsEffHandle->cbegin();
            std::vector<int>::const_iterator iend = LbLoCandsEffHandle->cend  ();
            while ( iter != iend )
            {
                bool matched = false;
                int recordVal = *iter++;
                if ( recordVal<0 )
                {
                    matched = true;
                    recordVal*= -1;
                }


                for ( int idx = 0; idx <25; ++idx )
                    if ( cutStatus(recordVal, idx) )
                    {
                        effLbLoVtx->Fill(idx);
                        if ( matched )
                            effLbLoVtxMatched->Fill(idx);
                    }
            }
        }
    } // useLbLoCandsEff end }}}

    if ( usetotFourTkEff ) // {{{
    {
        edm::Handle<int> totFourTkEffHandle;
        
        ev.getByToken(totFourTkEffToken, totFourTkEffHandle);
        if ( totFourTkEffHandle.isValid() )
        {
            int recordVal = *totFourTkEffHandle.product();
            for ( int idx = 0; idx <25; ++idx )
                if ( cutStatus(recordVal, idx) )
                {
                    effFourTkVtx->Fill(idx);
                }
        }
    } // usetotFourTkEff end }}}




	return;
}

void EfficiencyRecorder::beginJob()
{
	return;
}

void EfficiencyRecorder::endJob()
{
}

//define this as a plug-in
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_FWK_MODULE(EfficiencyRecorder);
