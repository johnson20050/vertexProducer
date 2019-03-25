#ifndef __familyRelationShipLbToJPsipK_h__
#define __familyRelationShipLbToJPsipK_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Lb->JPsi+p+K, JPsi->mu+mu
// finalstateparticle is mu, proton, kaon

class familyRelationShipLbToJPsipK : public familyRelationShip
{
public :
    familyRelationShipLbToJPsipK();
    virtual void fillDaugIdx();
};

#endif
