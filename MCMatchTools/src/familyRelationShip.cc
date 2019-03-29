#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
#include "TVector2.h"

#define DELTARCUT 0.03
#define PTRATIOCUT 0.6

familyRelationShip::familyRelationShip( int nDaug ) : _nDaug(nDaug)
{
    daugLayer1Idx = new unsigned[_nDaug];
    daugLayer2Idx = new unsigned[_nDaug];
    daugLayer3Idx = new unsigned[_nDaug];
    for ( unsigned i=0; i<_nDaug; ++i )
    {
        daugLayer1Idx[i] = -1;
        daugLayer2Idx[i] = -1;
        daugLayer3Idx[i] = -1;
    }
    return;
}
familyRelationShip::~familyRelationShip()
{
    delete[] daugLayer1Idx;
    delete[] daugLayer2Idx;
    delete[] daugLayer3Idx;
    return;
}
int familyRelationShip::daugLayer(int iDaug)
{
    unsigned idx = (unsigned) iDaug;
    // if out
    if ( idx>=_nDaug ) return -1;
    if ( daugLayer3Idx[idx] != (unsigned)-1 ) return 3;
    if ( daugLayer2Idx[idx] != (unsigned)-1 ) return 2;
    if ( daugLayer1Idx[idx] != (unsigned)-1 ) return 1;
    return -1;
}
unsigned familyRelationShip::getDaughterIdxOnLayer( int iDaug, int layer )
{
    unsigned idx = (unsigned) iDaug;
    if ( idx>=_nDaug ) return -1;
    switch ( layer )
    {
    case 0 :
        return daugLayer1Idx[idx];
    case 1 :
        return daugLayer2Idx[idx];
    case 2 :
        return daugLayer3Idx[idx];
    default:
        return -1;
    }
    return -1;
}
bool familyRelationShip::truthMatching(const reco::Candidate& cand, const reco::Candidate& mcParticle)
{
    if ( cand.charge() != mcParticle.charge() ) return false;
    if ( (cand.pt()-mcParticle.pt())/mcParticle.pt() > PTRATIOCUT ) return false;
    if ( pow(cand.eta()-mcParticle.eta(),2)+pow(TVector2::Phi_mpi_pi(cand.phi()-mcParticle.phi()),2) > DELTARCUT*DELTARCUT ) return false;
    return true;
}
bool familyRelationShip::truthMatching(const reco::Track& trk, const reco::Candidate& mcParticle)
{
    if ( trk.charge() != mcParticle.charge() ) return false;
    if ( (trk.pt()-mcParticle.pt())/mcParticle.pt() > PTRATIOCUT )
        return false;
    
    double EtaDiff = fabs(trk.eta()-mcParticle.eta());
    double PhiDiff = fabs(TVector2::Phi_mpi_pi(trk.phi()-mcParticle.phi()));
    double deltaR = sqrt(EtaDiff*EtaDiff+PhiDiff*PhiDiff);
    //double deltaRErr = (trk.etaError()*EtaDiff+TVector2::Phi_mpi_pi(trk.phiError())*PhiDiff)/deltaR;

    return deltaR < DELTARCUT;
}
double familyRelationShip::deltaR_Significance(const reco::Track& trk, const reco::Candidate& mcParticle)
{
    double EtaDiff = fabs(trk.eta()-mcParticle.eta());
    double PhiDiff = fabs(TVector2::Phi_mpi_pi(trk.phi()-mcParticle.phi()));
    double deltaR = sqrt(EtaDiff*EtaDiff+PhiDiff*PhiDiff);
    double deltaRErr = (trk.etaError()*EtaDiff+fabs(TVector2::Phi_mpi_pi(trk.phiError()))*PhiDiff)/deltaR;

    if ( deltaR/deltaRErr<0. )
        printf("-----WARNING----- familyRelationShip::deltaR_Significance() : Significance < 0!!!!!!\n\t\t(deltaR,deltaRErr)=(%.6f,%.6f), (EtaDiff,PhiDiff)=(%.3f,%.3f), (etaErr,phiErr)=(%.5f,%.5f)\n",
                deltaR,deltaRErr,EtaDiff,PhiDiff,trk.etaError(),trk.phiError());
    return deltaR/deltaRErr;
}
double familyRelationShip::deltaR_Value(const reco::Track& trk, const reco::Candidate& mcParticle)
{
    double EtaDiff = fabs(trk.eta()-mcParticle.eta());
    double PhiDiff = fabs(TVector2::Phi_mpi_pi(trk.phi()-mcParticle.phi()));
    double deltaR = sqrt(EtaDiff*EtaDiff+PhiDiff*PhiDiff);
    return deltaR;
}


bool familyRelationShip::isTargetGenParticleInDecayChannel(const reco::GenParticle& mc)
{
    unsigned nTrue = 0;
    for ( unsigned i=0; i<getNDaug(); ++i )
    {
        const reco::Candidate* daugPtr = &mc;
        for ( int layerIdx = 0; layerIdx < daugLayer(i); ++layerIdx )
        {
            // check if particle with wrong decay mode, it is needed to be removed.
            if ( daugPtr->status() != 2 ) return false;
            daugPtr = daugPtr->daughter( getDaughterIdxOnLayer(i,layerIdx) );
        }
        if ( daugPtr )
            if ( daugPtr->status() == 1 )
                for ( int fsPID : finalStatePID )
                    if ( abs(daugPtr->pdgId()) == fsPID )
                        ++nTrue;
    }
    if ( nTrue == getNDaug() )
        return true;
    return false;
}
