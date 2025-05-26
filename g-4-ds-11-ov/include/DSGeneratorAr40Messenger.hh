// --------------------------------------------------------------------------//
/**
 * AUTHOR: D. Franco (Modified for Argon-40 CSV Input)
 * CONTACT: davide.franco@mi.infn.it
 */
// --------------------------------------------------------------------------//

#ifndef DSGENERATORAR40MESSENGER_HH
#define DSGENERATORAR40MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"

class DSGeneratorAr40;

class DSGeneratorAr40Messenger : public G4UImessenger {
public:
    DSGeneratorAr40Messenger(DSGeneratorAr40* generator);
    ~DSGeneratorAr40Messenger();

    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    DSGeneratorAr40* fGenerator;
    G4UIcmdWithAString* fCSVFileCmd;
};

#endif // DSGENERATORAR40MESSENGER_HH
