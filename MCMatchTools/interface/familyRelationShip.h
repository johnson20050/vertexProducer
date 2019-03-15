#ifndef __familyRelationShip_h__
#define __familyRelationShip_h__
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"

class familyRelationShip
{
public:
    familyRelationShip(int nDaug );
    virtual ~familyRelationShip();
    virtual void fillDaugIdx() = 0;
    static int encodeBOOL(int idx) { return 1 << idx; }
    static bool passCut(int recorder, int idx) { return (recorder>>idx)%2; }
    int daugLayer(int iDaug);
    unsigned getDaughterIdxOnLayer( int iDaug, int layer );
    unsigned getNDaug() const { return _nDaug; }
    //define how to do truth matching
    static bool truthMatching(const reco::Candidate& cand, const reco::Candidate& mcParticle);
    static bool truthMatching(const reco::Track& trk, const reco::Candidate& mcParticle);
    bool isTargetMother(const reco::GenParticle& mc);

protected:
    unsigned *daugLayer1Idx;
    unsigned *daugLayer2Idx;
    unsigned *daugLayer3Idx;
    unsigned _nDaug;
};

#endif
