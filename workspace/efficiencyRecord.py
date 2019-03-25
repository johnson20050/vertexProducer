#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("hiii")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30) )
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)

fileList=cms.untracked.vstring()
fileList.extend( [
"file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/noPreselection_vertexed/reco_fourTracksVertexing_0.root",
#'file:reco_fourTracksVertexing_1.root',
#'file:reco_fourTracksVertexing_2.root',
#'file:reco_fourTracksVertexing_3.root',
#'file:reco_fourTracksVertexing_4.root',
#'file:reco_fourTracksVertexing_5.root',
#'file:reco_fourTracksVertexing_6.root',
#'file:reco_fourTracksVertexing_7.root',
#'file:reco_fourTracksVertexing_8.root',
#'file:reco_fourTracksVertexing_9.root',
] )
process.source = cms.Source("PoolSource",fileNames = fileList, duplicateCheckMode = cms.untracked.string('noDuplicateCheck'))

# preselect pat muon and pat tracks
process.load('vertexProducer.vertexProducer.efficiencyRecorder_cfi')


process.TFileService = cms.Service("TFileService",
        fileName = cms.string('effResult.root')
        )
process.path = cms.Path(
        process.efficiencyRecordAnalyzer
        )


