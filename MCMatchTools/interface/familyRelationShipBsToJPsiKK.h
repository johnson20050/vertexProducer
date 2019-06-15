#ifndef __familyRelationShipBsToJPsiKK_h__
#define __familyRelationShipBsToJPsiKK_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bs->JPsi+K+K, JPsi->mu+mu
// finalstateparticle is mu + K.

class familyRelationShipBsToJPsiKK : public familyRelationShip
{
public :
    familyRelationShipBsToJPsiKK();
    virtual void fillDaugIdx();
};

#endif
