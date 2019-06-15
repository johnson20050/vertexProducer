#ifndef __familyRelationShipBdGeneral_h__
#define __familyRelationShipBdGeneral_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bd->JPsi+?, JPsi->mu+mu
// finalstateparticle is mu + ?. But ? not showing

class familyRelationShipBdGeneral : public familyRelationShip
{
public :
    familyRelationShipBdGeneral();
    virtual void fillDaugIdx();
};

#endif
