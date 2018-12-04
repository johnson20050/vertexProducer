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



    // load mumuCandidate & tktkCandidate to do the vertexing
    for ( unsigned mumuIdx = 0; mumuIdx != mumuCands.size(); ++mumuIdx )
    {
        const reco::VertexCompositeCandidate& mumuCand = mumuCands[mumuIdx];
        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuPos") );
        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter("MuNeg") );
        
        
        for ( unsigned tktkIdx = 0; tktkIdx != tktkCands.size(); ++tktkIdx )
        //for ( int tktkIdx = 0; tktkIdx < tktkCands.size(); ++tktkIdx )
        {
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
            reco::TrackRef muPosTkRef =  muPosCandPtr->track();
            reco::TrackRef muNegTkRef =  muNegCandPtr->track();
            reco::TrackRef tkPosTkRef =  tkPosCandPtr->track();
            reco::TrackRef tkNegTkRef =  tkNegCandPtr->track();

            reco::TransientTrack muPosTransTk( *muPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack muNegTransTk( *muNegTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkPosTransTk( *tkPosTkRef, magField, globTkGeomHandle );
            reco::TransientTrack tkNegTransTk( *tkNegTkRef, magField, globTkGeomHandle );

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
            reco::Particle::Point vtx(theVtx.x(), theVtx.y(), theVtx.z());
            const reco::Vertex::CovarianceMatrix vtxCov(theVtx.covariance());
            double vtxChi2(theVtx.chi2());
            double vtxNdof(theVtx.ndof());

            reco::VertexCompositeCandidate* fourTkCand =
                new reco::VertexCompositeCandidate(0, candP4, vtx, vtxCov, vtxChi2, vtxNdof);


            // start to get daughter information.
            fourTracksKineTree->movePointerToTheTop();
            fourTracksKineTree->movePointerToTheFirstChild();
            RefCountedKinematicParticle pMuMomRefitted = fourTracksKineTree->currentParticle();
            // testing
            RefCountedKinematicVertex   pMuKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex pMuVtx = *(pMuKineVertex.get());
            const reco::Vertex::CovarianceMatrix pMuVtxCov(pMuVtx.covariance());
            // tested
            reco::RecoChargedCandidate pMuCand( 1, reco::Particle::LorentzVector(
                        pMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        pMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        pMuMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            pMuCand.setTrack( muPosTkRef );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle nMuMomRefitted = fourTracksKineTree->currentParticle();
            // testing
            RefCountedKinematicVertex   nMuKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex nMuVtx = *(nMuKineVertex.get());
            const reco::Vertex::CovarianceMatrix nMuVtxCov(nMuVtx.covariance());
            // tested
            reco::RecoChargedCandidate nMuCand(-1, reco::Particle::LorentzVector(
                        nMuMomRefitted->currentState().kinematicParameters().momentum().x(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().y(),
                        nMuMomRefitted->currentState().kinematicParameters().momentum().z(),
                        nMuMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            nMuCand.setTrack( muNegTkRef );

            fourTracksKineTree->movePointerToTheNextChild();
            // testing
            RefCountedKinematicVertex   tktkKineVertex   = fourTracksKineTree->currentDecayVertex();
            reco::Vertex tktkVtx = *(tktkKineVertex.get());
            const reco::Vertex::CovarianceMatrix tktkVtxCov(tktkVtx.covariance());
            // tested
            RefCountedKinematicParticle TkTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::VertexCompositeCandidate TkTkCand( 0, reco::Particle::LorentzVector(
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        TkTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        TkTkMomRefitted->currentState().kinematicParameters().energy() ), 
                        tktkVtx.position(), tktkVtxCov, tktkKineVertex->chiSquared(), tktkKineVertex->degreesOfFreedom() );

            AddFourMomenta addp4;

            fourTkCand->addDaughter( pMuCand, optS[muPosName] );
            fourTkCand->addDaughter( nMuCand, optS[muNegName] );
            fourTkCand->addDaughter( TkTkCand,optS[tktkName] );
            fourTkCand->setPdgId(5122);
            addp4.set(*fourTkCand);


//            // testing
//    typedef ROOT::Math::SMatrix< double, 3, 3, ROOT::Math::MatRepSym<double,3> > SMatrixSym3D;
//    typedef ROOT::Math::SVector< double, 3 > SVector3;
//            SMatrixSym3D mumuCOV = pMuVtxCov + nMuVtxCov;
//            SMatrixSym3D mutkCOV = pMuVtxCov + tktkVtxCov;
//            SMatrixSym3D tktkCOV = vtxCov+ tktkCand.vertexCovariance();
//            SMatrixSym3D muJPCOV = mumuCand.vertexCovariance() + vtxCov;
//            SVector3 mumuFDvector( pMuVtx.x() - nMuVtx.x(), pMuVtx.y() - nMuVtx.y(), 0. ); 
//            SVector3 mutkFDvector( pMuVtx.x() -tktkVtx.x(),pMuVtx.y() -tktkVtx.y(), 0. );
//            SVector3 tktkFDvector( tktkVTX.x()-    vtx.x(),tktkVTX.y()-    vtx.y(), 0. );
//            SVector3 muJPFDvector( mumuVTX.x()-    vtx.x(), mumuVTX.y()-    vtx.y(), 0. );
//            double mumuFDmag = ROOT::Math::Mag( mumuFDvector ); 
//            double mutkFDmag = ROOT::Math::Mag( mutkFDvector ); 
//            double tktkFDmag = ROOT::Math::Mag( tktkFDvector ); 
//            double muJPFDmag = ROOT::Math::Mag( muJPFDvector ); 
//            double mumuFDerr = sqrt( ROOT::Math::Similarity(mumuCOV, mumuFDvector) ) / mumuFDmag;
//            double mutkFDerr = sqrt( ROOT::Math::Similarity(mutkCOV, mutkFDvector) ) / mutkFDmag;
//            double tktkFDerr = sqrt( ROOT::Math::Similarity(tktkCOV, tktkFDvector) ) / tktkFDmag;
//            double muJPFDerr = sqrt( ROOT::Math::Similarity(muJPCOV, muJPFDvector) ) / muJPFDmag;
//
//            double mumuFDSig = mumuFDmag / mumuFDerr;
//            double mutkFDSig = mutkFDmag / mutkFDerr;
//            double tktkFDSig = tktkFDmag / tktkFDerr;
//            double muJPFDSig = muJPFDmag / muJPFDerr;
//            std::cout << "--------fourTracksFitter2::fitAll() : ";
//            if ( mumuFDSig > 0.5 ) printf(" ---- mu mu is NOT at the same vertex %.4f ---- ", mumuFDSig );
//            else printf("    __mu mu SAME %.4f__    ", mumuFDSig );
//            if ( mutkFDSig < 5.  ) printf(" ---- jpsi Lambda0 IS at the same vertex %.4f ---- ", mutkFDSig );
//            else printf("    __jpsi lam0 displaced %.4f__    ", mutkFDSig);
//            if ( muJPFDSig > 0.5 ) printf(" ---- LambdaB jpsi is not at the same vertex %.4f ---- ", muJPFDSig );
//            else printf("    __LambdaB jpsi SAME %.4f__    ", muJPFDSig);
//            if ( tktkFDSig > 0.5 ) printf(" ---- LambdaB Lambda0 is NOT at the same vertex %.4f ---- ", tktkFDSig );
//            else printf("    __LambdaB lam0 SAME %.4f__    ", tktkFDSig);
//            printf(" : : : : normalizedChi2 = %.8f", vtxChi2/vtxNdof);
//            std::cout << std::endl;
//            if ( fabs(tktkVtx.x() -    vtx.x())<0.00000001 && fabs(tktkVtx.y() -    vtx.y())<0.0000001 ) printf("    :::   refit tktk vertex point is LambdaB  :::   ");
//            if ( fabs( pMuVtx.x() -    vtx.x())<0.00000001 && fabs( pMuVtx.y() -    vtx.y())<0.0000001 ) printf("    :::   refit pMu vertex point is LambdaB   :::   ");
//            if ( fabs( nMuVtx.x() -    vtx.x())<0.00000001 && fabs( nMuVtx.y() -    vtx.y())<0.0000001 ) printf("    :::   refit nMu vertex point is LambdaB   :::   ");
//            if ( fabs( nMuVtx.x() - pMuVtx.x())<0.00000001 && fabs( nMuVtx.y() - pMuVtx.y())<0.0000001 ) printf("    :::   refit nMu vertex point is pMu       :::   ");
//            if ( fabs(tktkVtx.x() - pMuVtx.x())<0.00000001 && fabs(tktkVtx.y() - pMuVtx.y())<0.0000001 ) printf("    :::   refit nMu vertex point is Lambda0   :::   ");
//            if ( fabs( nMuVtx.x() -tktkVtx.x())<0.00000001 && fabs( nMuVtx.y() -tktkVtx.y())<0.0000001 ) printf("    :::   refit pMu vertex point is Lambda0   :::   ");
//            std::cout << std::endl;
//            // tested


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


