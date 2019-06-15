#ifndef __familyRelationShipBdToJPsiKstar892_h__
#define __familyRelationShipBdToJPsiKstar892_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bd->JPsi+Kstar1430, JPsi->mu+mu, kstar1430->K + pi
// finalstateparticle is mu + K + pi.

class familyRelationShipBdToJPsiKstar892 : public familyRelationShip
{
public :
    familyRelationShipBdToJPsiKstar892();
    virtual void fillDaugIdx();
};

#endif
