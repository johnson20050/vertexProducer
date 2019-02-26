#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToJPsiL0.h"

// this is needed to varify.
void familyRelationShipLbToJPsiL0::fillDaugIdx()
{
    // Lb->Jpsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Lb->Jpsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 0;
    // Lb->proton
    daugLayer1Idx[2] = 1;
    daugLayer2Idx[2] = 0;
    // Lb->kaon
    daugLayer1Idx[3] = 1;
    daugLayer2Idx[3] = 1;
    return;
}
