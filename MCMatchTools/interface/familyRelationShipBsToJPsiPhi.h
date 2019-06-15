#ifndef __familyRelationShipBsToJPsiPhi_h__
#define __familyRelationShipBsToJPsiPhi_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bs->JPsi+Phi, JPsi->mu+mu, Phi->K+K
// finalstateparticle is mu + K.

class familyRelationShipBsToJPsiPhi : public familyRelationShip
{
public :
    familyRelationShipBsToJPsiPhi();
    virtual void fillDaugIdx();
};

#endif
