#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("myVertexingProcedure")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(15800*2) )
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
#    'root://cms-se0.kipt.kharkov.ua//dpm/kipt.kharkov.ua/home/cms/store/data/Run2017B/Charmonium/AOD/17Nov2017-v1/50000/D2CE2C1E-F8EB-E711-8DCA-E0071B7AC5D0.root'
    # 2016
#'file:///afs/cern.ch/user/l/ltsai/ReceivedFile/DATA/CMSSW_8_0_28/Run2016G-07Aug17-v1/7EF6DB2A-14AF-E711-B529-0025905A6070.root'

    #2017
'file:///afs/cern.ch/user/l/ltsai/ReceivedFile/DATA/CMSSW_9_4_0/default/86F445C4-44ED-E711-A839-001E677928AA.root',
'file:///afs/cern.ch/user/l/ltsai/ReceivedFile/DATA/CMSSW_9_4_0/default/B620BAD0-44ED-E711-BFC7-001E67E33C10.root',
),
        duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016LegacyRepro_v4', '')

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
## 2016 Triggers
#process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(
#    'HLT_Dimuon10_Jpsi_Barrel_v*',
#    'HLT_Dimuon16_Jpsi_v*',
#    'HLT_Dimuon20_Jpsi_v*',
#    'HLT_DoubleMu4_3_Jpsi_Displaced_v*',
#    'HLT_DoubleMu4_JpsiTrk_Displaced_v*',
#    ))
# 2017 Triggers
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(
    'HLT_Dimuon20_Jpsi_Barrel_*',
    'HLT_Dimuon25_Jpsi_v*',
    'HLT_DoubleMu4_3_Jpsi_Displaced_v*',
    'HLT_DoubleMu4_Jpsi_Displaced_v*',
    'HLT_DoubleMu4_JpsiTrk_Displaced_v*',
    #'HLT_DoubleMu4_JpsiTrkTrk_Displaced_v*',
    ))

# preselect pat muon and pat tracks
process.load('vertexProducer.PreselectFilter.FilterConf9_cfi')

# remove MC dependence
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, names=['All'], outputModules=[] )

process.load('vertexProducer.vertexProducer.tktkVertexingProducer_cfi')
process.load('vertexProducer.vertexProducer.mumuVertexingProducer_cfi')
process.load('vertexProducer.vertexProducer.fourTracksFromVCCProducer_cfi')

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('vertexProducer_BdRemoved.root'),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_mumuVertexingProducer_*_myVertexingProcedure",
        "keep *_tktkVertexingProducer_*_myVertexingProcedure",
        "keep *_fourTracksFromVCCProducer_*_myVertexingProcedure",
        "keep *_generalV0Candidates_*_RECO",
        "keep *_offlineBeamSpot_*_RECO",
        "keep *_offlinePrimaryVertices_*_RECO",
        "keep *_genParticles__HLT",
        "keep *_TriggerResults__HLT",
    ),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('myfilterpath') )
)
process.myfilterpath = cms.Path(
      process.hltHighLevel
    * process.myMuonsSequence
    * process.myTrackSequence
    * process.tktkVertexingProducer
    * process.mumuVertexingProducer
    * process.fourTracksFromVCCProducer
)


process.e = cms.EndPath(process.out)

