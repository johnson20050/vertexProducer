#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("ccLoader")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",fileNames =
        cms.untracked.vstring('file:recoBPHanalysis_withFilter.root'),
        duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.TFileService = cms.Service('TFileService',
  fileName = cms.string('histos_compcandAnalyzer.root'),
  closeFileFast = cms.untracked.bool(True)
)

from vertexProducer.vertexProducer.compCandAnalyzerConfigurations_cfi import recoOptions
process.compCandAnalyzer = cms.EDAnalyzer('compCandAnalyzer',
        recoOptions = cms.VPSet( recoOptions )
)

process.p = cms.Path(
        process.compCandAnalyzer
)


