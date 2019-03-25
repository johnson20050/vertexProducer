#ifndef __familyRelationShipLbToPcK_h__
#define __familyRelationShipLbToPcK_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Lb->Pc+K. Pc->JPsi+p. JPsi->mu+mu
// finalstateparticle is mu, proton, kaon

class familyRelationShipLbToPcK : public familyRelationShip
{
public :
    familyRelationShipLbToPcK();
    virtual void fillDaugIdx();
};

#endif
