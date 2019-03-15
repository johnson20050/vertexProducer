#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
#include "TVector2.h"

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
    const double deltaR = 0.04;
    if ( cand.charge() != mcParticle.charge() ) return false;
    if ( pow(cand.eta()-mcParticle.eta(),2)+pow(TVector2::Phi_mpi_pi(cand.phi()-mcParticle.phi()),2) > deltaR*deltaR ) return false;
    return true;
}
bool familyRelationShip::truthMatching(const reco::Track& trk, const reco::Candidate& mcParticle)
{
    const double deltaRTarget = 0.25;
    if ( trk.charge() != mcParticle.charge() ) return false;
    
    // testing
    bool test1 = true;
    bool test2 = true;
    double EtaDiff = fabs(trk.eta()-mcParticle.eta());
    double PhiDiff = fabs(TVector2::Phi_mpi_pi(trk.phi()-mcParticle.phi()));
    double deltaR = sqrt(EtaDiff*EtaDiff+PhiDiff*PhiDiff);
    double deltaRErr = (trk.etaError()*EtaDiff+TVector2::Phi_mpi_pi(trk.phiError())*PhiDiff)/deltaR;
    if (deltaR>3*deltaRErr) return false;
    if (deltaR>3*deltaRErr) test1 = false;
    //if ( deltaR > deltaRTarget ) return false;
    //if ( trk.eta()-mcParticle.eta() > 2.* trk.etaError() ) return false;
    //if ( trk.phi()-mcParticle.phi() > 2.* trk.phiError() ) return false;
    //if ( pow(trk.eta()-mcParticle.eta(),2)+pow(TVector2::Phi_mpi_pi(trk.phi()-mcParticle.phi()),2) > deltaRTarget*deltaRTarget ) return false;
    if ( (trk.pt()-mcParticle.pt())/mcParticle.pt() > 0.10 ) test2 = false;
    //if ( (trk.pt()-mcParticle.pt())/mcParticle.pt() > 0.10 ) return false;
    if ( test1 || test2 ) printf("familyRelationShip::truthMatching() : method Error res ( %s )   ,   method abs res ( %s )\n", test1?"pass":"fail", test2?"pass":"fail");
    // tested
    return test1||test2;
    //return true;

    //if ( pow(cand.eta()-mcParticle.eta(),2)+pow(TVector2::Phi_mpi_pi(cand.phi()-mcParticle.phi()),2) > deltaR*deltaR ) return false;
    //return true;
}
bool familyRelationShip::isTargetMother(const reco::GenParticle& mc)
{
    unsigned nTrue = 0;
    for ( unsigned i=0; i<getNDaug(); ++i )
    {
        const reco::Candidate* daugPtr = &mc;
        for ( int layerIdx = 0; layerIdx < daugLayer(i); ++layerIdx )
        {
            // check if particle with wrong decay mode, it is needed to be removed.
            if ( daugPtr->status() == 2 ) return false;
            daugPtr = daugPtr->daughter( getDaughterIdxOnLayer(i,layerIdx) );
        }
        if ( !daugPtr ) continue;
        if ( abs(daugPtr->pdgId()) == 2212 || abs(daugPtr->pdgId()) == 321 || abs(daugPtr->pdgId()) == 13 )
            ++nTrue;
    }
    if ( nTrue == getNDaug() )
        return true;
    return false;
}
