#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SPSRandomGenerator.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "DSGeneratorAr40.hh"
#include "DSGeneratorAr40Messenger.hh"
#include "DSLogger.hh"

#include "DSEventHandler.hh"

#include <fstream>
#include <sstream>
#include <vector>

//---------------------------------------------------------------------------//


DSGeneratorAr40::DSGeneratorAr40() : DSVGenerator("DSGeneratorAr40") {
    fParticleGun = new G4ParticleGun;

    fTheMessenger = new DSGeneratorAr40Messenger(this);
    if (!fParticleGun) {
        DSLog(error) << "Could not allocate G4ParticleGun! Out of memory?" << endlog;
        DSLog(fatal) << endlog;
    }
    DSLog(routine) << "G4ParticleGun Constructed." << endlog;
}

//---------------------------------------------------------------------------//

DSGeneratorAr40::~DSGeneratorAr40() {
    delete fTheMessenger;
    delete fParticleGun;
}

//---------------------------------------------------------------------------//

void DSGeneratorAr40::LoadCSVData(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        double time, energy;
        ss >> time >> energy;
        DSLog(debugging) << "Loaded event: time = " << time << " ns, energy = " << energy << " keV" << endlog;
        recoilEvents.push_back({time * ns, energy * keV}); // Convert from keV and ns
    }
    DSLog(routine) << "[INFO] Loaded " << recoilEvents.size() << " recoil events from CSV file." << endlog;
}

//---------------------------------------------------------------------------//

void DSGeneratorAr40::DSGeneratePrimaries(G4Event* event) {
    if (recoilEvents.empty()) {
        DSLog(error) << "[ERROR] No recoil events loaded!" << endlog;
        return;
    }

    G4ThreeVector position(
        G4UniformRand() * cm, 
        G4UniformRand() * cm, 
        G4UniformRand() * cm
    );

    for (int i = 0; i < GetVNumberOfParticles(); ++i) {
        RecoilData recoil = recoilEvents.back();
        recoilEvents.pop_back();

        G4ThreeVector direction = G4RandomDirection();
        G4ParticleDefinition* particle = GetVParticleDefinition();
        if (!particle) {
         DSLog(error) << "[ERROR] GetVParticleDefinition() returned null!" << endlog;
        return;
        }
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticlePosition(position);
        fParticleGun->SetParticleMomentumDirection(direction);
        fParticleGun->SetParticleEnergy(recoil.energy);
        fParticleGun->SetParticleTime(recoil.time);

        DSLog(development) << particle->GetParticleName() << " "
                           << "Energy: " << recoil.energy / keV << " keV; "
                           << "Position: " << position / cm << " cm;  "
                           << "Direction: " << direction << " " << endlog;

        fParticleGun->GeneratePrimaryVertex(event);

        if (GetVNumberOfParticles() == 1) {
            DSEventHandler::Get()->SetPDG(particle->GetPDGEncoding());
            DSEventHandler::Get()->SetPosition(position / cm);
            DSEventHandler::Get()->SetDirection(direction);
            DSEventHandler::Get()->SetEnergy(recoil.energy / keV);
            DSEventHandler::Get()->SetTime(recoil.time / ns);

        } else {
            if (i == 0) {
                DSEventHandler::Get()->SetPDG(particle->GetPDGEncoding());
                DSEventHandler::Get()->SetPosition(position / cm);
                DSEventHandler::Get()->SetDirection(direction);
                DSEventHandler::Get()->SetEnergy(recoil.energy / keV);
                DSEventHandler::Get()->SetTime(recoil.time / ns);

            } else {
                DSEventHandler::Get()->SetDPDG(particle->GetPDGEncoding());
                DSEventHandler::Get()->SetDPosition(position / cm);
                DSEventHandler::Get()->SetDDirection(direction);
                DSEventHandler::Get()->SetDEnergy(recoil.energy / keV);
                DSEventHandler::Get()->SetDTime(recoil.time / ns);
                DSEventHandler::Get()->SetDaughters();
            }
        }
    }
}

