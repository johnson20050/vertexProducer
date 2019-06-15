import FWCore.ParameterSet.Config as cms

tktkVertexingProducer = cms.EDProducer("tktkVertexingProducer",

    # InputTag that tells which TrackCollection to use for vertexing
    selTrackLabel = cms.InputTag('selectedTracks'),
    #selTrackLabel = cms.InputTag('generalTracks'),
    beamspotLabel = cms.InputTag('offlineBeamSpot::RECO'),
    #beamspotLabel = cms.InputTag('offlineBeamSpot'),
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
    #vtxChi2Cut = cms.double(7.0),
    vtxChi2Cut = cms.double(15.0), # try a loose cut because p k are not needed to form a good vertex
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
            pTkName = cms.string('PiPos'),
            nTkName = cms.string('PiNeg'),
            candName = cms.string('generalTkPair'),
            pTkMass = cms.double(0.13957018),
            nTkMass = cms.double(0.13957018),
            tktkMass = cms.double(-1.),
            tktkMassCut = cms.double(3.5),
            #   Radial vertex significance >
            vtxSignificance2DCut = cms.double(-1.),
            ),
        #  cms.PSet(
        #      pTkName = cms.string('Proton'),
        #      nTkName = cms.string('Kaon'),
        #      candName = cms.string('TkTk'),
        #      pTkMass = cms.double(0.938272046),
        #      nTkMass = cms.double(0.493667),
        #      tktkMass = cms.double(-1.),
        #      tktkMassCut = cms.double(3.5),
        #      #   Radial vertex significance >
        #      vtxSignificance2DCut = cms.double(2.9),
        #      ),
        #  cms.PSet(
        #      pTkName = cms.string('Kaon'),
        #      nTkName = cms.string('Proton'),
        #      candName = cms.string('tktk'),
        #      pTkMass = cms.double(0.493667),
        #      nTkMass = cms.double(0.938272046),
        #      tktkMass = cms.double(-1.),
        #      tktkMassCut = cms.double(3.5),
        #      #   Radial vertex significance >
        #      vtxSignificance2DCut = cms.double(2.9),
        #      ),
        #  cms.PSet(
        #      pTkName = cms.string('Proton'),
        #      nTkName = cms.string('Pion'),
        #      candName = cms.string('Lam0'),
        #      pTkMass = cms.double(0.938272046),
        #      nTkMass = cms.double(0.13957018),
        #      tktkMass = cms.double(1.115683),
        #      tktkMassCut = cms.double(0.5),
        #      #   Radial vertex significance >
        #      vtxSignificance2DCut = cms.double(15.),
        #      ),
        #  cms.PSet(
        #      pTkName = cms.string('Pion'),
        #      nTkName = cms.string('Proton'),
        #      candName = cms.string('Lamo'),
        #      pTkMass = cms.double(0.13957018),
        #      nTkMass = cms.double(0.938272046),
        #      tktkMass = cms.double(1.115683),
        #      tktkMassCut = cms.double(0.5),
        #      #   Radial vertex significance >
        #      vtxSignificance2DCut = cms.double(15.),
        #      ),
        ),
)


