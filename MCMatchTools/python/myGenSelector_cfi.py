import FWCore.ParameterSet.Config as cms

myGenSelector = cms.EDFilter(
        "GenEventSelector",
        mcmatchsrc = cms.InputTag('genParticles::HLT'),
        targetPIDs = cms.vint32(5122, -5122),
        )
