#ifndef __familyRelationShipBdToJPsiKpi_h__
#define __familyRelationShipBdToJPsiKpi_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"
// give a definition to target genparticle.
// Bd->JPsi+K+pi, JPsi->mu+mu
// finalstateparticle is mu + K + pi.

class familyRelationShipBdToJPsiKpi : public familyRelationShip
{
public :
    familyRelationShipBdToJPsiKpi();
    virtual void fillDaugIdx();
};

#endif
