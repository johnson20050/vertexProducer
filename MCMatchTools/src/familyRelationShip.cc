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
    const double deltaR = 0.04;
    if ( trk.charge() != mcParticle.charge() ) return false;
    
    // testing
    if ( trk.eta()-mcParticle.eta() < 3.* trk.etaError() ) return false;
    if ( trk.phi()-mcParticle.phi() < 3.* trk.phiError() ) return false;
    // tested
    return true;

    //if ( pow(cand.eta()-mcParticle.eta(),2)+pow(TVector2::Phi_mpi_pi(cand.phi()-mcParticle.phi()),2) > deltaR*deltaR ) return false;
    //return true;
}
