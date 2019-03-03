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
	if ( !recorded ) return;
	const reco::VertexCompositeCandidateCollection& mumuCands = *(theMuMuPairHandlePtr->product());
	const reco::VertexCompositeCandidateCollection& tktkCands = *(theTkTkPairHandlePtr->product());
	const MagneticField* magField = bFieldHandlePtr->product();
	const edm::ESHandle<GlobalTrackingGeometry>& globTkGeomHandle = *(globTkGeomHandlePtr.get());
    if ( mumuCands.size() > 1 ) return; // lowering combinatorial background.

    cutRecordList.clear();
    cutRecordList.reserve(mumuCands.size()*tktkCands.size()+1);

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
			// double cosa2d = Cosa2d( tktkCand, mumuCand );
			// if ( cosa2d < optD[Cosa2dPreCut_tktkTomumu] ) continue;
			// // fd need to biggerr than N sigma.
			// double fdSig = FDSig( tktkCand, mumuCand );
			// if ( fdSig < optD[FDSigPreCut_tktkTomumu] ) continue;
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
			reco::TrackRef muPosTkRef = muPosCandPtr->track();
			reco::TrackRef muNegTkRef = muNegCandPtr->track();
			reco::TrackRef tkPosTkRef = tkPosCandPtr->track();
			reco::TrackRef tkNegTkRef = tkNegCandPtr->track();

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
			GlobalPoint tktkVTX ( tktkCand.vertex().x(),tktkCand.vertex().y(), tktkCand.vertex().z() );
			GlobalPoint mumuVTX ( mumuCand.vertex().x(),mumuCand.vertex().y(), mumuCand.vertex().z() );
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
			RefCountedKinematicTree tktkTmpKineTree;
			tktkTmpKineTree = tktkTmpFitter.fit( tktkTmpCand );
			if ( !tktkTmpKineTree->isValid() ) continue;
			RefCountedKinematicParticle tktkTmpKineParticle = tktkTmpKineTree->currentParticle();
			RefCountedKinematicVertex tktkTmpKineVertex   = tktkTmpKineTree->currentDecayVertex();
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
			RefCountedKinematicVertex candKineVertex   = fourTracksKineTree->currentDecayVertex();
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

			// start to get daughter information.
			fourTracksKineTree->movePointerToTheTop();

			// pos mu
			fourTracksKineTree->movePointerToTheFirstChild();
			RefCountedKinematicParticle pMuMomRefitted = fourTracksKineTree->currentParticle();
			reco::Particle::LorentzVector pMuMom(
				pMuMomRefitted->currentState().kinematicParameters().momentum().x(),
				pMuMomRefitted->currentState().kinematicParameters().momentum().y(),
				pMuMomRefitted->currentState().kinematicParameters().momentum().z(),
				pMuMomRefitted->currentState().kinematicParameters().energy() );
			reco::RecoChargedCandidate pMuCand( 1, pMuMom, mumuCand.vertex() );
			pMuCand.setTrack( muPosTkRef );

			// neg mu
			fourTracksKineTree->movePointerToTheNextChild();
			RefCountedKinematicParticle nMuMomRefitted = fourTracksKineTree->currentParticle();
			reco::Particle::LorentzVector nMuMom(
				nMuMomRefitted->currentState().kinematicParameters().momentum().x(),
				nMuMomRefitted->currentState().kinematicParameters().momentum().y(),
				nMuMomRefitted->currentState().kinematicParameters().momentum().z(),
				nMuMomRefitted->currentState().kinematicParameters().energy() );
			reco::RecoChargedCandidate nMuCand(-1, nMuMom, mumuCand.vertex());
			nMuCand.setTrack( muNegTkRef );

			// lambda0 ( 2 tks )
			fourTracksKineTree->movePointerToTheNextChild();
			RefCountedKinematicParticle TkTkMomRefitted = fourTracksKineTree->currentParticle();
			reco::VertexCompositeCandidate TkTkCand( 0, reco::Particle::LorentzVector(
					TkTkMomRefitted->currentState().kinematicParameters().momentum().x(),
					TkTkMomRefitted->currentState().kinematicParameters().momentum().y(),
					TkTkMomRefitted->currentState().kinematicParameters().momentum().z(),
					TkTkMomRefitted->currentState().kinematicParameters().energy() ),
			    tktkCand.vertex(), tktkCand.vertexCovariance(), tktkCand.vertexChi2(), tktkCand.vertexNdof() );

			AddFourMomenta addp4;

			reco::VertexCompositeCandidate MuMuCand( 0, pMuMom+nMuMom, mumuCand.vertex(), mumuCand.vertexCovariance(), mumuCand.vertexChi2(), mumuCand.vertexNdof() );
			MuMuCand.addDaughter( pMuCand, optS[muPosName] );
			MuMuCand.addDaughter( nMuCand, optS[muNegName] );
			addp4.set(MuMuCand);

			fourTkCand->addDaughter( MuMuCand,optS[mumuName] );
			fourTkCand->addDaughter( TkTkCand,optS[tktkName] );
			fourTkCand->setPdgId(5122);
			addp4.set(*fourTkCand);

			if ( fourTkCand->mass() > optD[mCandMassCut] &&
			    fourTkCand->mass() < optD[MCandMassCut] )
				tmpContainerToTkTkCands[nCandsSize++] = *fourTkCand;
			if ( nCandsSize == tmpContainerSize )
				enlargeContainer();
			delete fourTkCand;
			fourTkCand = nullptr;
            cutRecord += 1 << 0;
		}	// tktkPair loop end
	}	// mumuPair loop end

	fillInContainer();
	return;
}
