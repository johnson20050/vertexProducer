#include "vertexProducer/MCMatchTools/interface/familyRelationShipLbToPcK.h"

// this is needed to varify.
void familyRelationShipLbToPcK::fillDaugIdx()
{
    // Lb->Pc->Jpsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    daugLayer3Idx[0] = 0;
    // Lb->Pc->Jpsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 0;
    daugLayer3Idx[1] = 1;
    // Lb->Pc->proton
    daugLayer1Idx[2] = 0;
    daugLayer2Idx[2] = 1;
    // Lb->kaon
    daugLayer1Idx[3] = 1;
    return;
}
