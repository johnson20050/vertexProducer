#ifndef __familyRelationShipLbToJPsiL0_h__
#define __familyRelationShipLbToJPsiL0_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"

class familyRelationShipLbToJPsiL0 : public familyRelationShip
{
public :
    familyRelationShipLbToJPsiL0() : familyRelationShip(4) {}
    virtual void fillDaugIdx();
};

#endif
