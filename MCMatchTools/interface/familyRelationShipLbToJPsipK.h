#ifndef __familyRelationShipLbToJPsipK_h__
#define __familyRelationShipLbToJPsipK_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"

class familyRelationShipLbToJPsipK : public familyRelationShip
{
public :
    familyRelationShipLbToJPsipK() : familyRelationShip(4) {}
    virtual void fillDaugIdx();
};

#endif
