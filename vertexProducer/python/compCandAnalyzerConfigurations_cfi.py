import FWCore.ParameterSet.Config as cms


recoOptions = cms.VPSet(
        cms.PSet(
            vccLabel = cms.string('generalV0Candidates:Lambda:RECO'),
           #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
            candName = cms.string('LambdaFromV0'),
            ),
        cms.PSet(
            vccLabel = cms.string('mumuVertexingProducer:JPsi:myVertexingProcedure'),
           #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
            candName = cms.string('JPsiFromMuMu'),
            ),
        cms.PSet(
            vccLabel = cms.string('tktkVertexingProducer:Lam0:myVertexingProcedure'),
           #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
            candName = cms.string('LambdaFromTkTk'),
            ),
        )



