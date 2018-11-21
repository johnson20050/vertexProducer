#include "vertexProducer/vertexProducer/interface/fourTracksFittingMethod1.h"

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
void fourTracksFittingMethod1::fitAll(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
    clearAndInitializeContainer();

    using std::vector;
    using std::cout;
    using std::endl;
    using namespace reco;
    using namespace edm;



    // Handles for tracks, B-field, and tracker geometry
    Handle < reco::VertexCompositeCandidateCollection > theMuMuPairHandle;
    Handle < reco::VertexCompositeCandidateCollection > theTkTkPairHandle;
    Handle < reco::BeamSpot > theBeamSpotHandle;

    ESHandle < MagneticField > bFieldHandle;
    ESHandle < GlobalTrackingGeometry > globTkGeomHandle;

    // Get the tracks from the event, and get the B-field record
    //  from the EventSetup
    iEvent.getByToken(mumuPairToken, theMuMuPairHandle);
    iEvent.getByToken(tktkPairToken, theTkTkPairHandle);
    iEvent.getByToken(beamspotToken, theBeamSpotHandle);
    if (theBeamSpotHandle.isValid()) std::cout << " get bs ";
    if (theTkTkPairHandle.isValid()) std::cout << " get tk ";
    if (theMuMuPairHandle.isValid()) std::cout << " get mu ";
    std::cout << "\n";
    if (!theBeamSpotHandle.isValid()) return;
    if (!theTkTkPairHandle.isValid()) return;
    if (!theMuMuPairHandle.isValid()) return;
    if (!theMuMuPairHandle->size()) return;
    if (!theTkTkPairHandle->size()) return;
            std::cout << "chk point00 \n";
    iSetup.get < IdealMagneticFieldRecord > ().get(bFieldHandle);
    iSetup.get < GlobalTrackingGeometryRecord > ().get(globTkGeomHandle);
    magField = bFieldHandle.product();

    // load mumuCandidate & tktkCandidate to do the vertexing
    for ( const VertexCompositeCandidate& mumuCand : *(theMuMuPairHandle.product()) )
    {
            std::cout << "chk point01 \n";
        
        
        for ( const VertexCompositeCandidate& tktkCand : *(theTkTkPairHandle.product()) )
        {
            //reco::RecoChargedCandidate* tkPosCandPtr = tktkCand.daughter( optS[tkPosName] );
            //reco::RecoChargedCandidate* tkNegCandPtr = tktkCand.daughter( optS[tkNegName] );
            std::cout << "chk point0 \n";
            const reco::RecoChargedCandidate* tkPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkPosName]) );
            const reco::RecoChargedCandidate* tkNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( tktkCand.daughter(optS[tkNegName]) );
        const reco::RecoChargedCandidate* muPosCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muPosName]) );
        const reco::RecoChargedCandidate* muNegCandPtr = dynamic_cast<const reco::RecoChargedCandidate*>( mumuCand.daughter(optS[muNegName]) );

            // preselections
            double fdSig = FDSig( tktkCand, mumuCand );
            if ( fdSig > optD[FDSigPreCut_tktkTomumu] ) continue;

            std::cout << "chk point1 \n";

            // build trackRef & TransientTracks
            reco::TrackRef muPosTkRef =  muPosCandPtr->track();
            reco::TrackRef muNegTkRef =  muNegCandPtr->track();
            reco::TrackRef tkPosTkRef =  tkPosCandPtr->track();
            reco::TrackRef tkNegTkRef =  tkNegCandPtr->track();

            reco::TransientTrack muPosTransTk( *muPosTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack muNegTransTk( *muNegTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack tkPosTransTk( *tkPosTkRef, &(*bFieldHandle), globTkGeomHandle );
            reco::TransientTrack tkNegTransTk( *tkNegTkRef, &(*bFieldHandle), globTkGeomHandle );


            KinematicParticleFactoryFromTransientTrack pFactory;
            std::vector<RefCountedKinematicParticle> mumutktkCand;
            mumutktkCand.reserve(4);
            float m1sig = optD[muPosSigma];
            float m2sig = optD[muNegSigma];
            float m3sig = optD[tkPosSigma];
            float m4sig = optD[tkNegSigma];
            mumutktkCand.emplace_back( pFactory.particle(muPosTransTk,optD[muPosMass], 0., 0., m1sig) );
            mumutktkCand.emplace_back( pFactory.particle(muNegTransTk,optD[muNegMass], 0., 0., m2sig) );
            mumutktkCand.emplace_back( pFactory.particle(tkPosTransTk,optD[tkPosMass], 0., 0., m3sig) );
            mumutktkCand.emplace_back( pFactory.particle(tkNegTransTk,optD[tkNegMass], 0., 0., m4sig) );

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
            if ( FDSig( *fourTkCand, *theBeamSpotHandle.product() ) > optD[FDSigCut_mumutktkToBS] )
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
            RefCountedKinematicParticle pTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::RecoChargedCandidate pTkCand( 1, reco::Particle::LorentzVector(
                        pTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        pTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        pTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        pTkMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            pTkCand.setTrack( tkPosTkRef );

            fourTracksKineTree->movePointerToTheNextChild();
            RefCountedKinematicParticle nTkMomRefitted = fourTracksKineTree->currentParticle();
            reco::RecoChargedCandidate nTkCand(-1, reco::Particle::LorentzVector(
                        nTkMomRefitted->currentState().kinematicParameters().momentum().x(),
                        nTkMomRefitted->currentState().kinematicParameters().momentum().y(),
                        nTkMomRefitted->currentState().kinematicParameters().momentum().z(),
                        nTkMomRefitted->currentState().kinematicParameters().energy() ), vtx );
            nTkCand.setTrack( tkNegTkRef );

            AddFourMomenta addp4;

            fourTkCand->addDaughter( pMuCand, optS[muPosName] );
            fourTkCand->addDaughter( nMuCand, optS[muNegName] );
            fourTkCand->addDaughter( pTkCand, optS[tkPosName] );
            fourTkCand->addDaughter( nTkCand, optS[tkNegName] );
            fourTkCand->setPdgId(5122);
            addp4.set(*fourTkCand);

            std::cout << "chk point2 \n";

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

