#ifndef __familyRelationShipBdGeneral_h__
#define __familyRelationShipBdGeneral_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Lb->JPsi+p+K, JPsi->mu+mu
// finalstateparticle is mu, proton, kaon

class familyRelationShipBdGeneral : public familyRelationShip
{
public :
    familyRelationShipBdGeneral();
    virtual void fillDaugIdx();
};

#endif
