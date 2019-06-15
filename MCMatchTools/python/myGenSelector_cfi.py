import FWCore.ParameterSet.Config as cms
# mcChannels :
#   0 = LbL0
#   1 = LbToJpsipK
#   2 = LbToPcK
#   3 = BsGeneral
#   4 = BdGeneral

myGenSelector = cms.EDFilter(
        "GenEventSelector",
        mcmatchsrc = cms.InputTag('genParticles::HLT'),
        targetPIDs = cms.vint32(5122, -5122),
        mcChannels = cms.int32(1),
        )
