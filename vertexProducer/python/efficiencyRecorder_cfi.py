import FWCore.ParameterSet.Config as cms

efficiencyRecordAnalyzer = cms.EDAnalyzer("EfficiencyRecorder",
    tkPSelectEffLabel = cms.InputTag('selectedTracks:TrackPreselectionEfficiencyBoolInt:myVertexingProcedure'),
    muPSelectEffLabel = cms.InputTag('selectedMuons:MuonPreselectionEfficiencyBoolInt:myVertexingProcedure'),
    tksVertexEffLabel = cms.InputTag('tktkVertexingProducer:tktkVertexingEfficiencyBoolInt:myVertexingProcedure'),
    musVertexEffLabel = cms.InputTag('mumuVertexingProducer:mumuVertexingEfficiencyBoolInt:myVertexingProcedure'),
    totFourTkEffLabel = cms.InputTag('fourTracksFromVCCProducer:fourTracksTotallyVertexingEfficiency:myVertexingProcedure'),
	pL0BCandsEffLabel = cms.InputTag('fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntpL0B:myVertexingProcedure'),
	nL0BCandsEffLabel = cms.InputTag('fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntnL0B:myVertexingProcedure'),
	LbL0CandsEffLabel = cms.InputTag('fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntLbL0:myVertexingProcedure'),
	LbLoCandsEffLabel = cms.InputTag('fourTracksFromVCCProducer:fourTkVertexingEfficiencyBoolIntLbLo:myVertexingProcedure'),
)

