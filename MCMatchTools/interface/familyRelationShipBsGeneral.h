#ifndef __familyRelationShipBsGeneral_h__
#define __familyRelationShipBsGeneral_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bs->JPsi+?, JPsi->mu+mu
// finalstateparticle is mu + ?. But ? not showing

class familyRelationShipBsGeneral : public familyRelationShip
{
public :
    familyRelationShipBsGeneral();
    virtual void fillDaugIdx();
};

#endif
