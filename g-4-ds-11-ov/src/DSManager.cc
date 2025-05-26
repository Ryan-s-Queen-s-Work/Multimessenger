#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "DSLogger.hh"
#include "DSManagerMessenger.hh"
#include "DSEventHandler.hh"
#include "DSMaterial.hh"
#include "DSStorage.hh"
//---------------------------------------------------------------------------//

#include <time.h>
#include <iostream>
#include "Randomize.hh"

//---------------------------------------------------------------------------//

#include "DSIO.hh"
#include "DSManager.hh"

//---------------------------------------------------------------------------//
DSManager* DSManager::Manager = 0;

DSManager::DSManager() {
  Manager = this;
  fMinEnergy = 0;
  fMaxEnergy = 1*GeV;

  fIsTPCEnergy = false;
  fIsIVEnergy = false;
  fIsDaughters = false;
  fIsDeposits = false;
  fIsArDM  = false;
  fIsARIS  = false;

  fDSMessenger = new DSManagerMessenger(this);
}

DSManager* DSManager::Get() {
  if (!Manager) Manager = new DSManager();
  return Manager;
}

//---------------------------------------------------------------------------//

// DSManager::DSManager(const DSManager & other)
//{;}

//---------------------------------------------------------------------------//

DSManager::~DSManager() {
  delete fDSMessenger;
}


G4bool DSManager::EventSurvivesFiltering() {
  G4bool doFilter = true;
  if(fIsTPCEnergy) doFilter &=  FilterTPCEnergy();
  if(fIsTPCNpe)    doFilter &=  FilterTPCNpe();
  if(fIsIVEnergy)  doFilter &=  FilterIVEnergy();
  if(fIsDaughters) doFilter &=  FilterDaughters();
  if(fIsDeposits)  doFilter &=  FilterDeposits();
  if(fIsArDM)      doFilter &=  FilterArDM();
  if(fIsARIS)      doFilter &=  FilterARIS();
  return doFilter;
}


void DSManager::SetFilter(G4String FilterName) {
  if      (strcmp(FilterName, "TPCEnergy")   == 0)   fIsTPCEnergy = true;
  else if (strcmp(FilterName, "TPCNpe") == 0)        fIsTPCNpe = true;
  else if (strcmp(FilterName, "IVEnergy") == 0)      fIsIVEnergy = true;
  else if (strcmp(FilterName, "Daughters") == 0)     fIsDaughters = true;
  else if (strcmp(FilterName, "Deposits")     == 0)  fIsDeposits = true;
  else if (strcmp(FilterName, "ArDM") == 0)          fIsArDM  = true;
  else if (strcmp(FilterName, "ARIS") == 0)          fIsARIS  = true;

  DSLog(routine) << "Data Storage Filter: skip particles applying filter: " << FilterName   << endlog ;

}

G4bool DSManager::FilterTPCNpe() {

  if ( DSEventHandler::Get()->GetNPE() > 0) return true;

  return false;
}


G4bool DSManager::FilterTPCEnergy() {

  if ( DSEventHandler::Get()->GetTPCDepEnergy() >= fMinEnergy / keV
    && DSEventHandler::Get()->GetTPCDepEnergy() <= fMaxEnergy / keV) return true;

  return false;
}

G4bool DSManager::FilterIVEnergy() {

  if ( DSEventHandler::Get()->GetVetoDepEnergy() >= fMinEnergy / keV
    && DSEventHandler::Get()->GetVetoDepEnergy() <= fMaxEnergy / keV) return true;

  return false;
}


G4bool DSManager::FilterDaughters() {
  if (DSEventHandler::Get()->GetNDaughters() == 0) return false;
  return true ;
}

G4bool DSManager::FilterDeposits() {
  if (DSEventHandler::Get()->GetNDeposits() == 0) return false;
  return true ;
}
G4bool DSManager::FilterArDM() {

  bool inArDM = false;

  for (int i = 0; i < DSEventHandler::Get()->GetNDeposits(); i++) {
    DepositStructure& dep = DSEventHandler::Get()->GetVDeposits()[i];

    // if deposit in ArDM or DART
    if (   dep.Volume == int(DSMaterial::Get()->GetPseudoArgon()->GetIndex())
        || dep.Volume == int(DSMaterial::Get()->GetLiquidArgon()->GetIndex())) {

      if (!inArDM) DSStorage::Get()->IncrArDMeventsShieldPE();

      if (DSEventHandler::Get()->GetVDeposits()[i].Energy > 0) return true;  // event accepted, will be written on file
      inArDM = true;
    }

    // events crossing the PE shield of ArDM
    if (!inArDM && DSEventHandler::Get()->GetVDeposits()[i].Volume != int(DSMaterial::Get()->GetAir()->GetIndex())
                && DSEventHandler::Get()->GetVDeposits()[i].Volume != int(DSMaterial::Get()->GetVacuum()->GetIndex())) {
      inArDM = true;
      DSStorage::Get()->IncrArDMeventsShieldPE();
    }
  }

  return false;  // event rejected, will not to be written on file
}

G4bool DSManager::FilterARIS() {

  bool doFilter = false;
  for (int i = 0; i < DSEventHandler::Get()->GetNDeposits(); i++) {
    int volume = DSEventHandler::Get()->GetVDeposits()[i].Volume;
    if ( volume == int(DSMaterial::Get()->GetBariumFluoride1()->GetIndex())
      || volume == int(DSMaterial::Get()->GetBariumFluoride2()->GetIndex())
      || volume == int(DSMaterial::Get()->GetGermanium()->GetIndex())
      || volume == int(DSMaterial::Get()->GetLiquidArgon()->GetIndex())) {
      doFilter = true;
      break;
    }
  }
  return doFilter;  // event rejected, will not to be written on file

}

/*
 * $Log: DSManager.cc,v $
 * Revision 1.1  2014/05/07 12:21:04  dfranco
 * Migration to Geant4.10.p01
 *
 * Revision 1.2  2013/03/22 14:09:40  dfranco
 * Added the cvs logger code at the end of each file
 *
 */
