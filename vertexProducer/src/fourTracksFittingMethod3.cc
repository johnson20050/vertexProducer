#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod3.h"

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
void fourTracksFittingMethod3::fitAll(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
    clearAndInitializeContainer();
    if ( !recorded ) return;
    const reco::VertexCompositeCandidateCollection& mumuCands = *(theMuMuPairHandlePtr->product());
    const reco::VertexCompositeCandidateCollection& tktkCands = *(theTkTkPairHandlePtr->product());
    const MagneticField* magField = bFieldHandlePtr->product();
    const edm::ESHandle<GlobalTrackingGeometry>& globTkGeomHandle = *(globTkGeomHandlePtr.get());
    if ( mumuCands.size() > 1 ) return; // lowering combinatorial background.

    int isTarget = 1;

    // load mumuCandidate & tktkCandidate to do the vertexing
    for ( unsigned mumuIdx = 0; mumuIdx != mumuCands.size(); ++mumuIdx )
    {
        const reco::VertexCompositeCandidate& mumuCand = mumuCands[mumuIdx];
        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuPos") );
        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuNeg") );
        
        
        for ( unsigned tktkIdx = 0; tktkIdx != tktkCands.size(); ++tktkIdx, cutRecordList.back()*=isTarget )
        {
            isTarget = 1;

            if ( fourTracksFitter::usedPair(mumuIdx, tktkIdx) ) continue;
            const reco::VertexCompositeCandidate& tktkCand = tktkCands[tktkIdx];
            const reco::RecoChargedCandidate* tkPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter("PiPos") );
            const reco::RecoChargedCandidate* tkNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter("PiNeg") );
            reco::Particle::LorentzVector tkPosP4 = tkPosCandPtr->p4(); tkPosP4.SetE(sqrt(tkPosP4.P()*tkPosP4.P()+optD[tkPosMass]*optD[tkPosMass]));
            reco::Particle::LorentzVector tkNegP4 = tkNegCandPtr->p4(); tkNegP4.SetE(sqrt(tkNegP4.P()*tkNegP4.P()+optD[tkNegMass]*optD[tkNegMass]));
            reco::Particle::LorentzVector tktkPreSelP4 = tkPosP4+tkNegP4;

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
            if ( tktkPreSelP4.M() < optD[mMassPreCut_tktk] ) continue;
            if ( tktkPreSelP4.M() > optD[MMassPreCut_tktk] ) continue;
            // preselections end



            // build trackRef & TransientTracks
            reco::TrackRef muPosTkRef =  muPosCandPtr->track();
            reco::TrackRef muNegTkRef =  muNegCandPtr->track();
            reco::TrackRef tkPosTkRef =  tkPosCandPtr->track();
            reco::TrackRef tkNegTkRef =  tkNegCandPtr->track();

            reco::TransientTrack muPosTransTk( *muPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack muNegTransTk( *muNegTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkPosTransTk( *tkPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkNegTransTk( *tkNegTkRef, magField, globTkGeomHandle );


            cutRecordList.push_back(0);
            int& cutRecord = cutRecordList.back();
            cutRecord += 1 << 1;
			// preselections end
            if ( useMC )
                if ( theGenMatchHandlePtr->isValid() )
                    if ( IsTargetJPsi(muPosTransTk.track(), muNegTransTk.track(), *(theGenMatchHandlePtr->product())) )
                        if ( IsTargetCand(tkPosTransTk.track(), tkNegTransTk.track(), *(theGenMatchHandlePtr->product())) )
                        isTarget = -1;

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
            cutRecord += 1 << 3;

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
            cutRecord += 1 << 9;
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
            cutRecord += 1 << 4;
            fourTracksKineTree->movePointerToTheTop();

            // start to fill in result.
            RefCountedKinematicParticle candKineParticle = fourTracksKineTree->currentParticle();
            RefCountedKinematicVertex   candKineVertex   = fourTracksKineTree->currentDecayVertex();
            if ( !candKineVertex->vertexIsValid() ) continue;
            cutRecord += 1 << 5;

            double vtxProb = TMath::Prob( candKineVertex->chiSquared(), candKineVertex->degreesOfFreedom() );
            if ( vtxProb < optD[vtxprobCut] ) continue;
            cutRecord += 1 << 6;

            const reco::Particle::LorentzVector candP4( 
                    candKineParticle->currentState().kinematicParameters().momentum().x(),
                    candKineParticle->currentState().kinematicParameters().momentum().y(),
                    candKineParticle->currentState().kinematicParameters().momentum().z(),
                    candKineParticle->currentState().kinematicParameters().energy() );
            reco::Vertex theVtx = *(candKineVertex.get());
            reco::Particle::Point vtx(theVtx.x(), theVtx.y(), theVtx.z());
            const reco::Vertex::CovarianceMatrix vtxCov(theVtx.covariance());
            double vtxChi2(theVtx.chi2());
            double vtxNdof(theVtx.ndof());

            reco::VertexCompositeCandidate* fourTkCand =
                new reco::VertexCompositeCandidate(0, candP4, vtx, vtxCov, vtxChi2, vtxNdof);
            std::shared_ptr < TrajectoryStateClosestToPoint > muPosTraj;
            std::shared_ptr < TrajectoryStateClosestToPoint > muNegTraj;
            std::shared_ptr < TrajectoryStateClosestToPoint > tkPosTraj;
            std::shared_ptr < TrajectoryStateClosestToPoint > tkNegTraj;

            // start to get daughter information.
            // start to record mu+
            fourTracksKineTree->movePointerToTheTop();
            fourTracksKineTree->movePointerToTheFirstChild();
            RefCountedKinematicVertex   pMuKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex pMuVtx = *(pMuKineVertex.get());
            const reco::Vertex::CovarianceMatrix pMuVtxCov(pMuVtx.covariance());
            GlobalPoint pMuVtxPoint( pMuVtx.x(), pMuVtx.y(), pMuVtx.z() );
            muPosTraj.reset(new TrajectoryStateClosestToPoint(muPosTransTk.trajectoryStateClosestToPoint(pMuVtxPoint)) );
            if ( muPosTraj.get() == 0 ) { printf("fourTracksFittingMeghod3::fitAll() : muPosTkRef recounting failed\n"); continue; }
            reco::Particle::LorentzVector pMuMom(
                    muPosTraj->momentum().x(),muPosTraj->momentum().y(),muPosTraj->momentum().z(),
                    sqrt(muPosTraj->momentum().mag2() + optD[muPosMass]*optD[muPosMass]) );

            reco::RecoChargedCandidate pMuCand( 1, pMuMom, pMuVtx.position() );
            pMuCand.setTrack( muPosTkRef );


            // start to record mu-
            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicVertex   nMuKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex nMuVtx = *(nMuKineVertex.get());
            const reco::Vertex::CovarianceMatrix nMuVtxCov(nMuVtx.covariance());
            GlobalPoint nMuVtxPoint( nMuVtx.x(), nMuVtx.y(), nMuVtx.z() );
            muNegTraj.reset(new TrajectoryStateClosestToPoint(muNegTransTk.trajectoryStateClosestToPoint(nMuVtxPoint)) );
            if ( muNegTraj.get() == 0 ) { printf("fourTracksFittingMeghod3::fitAll() : muNegTkRef recounting failed\n"); continue; }
            reco::Particle::LorentzVector nMuMom(
                    muNegTraj->momentum().x(),muNegTraj->momentum().y(),muNegTraj->momentum().z(),
                    sqrt(muNegTraj->momentum().mag2() + optD[muNegMass]*optD[muNegMass]) );

            reco::RecoChargedCandidate nMuCand( -1, nMuMom, nMuVtx.position() );
            nMuCand.setTrack( muNegTkRef );


            // start to record tktk
            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicVertex   tktkKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex tktkVtx = *(tktkKineVertex.get());
            const reco::Vertex::CovarianceMatrix tktkVtxCov(tktkVtx.covariance());
            GlobalPoint tktkVtxPoint( tktkVtx.x(), tktkVtx.y(), tktkVtx.z() );
            tkPosTraj.reset(new TrajectoryStateClosestToPoint(tkPosTransTk.trajectoryStateClosestToPoint(tktkVtxPoint)) );
            tkNegTraj.reset(new TrajectoryStateClosestToPoint(tkNegTransTk.trajectoryStateClosestToPoint(tktkVtxPoint)) );
            if ( tkPosTraj.get() == 0 ) { printf("fourTracksFittingMeghod3::fitAll() : tkPosTkRef recounting failed\n"); continue; }
            if ( tkNegTraj.get() == 0 ) { printf("fourTracksFittingMeghod3::fitAll() : tkNegTkRef recounting failed\n"); continue; }
            reco::Particle::LorentzVector pTkMom(
                    tkPosTraj->momentum().x(),tkPosTraj->momentum().y(),tkPosTraj->momentum().z(),
                    sqrt(tkPosTraj->momentum().mag2() + optD[tkPosMass]*optD[tkPosMass]) );
            reco::Particle::LorentzVector nTkMom(
                    tkNegTraj->momentum().x(),tkNegTraj->momentum().y(),tkNegTraj->momentum().z(),
                    sqrt(tkNegTraj->momentum().mag2() + optD[tkNegMass]*optD[tkNegMass]) );

            reco::RecoChargedCandidate pTkCand( 1, pTkMom, tktkVtx.position() );
            reco::RecoChargedCandidate nTkCand(-1, nTkMom, tktkVtx.position() );
            pTkCand.setTrack( tkPosTkRef );
            nTkCand.setTrack( tkNegTkRef );


            AddFourMomenta addp4;

            // for check
            reco::VertexCompositeCandidate MuMuCand( 0, pMuMom+nMuMom, pMuVtx.position(), pMuVtxCov, pMuKineVertex->chiSquared(), pMuKineVertex->degreesOfFreedom() );
            MuMuCand.addDaughter( pMuCand, optS[muPosName] );
            MuMuCand.addDaughter( nMuCand, optS[muNegName] );
            addp4.set(MuMuCand);
            
            reco::VertexCompositeCandidate TkTkCand( 0, pTkMom+nTkMom, tktkVtx.position(), tktkVtxCov, tktkKineVertex->chiSquared(), tktkKineVertex->degreesOfFreedom() );
            TkTkCand.addDaughter( pTkCand, optS[tkPosName] );
            TkTkCand.addDaughter( nTkCand, optS[tkNegName] );


            fourTkCand->addDaughter( MuMuCand,optS[mumuName] );
            fourTkCand->addDaughter( TkTkCand,optS[tktkName] );
            fourTkCand->setPdgId(5122);
            addp4.set(*fourTkCand);
            if ( fourTkCand->mass() != candP4.mag() ) std::cout << "fourTracksFittingMeghod3::fitAll() : warning : final mumutktk refit mass is not the same as component sum\n";


            if ( fourTkCand->mass() > optD[mCandMassCut] &&
                 fourTkCand->mass() < optD[MCandMassCut] )
            {
                tmpContainerToTkTkCands[nCandsSize++] = *fourTkCand;
                cutRecord += 1 << 0;
            }
            if ( nCandsSize == tmpContainerSize )
                enlargeContainer();
            delete fourTkCand;
            fourTkCand = nullptr;
        } // tktkPair loop end
    } // mumuPair loop end

    fillInContainer();
    return;
}

