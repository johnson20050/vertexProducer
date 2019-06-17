import FWCore.ParameterSet.Config as cms
# mcChannels :
#   channelLbL0=0
#   channelLbToJPsipK=1
#   channelLbToPcK=2,

#   BsGeneral=3
#   BsToJPsiKK=4
#   BsToJPsiPhi=5

#   BdGeneral=6
#   BdToJPsiKpi=7
#   BdToJPsiKstar1430=8
#   BdToJPsiKstar892=9


myGenSelector = cms.EDFilter(
        "GenEventSelector",
        mcmatchsrc = cms.InputTag('genParticles::HLT'),
        targetPIDs = cms.vint32(5122, -5122),
        mcChannels = cms.int32(1),
        )
