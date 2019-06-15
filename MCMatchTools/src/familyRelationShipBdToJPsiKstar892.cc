#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdToJPsiKstar892.h"
//Only choose for Bd->JPsi(@#$%). So only select mu+ and mu- for final state

familyRelationShipBdToJPsiKstar892::familyRelationShipBdToJPsiKstar892() :
    familyRelationShip(4)
{
    std::vector<unsigned> pidList = { 13, 321, 211 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipBdToJPsiKstar892::fillDaugIdx()
{
    // Bd->JPsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Bd->JPsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    // Bd->Kstar->K
    daugLayer1Idx[2] = 1;
    daugLayer2Idx[2] = 0;
    // Bd->Kstar->pi
    daugLayer1Idx[3] = 1;
    daugLayer2Idx[3] = 1;
    return;
}
