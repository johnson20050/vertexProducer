#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdToJPsiKpi.h"
//Only choose for Bd->JPsi(@#$%). So only select mu+ and mu- for final state

familyRelationShipBdToJPsiKpi::familyRelationShipBdToJPsiKpi() :
    familyRelationShip(4)
{
    std::vector<unsigned> pidList = { 13, 321, 211 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipBdToJPsiKpi::fillDaugIdx()
{
    // Bd->JPsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Bd->JPsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    // Bd->K
    daugLayer1Idx[2] = 1;
    // Bd->Pi
    daugLayer1Idx[3] = 2;
    return;
}
