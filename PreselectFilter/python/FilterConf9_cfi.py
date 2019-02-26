import FWCore.ParameterSet.Config as cms


#--------------------------------------------------------------------------------
# - Muon  Setting
#--------------------------------------------------------------------------------
# prepare pat::Muon from AOD
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff import *
from PhysicsTools.PatAlgos.cleaningLayer1.cleanPatCandidates_cff import *


selectedPatMuons.cut = cms.string('muonID(\"TMOneStationTight\")'
    ' && abs(innerTrack.dxy) < 0.3'
    ' && abs(innerTrack.dz)  < 20.'
    ' && innerTrack.hitPattern.trackerLayersWithMeasurement > 5'
    ' && innerTrack.hitPattern.pixelLayersWithMeasurement > 0'
    ' && innerTrack.quality(\"highPurity\")'
)

selectedMuons = cms.EDFilter(
    "MuonProducer",
    muonsrc = cms.InputTag("selectedPatMuons"),
    mcmatchsrc = cms.InputTag('genParticles::HLT'),
    useMC = cms.bool(False),
    )

myMuonsSequence = cms.Sequence( selectedPatMuons * selectedMuons, makePatMuonsTask )


#--------------------------------------------------------------------------------
# - Track Setting
#--------------------------------------------------------------------------------


# preselect track
selectedTracks = cms.EDFilter(
    "TrackProducer",
    bssrc = cms.InputTag('offlineBeamSpot::RECO'),
    tracksrc = cms.InputTag("generalTracks::RECO"),
    muonsrc = cms.InputTag("selectedPatMuons"),
    mcmatchsrc = cms.InputTag('genParticles::HLT'),
    useMC = cms.bool(False),

    # cuts :
    # Select tracks using TrackBase::TrackQuality.
    # Select ALL tracks by leaving this vstring empty, which
    #   is equivalent to using 'loose'
    #trackQualities = cms.vstring('highPurity', 'goodIterative'),
    trackQualities = cms.vstring('loose'),

    # The next parameters are cut values.
    # All distances are in cm, all energies in GeV, as usual.

    # --Track quality/compatibility cuts--
    #   Normalized track Chi2 <
    tkChi2Cut = cms.double(5.0),
    #   Number of valid hits on track >=
    tkNhitsCut = cms.int32(6),
    #   Track impact parameter significance >
    impactParameterSigCut = cms.double(2.),
    )

myTrackSequence = cms.Sequence( selectedTracks )
