#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("myVertexingProcedure")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30) )
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")


process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(
#"root://se01.grid.nchc.org.tw//store/user/ltsai/MC/step1/LambdaBToJPsipK/testGenerateLb_step1/180912_014033/0000/LbToJPsiPK_GENSIM_pythia8_1.root"
#'root://se01.grid.nchc.org.tw///cms/store/user/ltsai/crabtest/MC/step1/LambdaBToPcK/MinBias/mc_LbToPcK_13TeV_2016Generated/190214_100145/0000/myTest_python_1.root'
'file:///home/ltsai/ReceivedFile/step3.root'

#"file:///home/ltsai/Data/CMSSW_9_4_0/default/0294A5EC-3CED-E711-97F1-001E677925F0.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/0E6098A1-44ED-E711-AF56-A4BF011257F8.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/2C5196D0-44ED-E711-BEFE-A4BF0112BC8C.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/344E8E0F-45ED-E711-8C1A-001E67E6F922.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/3A6A42D5-44ED-E711-ABE2-A4BF0112BC8C.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/44F7A3AF-44ED-E711-8BCB-A4BF01125660.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/462D37C6-44ED-E711-BB4A-A4BF01158888.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/6C22CCAC-44ED-E711-8569-A4BF01125660.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/82AA92DC-3CED-E711-B076-A4BF0112BC8C.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/86F445C4-44ED-E711-A839-001E677928AA.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/A28CD840-45ED-E711-BDF0-001E67E6A166.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/B620BAD0-44ED-E711-BFC7-001E67E33C10.root",
#"file:///home/ltsai/Data/CMSSW_9_4_0/default/C6161A9D-44ED-E711-AFFC-001E67792890.root",
),
        duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v1', '')

HLTName='HLT_DoubleMu4_JpsiTrk_Displaced_v*'
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(HLTName))

# preselect pat muon and pat tracks
process.load('vertexProducer.PreselectFilter.FilterConf9_cfi')

# remove MC dependence
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, names=['All'], outputModules=[] )

process.load('vertexProducer.vertexProducer.tktkVertexingProducer_cfi')
process.load('vertexProducer.vertexProducer.mumuVertexingProducer_cfi')

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('recoBPHanalysis_withFilter_Data.root'),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_selectedMuons_MuonPreselectionBoolInt_myVertexingProcedure",
        "keep *_selectedTracks_TrackPreselectionBoolInt_myVertexingProcedure",
        "keep *_mumuVertexingProducer_*_myVertexingProcedure",
        "keep *_tktkVertexingProducer_*_myVertexingProcedure",
        "keep *_generalV0Candidates_*_RECO",
        "keep *_offlineBeamSpot_*_RECO",
        "keep *_offlinePrimaryVertices_*_RECO",
        "keep *_genParticles__HLT",
        "keep *_TriggerResults__HLT",
    ),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('myfilterpath') )
)
process.myfilterpath = cms.Path(
    # process.hltHighLevel
      process.myMuonsSequence
    * process.myTrackSequence
    * process.tktkVertexingProducer
    * process.mumuVertexingProducer
)


process.e = cms.EndPath(process.out)
