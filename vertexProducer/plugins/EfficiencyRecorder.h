// -*- C++ -*-
// file to read efficiency tag stored in int.
// and output to histogram

#ifndef __EfficiencyRecorder_H__
#define __EfficiencyRecorder_H__

// system include files
#include <memory>

// user include files

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TH1I.h>

class EfficiencyRecorder : public edm::EDAnalyzer
{
public:
	explicit EfficiencyRecorder(const edm::ParameterSet&);
	~EfficiencyRecorder();

	virtual void beginJob() override;
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() override;

	static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

private:
	edm::Service<TFileService> fs;
	std::vector<TFileDirectory> dirs;

    edm::EDGetTokenT<std::vector<int>> tkPSelectEffToken;
    edm::EDGetTokenT<std::vector<int>> muPSelectEffToken;
    edm::EDGetTokenT<std::vector<int>> tksVertexEffToken;
    edm::EDGetTokenT<std::vector<int>> musVertexEffToken;
    edm::EDGetTokenT<int>              totFourTkEffToken;
	edm::EDGetTokenT<std::vector<int>> pL0BCandsEffToken;
	edm::EDGetTokenT<std::vector<int>> nL0BCandsEffToken;
	edm::EDGetTokenT<std::vector<int>> LbL0CandsEffToken;
	edm::EDGetTokenT<std::vector<int>> LbLoCandsEffToken;

    bool usetkPSelectEff;
    bool usemuPSelectEff;
    bool usetksVertexEff;
    bool usemusVertexEff;
    bool usetotFourTkEff;
	bool usepL0BCandsEff;
	bool usenL0BCandsEff;
	bool useLbL0CandsEff;
	bool useLbLoCandsEff;


    TH1I* effTk;
    TH1I* effMu;
    TH1I* effMuMuVtx;
    TH1I* effTkTkVtx;
    TH1I* effFourTkVtx;
    TH1I* effpL0BVtx;
    TH1I* effnL0BVtx;
    TH1I* effLbL0Vtx;
    TH1I* effLbLoVtx;

    TH1I* effTkMatched;
    TH1I* effMuMatched;
    TH1I* effMuMuVtxMatched;
    TH1I* effTkTkVtxMatched;
    TH1I* effFourTkVtxMatched;
    TH1I* effpL0BVtxMatched;
    TH1I* effnL0BVtxMatched;
    TH1I* effLbL0VtxMatched;
    TH1I* effLbLoVtxMatched;
    bool cutStatus(int recorder, int cutIdx) const { return (recorder>>cutIdx)%2; }
};

#endif
