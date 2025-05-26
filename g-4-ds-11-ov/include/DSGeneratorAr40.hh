#ifndef DSGENERATORAR40_HH
#define DSGENERATORAR40_HH

#include "G4VPrimaryGenerator.hh"
#include "DSGeneratorG4GunMessenger.hh"
#include "DSVGenerator.hh"
#include "G4Event.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "G4ParticleGun.hh"
#include "DSVGenerator.hh"
#include "G4ThreeVector.hh"
#include "DSGeneratorAr40Messenger.hh"
#include "DSLogger.hh"
#include <vector>

//---------------------------------------------------------------------------//

struct RecoilData {
    double time;
    double energy;
};

class DSGeneratorAr40 : public DSVGenerator {
public:
    DSGeneratorAr40();
    
    
    virtual ~DSGeneratorAr40();

    void LoadCSVData(const std::string& filename);
    //void GeneratePrimaryVertex(G4Event* event) override;
    virtual void DSGeneratePrimaries(G4Event* event);


private:
    std::vector<RecoilData> recoilEvents;
    G4ParticleGun* fParticleGun;
    DSGeneratorAr40Messenger* fTheMessenger;
};

#endif // DSGENERATORAR40_HH
