#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

useMC = True
process = cms.Process("myVertexingProcedure")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30) )
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


process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)

fileList=cms.untracked.vstring()
fileList.extend([
    'file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3/step3_0.root',
    'file:///home/ltsai/Data/mcStep3_LbToPcK_13TeV_withoutPileUp_190219/step3/step3_1.root',
])
process.source = cms.Source("PoolSource",fileNames=fileList, duplicateCheckMode = cms.untracked.string('noDuplicateCheck'))
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

# preselect pat muon and pat tracks
process.load('vertexProducer.PreselectFilter.FilterConf9_cfi')


process.load('vertexProducer.vertexProducer.TESTtktkVertexingProducer_cfi')
process.load('vertexProducer.vertexProducer.TESTmumuVertexingProducer_cfi')
process.load('vertexProducer.vertexProducer.TESTfourTracksFromVCCProducer_cfi')

if useMC:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
    process.selectedMuons.useMC = cms.bool(True)
    process.selectedTracks.useMC = cms.bool(True)
    process.mumuVertexingProducer.useMC = cms.bool(True)
    process.tktkVertexingProducer.useMC = cms.bool(True)
    process.fourTracksFromVCCProducer.useMC = cms.bool(True)

    # only choose event with LambdaB (+-5122)
    process.load('vertexProducer.MCMatchTools.myGenSelector_cfi')
    process.myGenSelector.targetPIDs = cms.vint32(5122) # throw out anti particle
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v1', '')
# remove MC dependence
    from PhysicsTools.PatAlgos.tools.coreTools import *
    removeMCMatching(process, names=['All'], outputModules=[] )

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('reco_myTESTfourTracksVertexingMC.root'),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_mumuVertexingProducer_*_myVertexingProcedure",
        "keep *_tktkVertexingProducer_*_myVertexingProcedure",
        "keep *_fourTracksFromVCCProducer_*_myVertexingProcedure",
        "keep *_selectedMuons_MuonPreselection*_myVertexingProcedure",
        "keep *_selectedTracks_TrackPreselection*_myVertexingProcedure",
        "keep *_generalV0Candidates_*_RECO",
        "keep *_offlineBeamSpot_*_RECO",
        "keep *_offlinePrimaryVertices_*_RECO",
        "keep *_genParticles__HLT",
        "keep *_TriggerResults__HLT",
    ),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('myfilterpath') )
)
process.myfilterpath = cms.Path(
      process.myGenSelector
    * process.myMuonsSequence
    * process.myTrackSequence
    * process.tktkVertexingProducer
    * process.mumuVertexingProducer
    * process.fourTracksFromVCCProducer
)


process.e = cms.EndPath(process.out)
