#ifndef __familyRelationShipBsGeneral_h__
#define __familyRelationShipBsGeneral_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Lb->JPsi+p+K, JPsi->mu+mu
// finalstateparticle is mu, proton, kaon

class familyRelationShipBsGeneral : public familyRelationShip
{
public :
    familyRelationShipBsGeneral();
    virtual void fillDaugIdx();
};

#endif
