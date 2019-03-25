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

    // just for testing
    static double deltaR_Significance(const reco::Track& trk, const reco::Candidate& mcParticle);
    static double deltaR_Value(const reco::Track& trk, const reco::Candidate& mcParticle);


    // if the gen particle was the particle we need, pass this function.
    bool isTargetGenParticleInDecayChannel(const reco::GenParticle& mc);
    

    void setupDaugPID( std::vector<unsigned> pidList ) { finalStatePID = pidList; return; }
protected:
    unsigned *daugLayer1Idx;
    unsigned *daugLayer2Idx;
    unsigned *daugLayer3Idx;
    unsigned _nDaug;
private:
    std::vector<unsigned> finalStatePID;
};

#endif
