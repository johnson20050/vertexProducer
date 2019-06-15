#include "vertexProducer/MCMatchTools/interface/familyRelationShipBdGeneral.h"
//Only choose for Bd->JPsi(@#$%). So only select mu+ and mu- for final state

familyRelationShipBdGeneral::familyRelationShipBdGeneral() :
    familyRelationShip(2)
{
    std::vector<unsigned> pidList = { 13 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipBdGeneral::fillDaugIdx()
{
    // Bd->JPsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Bd->JPsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    return;
}
