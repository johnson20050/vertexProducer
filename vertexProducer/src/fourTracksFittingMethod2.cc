#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod2.h"

#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"

#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "TMath.h"

// Method containing the algorithm for vertex reconstruction
void fourTracksFittingMethod2::fitAll(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
    clearAndInitializeContainer();

    using std::vector;
    using std::cout;
    using std::endl;
    using namespace reco;
    using namespace edm;

    // Handles for tracks, B-field, and tracker geometry
    Handle < reco::BeamSpot > theBeamSpotHandle;
    Handle < reco::VertexCompositeCandidateCollection > theMuMuPairHandle;
    Handle < reco::VertexCompositeCandidateCollection > theTkTkPairHandle;

    ESHandle < MagneticField > bFieldHandle;
    ESHandle < GlobalTrackingGeometry > globTkGeomHandle;

    // Get the tracks from the event, and get the B-field record
    //  from the EventSetup
    iEvent.getByToken(beamspotToken, theBeamSpotHandle);
    iEvent.getByToken(tktkPairToken, theTkTkPairHandle);
    iEvent.getByToken(mumuPairToken, theMuMuPairHandle);
    if (!theBeamSpotHandle.isValid()) return;
    if (!theMuMuPairHandle.isValid()) return;
    if (!theTkTkPairHandle.isValid()) return;
    iSetup.get < IdealMagneticFieldRecord > ().get(bFieldHandle);
    iSetup.get < GlobalTrackingGeometryRecord > ().get(globTkGeomHandle);
    magField = bFieldHandle.product();

    // load mumuCandidate & tktkCandidate to do the vertexing
    for ( const VertexCompositeCandidate& mumuCand : *(theMuMuPairHandle.product()) )
    //for ( int mumuIdx = 0; mumuIdx < mumuCands.size(); ++mumuIdx )
    {
        //const VertexCompositeCandidate& mumuCand = mumuCands[mumuIdx];
        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muPosName]) );
        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muNegName]) );
        
        
        for ( const VertexCompositeCandidate& tktkCand : *(theTkTkPairHandle.product()) )
        //for ( int tktkIdx = 0; tktkIdx < tktkCands.size(); ++tktkIdx )
        {
            //const reco::VertexCompositeCandidate& tktkCand = tktkCands[tktkIdx];
            const reco::RecoChargedCandidate* tkPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkPosName]) );
            const reco::RecoChargedCandidate* tkNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkNegName]) );

            // preselections
            double cosa2d = Cosa2d( tktkCand, mumuCand );
            if ( cosa2d < optD[Cosa2dPreCut_tktkTomumu] ) continue;
            // fd need to biggerr than N sigma.
            double fdSig = FDSig( tktkCand, mumuCand );
            if ( fdSig < optD[FDSigPreCut_tktkTomumu] ) continue;
            // pt need to bigger than N GeV
            if ( muPosCandPtr->pt() < optD[ptPreCut_muPos] ) continue;
            if ( muNegCandPtr->pt() < optD[ptPreCut_muNeg] ) continue;
            if ( tkPosCandPtr->pt() < optD[ptPreCut_tkPos] ) continue;
            if ( tkNegCandPtr->pt() < optD[ptPreCut_tkNeg] ) continue;

            // mass in some ranges:
            if ( mumuCand.mass() < optD[mMassPreCut_mumu] ) continue;
            if ( mumuCand.mass() > optD[MMassPreCut_mumu] ) continue;
            if ( tktkCand.mass() < optD[mMassPreCut_tktk] ) continue;
            if ( tktkCand.mass() > optD[MMassPreCut_tktk] ) continue;



            // build trackRef & TransientTracks
            reco::TrackRef muPosTkRef =  muPosCandPtr->track();
            reco::TrackRef muNegTkRef =  muNegCandPtr->track();
            reco::TrackRef tkPosTkRef =  tkPosCandPtr->track();
            reco::TrackRef tkNegTkRef =  tkNegCandPtr->track();

            reco::TransientTrack muPosTransTk( *muPosTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack muNegTransTk( *muNegTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack tkPosTransTk( *tkPosTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack tkNegTransTk( *tkNegTkRef, &(*bFieldHandle), globTkGeomHandle );

            // for check
            GlobalPoint tktkVTX ( tktkCand.vertex().x(),tktkCand.vertex().y(),  tktkCand.vertex().z() );
            GlobalPoint mumuVTX ( mumuCand.vertex().x(),mumuCand.vertex().y(),  mumuCand.vertex().z() );
            GlobalVector mpMom = muPosTransTk.trajectoryStateClosestToPoint(mumuVTX).momentum();
            GlobalVector mnMom = muNegTransTk.trajectoryStateClosestToPoint(mumuVTX).momentum();
            GlobalVector tpMom = tkPosTransTk.trajectoryStateClosestToPoint(tktkVTX).momentum();
            GlobalVector tnMom = tkNegTransTk.trajectoryStateClosestToPoint(tktkVTX).momentum();
            //printf("muPos : %f , muNeg : %f , tkPos : %f , tkNeg : %f \n", mpMom.mag(), mnMom.mag(), tpMom.mag(), tnMom.mag() );
            if ( fabs(mpMom.mag()-tpMom.mag()) < 0.001 ) continue;
            if ( fabs(mnMom.mag()-tnMom.mag()) < 0.001 ) continue;

            KinematicParticleFactoryFromTransientTrack pFactory;
            std::vector<RefCountedKinematicParticle> mumutktkCand;
            mumutktkCand.reserve(4);
            float m1sig = optD[muPosSigma];
            float m2sig = optD[muNegSigma];
            float m3sig = optD[tkPosSigma];
            float m4sig = optD[tkNegSigma];

            // create virtual particle
            std::vector<RefCountedKinematicParticle> tktkTmpCand;
            tktkTmpCand.push_back(pFactory.particle( tkPosTransTk, optD[tkPosMass], 0., 0., m3sig) );
            tktkTmpCand.push_back(pFactory.particle( tkNegTransTk, optD[tkNegMass], 0., 0., m4sig) );
            KinematicParticleVertexFitter tktkTmpFitter;
            RefCountedKinematicTree       tktkTmpKineTree;
            tktkTmpKineTree = tktkTmpFitter.fit( tktkTmpCand );
            if ( !tktkTmpKineTree->isValid() ) continue;
            RefCountedKinematicParticle tktkTmpKineParticle = tktkTmpKineTree->currentParticle();
            RefCountedKinematicVertex   tktkTmpKineVertex   = tktkTmpKineTree->currentDecayVertex();
            VirtualKinematicParticleFactory vFactory;
            float tktkTmpChi2 = tktkTmpKineVertex->chiSquared();
            float tktkTmpNdof = tktkTmpKineVertex->degreesOfFreedom();


            mumutktkCand.push_back( pFactory.particle(muPosTransTk,optD[muPosMass], 0., 0., m1sig) );
            mumutktkCand.push_back( pFactory.particle(muNegTransTk,optD[muNegMass], 0., 0., m2sig) );
            mumutktkCand.push_back( vFactory.particle(tktkTmpKineParticle->currentState(), tktkTmpChi2, tktkTmpNdof, tktkTmpKineParticle) );

            RefCountedKinematicTree fourTracksKineTree;
            if ( optD[mumuMassConstraint] > 0. )
            {
                MultiTrackKinematicConstraint* mumuConstr = new TwoTrackMassKinematicConstraint( optD[mumuMassConstraint] );
                KinematicConstrainedVertexFitter kcvFitter;
                fourTracksKineTree = kcvFitter.fit( mumutktkCand, mumuConstr );
            }
            else
            {
                KinematicParticleVertexFitter kpvFitter;
                fourTracksKineTree = kpvFitter.fit( mumutktkCand );
            }
            if ( !fourTracksKineTree->isValid() ) continue;
            fourTracksKineTree->movePointerToTheTop();

            // start to fill in result.
            RefCountedKinematicParticle candKineParticle = fourTracksKineTree->currentParticle();
            RefCountedKinematicVertex   candKineVertex   = fourTracksKineTree->currentDecayVertex();
            if ( !candKineVertex->vertexIsValid() ) continue;

            double vtxProb = TMath::Prob( candKineVertex->chiSquared(), candKineVertex->degreesOfFreedom() );
            if ( vtxProb < optD[vtxprobCut] ) continue;

            const reco::Particle::LorentzVector candP4( 
                    candKineParticle->currentState().kinematicParameters().momentum().x(),
                    candKineParticle->currentState().kinematicParameters().momentum().y(),
                    candKineParticle->currentState().kinematicParameters().momentum().z(),
                    candKineParticle->currentState().kinematicParameters().energy() );
            reco::Vertex theVtx = *(candKineVertex.get());
            Particle::Point vtx(theVtx.x(), theVtx.y(), theVtx.z());
            const Vertex::CovarianceMatrix vtxCov(theVtx.covariance());
            double vtxChi2(theVtx.chi2());
            double vtxNdof(theVtx.ndof());

            reco::VertexCompositeCandidate* fourTkCand =
                new VertexCompositeCandidate(0, candP4, vtx, vtxCov, vtxChi2, vtxNdof);
            if ( FDSig( *fourTkCand, *theBeamSpotHandle.product() ) < optD[FDSigCut_mumutktkToBS] )
            { delete fourTkCand; continue; }


            // start to get daughter information.
            fourTracksKineTree->movePointerToTheTop();
            fourTracksKineTree->movePointerToTheFirstChild();
            RefCountedKinematicParticle pMuMomRefitted = fourTracksKineTree->currentParticle();
            reco::RecoChargedCandidate pMuCand( 1, reco::Particle::LorentzVector(
                        pMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        pMuMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            pMuCand.setTrack( muPosTkRef );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle nMuMomRefitted = fourTracksKineTree->currentParticle();
            reco::RecoChargedCandidate nMuCand(-1, reco::Particle::LorentzVector(
                        nMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        nMuMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            nMuCand.setTrack( muNegTkRef );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle TkTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::RecoChargedCandidate TkTkCand( 0, reco::Particle::LorentzVector(
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        TkTkMomRefitted->currentState().kinematicParameters().energy() ), vtx );

            AddFourMomenta addp4;

            fourTkCand->addDaughter( pMuCand, optS[muPosName] );
            fourTkCand->addDaughter( nMuCand, optS[muNegName] );
            fourTkCand->addDaughter( TkTkCand,"Lam0" );
            fourTkCand->setPdgId(5122);
            addp4.set(*fourTkCand);


            if ( fourTkCand->mass() > optD[mCandMassCut] &&
                 fourTkCand->mass() < optD[MCandMassCut] )
                tmpContainerToTkTkCands[nCandsSize++] = *fourTkCand;
            if ( nCandsSize == tmpContainerSize )
                enlargeContainer();
            delete fourTkCand;
            fourTkCand = nullptr;
        } // tktkPair loop end
    } // mumuPair loop end

    fillInContainer();
    return;
}

