#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToJPsipK.h"

familyRelationShipLbToJPsipK::familyRelationShipLbToJPsipK() :
    familyRelationShip(4)
{
    std::vector<unsigned> pidList = { 13, 2212, 321 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipLbToJPsipK::fillDaugIdx()
{
    // Lb->Jpsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Lb->Jpsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    // Lb->proton
    daugLayer1Idx[2] = 1;
    // Lb->kaon
    daugLayer1Idx[3] = 2;
    return;
}
