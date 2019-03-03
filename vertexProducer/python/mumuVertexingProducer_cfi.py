import FWCore.ParameterSet.Config as cms

mumuVertexingProducer = cms.EDProducer("mumuVertexingProducer",

    # InputTag that tells which TrackCollection to use for vertexing
    selMuonsLabel = cms.InputTag('selectedMuons'),
    beamspotLabel = cms.InputTag('offlineBeamSpot::RECO'),
    genMatchLabel = cms.InputTag('genParticles::HLT'),
    useMC = cms.bool(False),

    # set to true, uses tracks refit by the KVF for V0Candidate kinematics
    #  NOTE: useSmoothing is automatically set to FALSE
    #  if using the AdaptiveVertexFitter (which is NOT recommended)
    useSmoothing = cms.bool(True),

    # Recommend leaving this one as is.
    vertexFitter = cms.string('KalmanVertexFitter'),

    # The next parameters are cut values.
    # All distances are in cm, all energies in GeV, as usual.

    # --Track quality/compatibility cuts--
    #   PCA distance between tracks <
    tkDCACut = cms.double(1.),

    # --V0 Vertex cuts--
    #   Vertex chi2 < 
    vtxChi2Cut = cms.double(7.0),
    #   Vertex radius cut >
    #   (UNUSED)
    rVtxCut = cms.double(0.0),
    # We check if either track has a hit inside (radially) the vertex position
    #  minus this number times the sigma of the vertex fit
    #  NOTE: Set this to -1 to disable this cut, which MUST be done
    #  if you want to run V0Producer on the AOD track collection!
    innerHitPosCut = cms.double(-1.),


    # set two tks information
    recoOptions = cms.VPSet(
        cms.PSet(
            pTkName = cms.string('MuPos'),
            nTkName = cms.string('MuNeg'),
            candName = cms.string('JPsi'),
            pTkMass = cms.double(0.1056583715),
            nTkMass = cms.double(0.1056583715),
            tktkMass = cms.double(3.096916),
            tktkMassCut = cms.double(1.0),
            #   Radial vertex significance >
            vtxSignificance2DCut = cms.double(2.9),
            ),
        ),
)
