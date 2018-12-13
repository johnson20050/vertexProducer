import FWCore.ParameterSet.Config as cms


recoOptions = cms.VPSet(
        #cms.PSet(
        #    vccLabel = cms.string('fourTracksFromVCCProducer:testLbL0:myVertexingProcedure'),
        #   #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
        #    beamspotLabel = cms.string('offlineBeamSpot::RECO'),
        #    candName = cms.string('LambdaFromV0'),
        #    MassFrameMin = cms.double(5.4),
        #    MassFrameMax = cms.double(5.8),
        #    ),
        #cms.PSet(
        #    vccLabel = cms.string('generalV0Candidates:Lambda:RECO'),
        #   #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
        #    beamspotLabel = cms.string('offlineBeamSpot::RECO'),
        #    candName = cms.string('LambdaFromV0'),
        #    MassFrameMin = cms.double(1.0),
        #    MassFrameMax = cms.double(1.3),
        #    ),
        #cms.PSet(
        #    vccLabel = cms.string('mumuVertexingProducer:JPsi:myVertexingProcedure'),
        #   #patLabel = cms.string('lbWriteSpecificDecay:LbTkFitted:bphAnalysis'),
        #    beamspotLabel = cms.string('offlineBeamSpot::RECO'),
        #    candName = cms.string('JPsiFromMuMu'),
        #    MassFrameMin = cms.double(2.9),
        #    MassFrameMax = cms.double(3.2),
        #    ),
        cms.PSet(
            vccLabel = cms.string('fourTracksFromVCCProducer:LbL0:myVertexingProcedure'),
           #patlabel = cms.string('lbwritespecificdecay:lbtkfitted:bphanalysis'),
            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
            candName = cms.string('LbL0'),
            MassFrameMin = cms.double(5.4),
            MassFrameMax = cms.double(5.8),
            ),
#        cms.PSet(
#            vccLabel = cms.string('fourTracksFromVCCProducer:LbLo:myVertexingProcedure'),
#           #patlabel = cms.string('lbwritespecificdecay:lbtkfitted:bphanalysis'),
#            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
#            candName = cms.string('LbLo'),
#            MassFrameMin = cms.double(5.4),
#            MassFrameMax = cms.double(5.8),
#            ),
#        cms.PSet(
#            vccLabel = cms.string('fourTracksFromVCCProducer:pL0B:myVertexingProcedure'),
#           #patlabel = cms.string('lbwritespecificdecay:lbtkfitted:bphanalysis'),
#            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
#            candName = cms.string('pL0B'),
#            MassFrameMin = cms.double(5.4),
#            MassFrameMax = cms.double(5.8),
#            ),
#        cms.PSet(
#            vccLabel = cms.string('fourTracksFromVCCProducer:nL0B:myVertexingProcedure'),
#           #patlabel = cms.string('lbwritespecificdecay:lbtkfitted:bphanalysis'),
#            beamspotLabel = cms.string('offlineBeamSpot::RECO'),
#            candName = cms.string('nL0B'),
#            MassFrameMin = cms.double(5.4),
#            MassFrameMax = cms.double(5.8),
#            ),
        )



