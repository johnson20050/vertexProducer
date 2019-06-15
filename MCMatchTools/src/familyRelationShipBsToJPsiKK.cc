#include "vertexProducer/MCMatchTools/interface/familyRelationShipBsToJPsiKK.h"
//Only choose for Bs->JPsi(@#$%). So only select mu+ and mu- for final state

familyRelationShipBsToJPsiKK::familyRelationShipBsToJPsiKK() :
    familyRelationShip(4)
{
    std::vector<unsigned> pidList = { 13, 321 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipBsToJPsiKK::fillDaugIdx()
{
    // Bs->JPsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Bs->JPsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    // Bs->Kpos
    daugLayer1Idx[2] = 1;
    // Bs->Kneg
    daugLayer1Idx[3] = 1;
    return;
}
