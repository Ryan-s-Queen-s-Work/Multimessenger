// --------------------------------------------------------------------------//
/**
 * AUTHOR: D. Franco (Modified for Argon-40 CSV Input)
 * CONTACT: davide.franco@mi.infn.it
 */
// --------------------------------------------------------------------------//

#include "DSGeneratorAr40Messenger.hh"
#include "DSGeneratorAr40.hh"
#include "G4UIcmdWithAString.hh"
#include "DSLogger.hh"

//---------------------------------------------------------------------------//

DSGeneratorAr40Messenger::DSGeneratorAr40Messenger(DSGeneratorAr40* generator) 
    : fGenerator(generator) {
    fCSVFileCmd = new G4UIcmdWithAString("/ds/generator/ar40/setCSVFile", this);
    fCSVFileCmd->SetGuidance("Set the CSV file containing recoil events");
    fCSVFileCmd->SetParameterName("filename", false);
}

//---------------------------------------------------------------------------//

DSGeneratorAr40Messenger::~DSGeneratorAr40Messenger() {
    delete fCSVFileCmd;
}

//---------------------------------------------------------------------------//

void DSGeneratorAr40Messenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fCSVFileCmd) {
        fGenerator->LoadCSVData(newValue);
        DSLog(routine) << "[INFO] Loaded CSV file: " << newValue << endlog;
    }
}

