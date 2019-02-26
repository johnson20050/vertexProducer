#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToJPsipK.h"

// this is needed to varify.
void familyRelationShipLbToJPsipK::fillDaugIdx()
{
    // Lb->Jpsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Lb->Jpsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 0;
    // Lb->proton
    daugLayer1Idx[2] = 1;
    // Lb->kaon
    daugLayer1Idx[3] = 2;
    return;
}
