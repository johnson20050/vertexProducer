#ifndef __familyRelationShipLbToPcK_h__
#define __familyRelationShipLbToPcK_h__
#include "vertexProducer/MCMatchTools/interface/familyRelationShip.h"

class familyRelationShipLbToPcK : public familyRelationShip
{
public :
    familyRelationShipLbToPcK() : familyRelationShip(4) { fillDaugIdx(); }
    virtual void fillDaugIdx();
};

#endif
