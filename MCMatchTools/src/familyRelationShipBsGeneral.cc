#include "vertexProducer/MCMatchTools/interface/familyRelationShipBsGeneral.h"
//Only choose for Bs->JPsi(@#$%). So only select mu+ and mu- for final state

familyRelationShipBsGeneral::familyRelationShipBsGeneral() :
    familyRelationShip(2)
{
    std::vector<unsigned> pidList = { 13 };
    setupDaugPID( pidList );
    fillDaugIdx();
    return;
}

// this is needed to varify.
void familyRelationShipBsGeneral::fillDaugIdx()
{
    // Lb->JPsi->muPos
    daugLayer1Idx[0] = 0;
    daugLayer2Idx[0] = 0;
    // Lb->JPsi->muNeg
    daugLayer1Idx[1] = 0;
    daugLayer2Idx[1] = 1;
    return;
}
