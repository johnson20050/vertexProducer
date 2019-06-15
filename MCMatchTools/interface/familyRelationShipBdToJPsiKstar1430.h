#ifndef __familyRelationShipBdToJPsiKstar1430_h__
#define __familyRelationShipBdToJPsiKstar1430_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bd->JPsi+Kstar1430, JPsi->mu+mu, kstar1430->K + pi
// finalstateparticle is mu + K + pi.

class familyRelationShipBdToJPsiKstar1430 : public familyRelationShip
{
public :
    familyRelationShipBdToJPsiKstar1430();
    virtual void fillDaugIdx();
};

#endif
