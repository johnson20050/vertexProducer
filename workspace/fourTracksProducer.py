#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("myFourTracksVertexingProcedure")

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
    'file:recoBPHanalysis_withFilter.root'
),
        duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v1', '')

process.load('vertexProducer.vertexProducer.fourTracksFromVCCProducer_cfi')

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('reco_fourTracksVertexing.root'),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_mumuVertexingProducer_*_myVertexingProcedure",
        "keep *_tktkVertexingProducer_*_myVertexingProcedure",
        "keep *_*_*_myFourTracksVertexingProcedure",
        "keep *_generalV0Candidates_*_RECO",
        "keep *_offlineBeamSpot_*_RECO",
        "keep *_offlinePrimaryVertices_*_RECO",
        "keep *_genParticles__HLT",
        "keep *_TriggerResults__HLT",
    ),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('myfilterpath') )
)
process.myfilterpath = cms.Path(
        process.fourTracksFromVCCProducer
)


process.e = cms.EndPath(process.out)
