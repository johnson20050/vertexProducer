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
        cms.untracked.vstring(
#'file:recoBPHanalysis_withFilter.root'
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-1.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-2.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-3.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-4.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-5.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-6.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-7.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-8.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1-9.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_1.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_10.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_11.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_12.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_13.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_14.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_15.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_16.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_17.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_18.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_19.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_2.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_20.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_21.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_22.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_23.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_24.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_25.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_26.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_27.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_28.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_3.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_4.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_5.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_6.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_7.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_8.root',
'file:///home/ltsai/Data/CRABdata/CRABdata_vertexProducer_2016RunF_30_11_2018/recoBPHanalysis_withFilter_9.root',
),
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


