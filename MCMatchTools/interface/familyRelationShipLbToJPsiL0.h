#ifndef __familyRelationShipLbToJPsiL0_h__
#define __familyRelationShipLbToJPsiL0_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Lb->JPsi+Lam0, JPsi->mu+mu, Lam0->p+pi
// finalstateparticle is mu, proton, pion


class familyRelationShipLbToJPsiL0 : public familyRelationShip
{
public :
    familyRelationShipLbToJPsiL0();
    virtual void fillDaugIdx();
};

#endif
