#ifndef DSDetectorDuneCryostat_H
#define DSDetectorDuneCryostat_H

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Para.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Paraboloid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"


class DSMaterial;

class DSDetectorDuneCryostat {

public:
  DSDetectorDuneCryostat(G4VPhysicalVolume*);
  ~DSDetectorDuneCryostat();

  G4VPhysicalVolume* GetDetectorComponent() { return fPhysicVetoArgon; }

private:

  void  DefineSurfaces();

  void PlaceSiPM();
  
  bool IsPlaceAdditionalShield;
  int PDUWLSMaterial;
  int CheckOverlap;
  bool isPENonCryo;
  bool isPENonGAr;

  G4LogicalVolume* fLogicTest;
  G4LogicalVolume* fLogicTest_sma;
  
  G4Box*              fSolidSteelTank;
  G4LogicalVolume*    fLogicSteelTank;
  G4VPhysicalVolume*  fPhysicSteelTank;

  G4Box*              fSolidOuterPlyWood;
  G4LogicalVolume*    fLogicOuterPlyWood;
  G4VPhysicalVolume*  fPhysicOuterPlyWood;

  G4Box*              fSolidOuterInsulatingFoam;
  G4LogicalVolume*    fLogicOuterInsulatingFoam;
  G4VPhysicalVolume*  fPhysicOuterInsulatingFoam;

  G4Box*              fSolidSecondaryMembrane;
  G4LogicalVolume*    fLogicSecondaryMembrane;
  G4VPhysicalVolume*  fPhysicSecondaryMembrane;

  G4Box*              fSolidInsulatingFoam2;
  G4LogicalVolume*    fLogicInsulatingFoam2;
  G4VPhysicalVolume*  fPhysicInsulatingFoam2;

  G4Box*              fSolidPlyWood2;
  G4LogicalVolume*    fLogicPlyWood2;
  G4VPhysicalVolume*  fPhysicPlyWood2;

  G4Box*              fSolidInsulatingFoam3;
  G4LogicalVolume*    fLogicInsulatingFoam3;
  G4VPhysicalVolume*  fPhysicInsulatingFoam3;

  G4Box*              fSolidPlyWood3;
  G4LogicalVolume*    fLogicPlyWood3;
  G4VPhysicalVolume*  fPhysicPlyWood3;

  G4Box*              fSolidInsulatingFoamRoof;
  G4LogicalVolume*    fLogicInsulatingFoamRoof;
  G4VPhysicalVolume*  fPhysicInsulatingFoamRoof;

  G4Box*              fSolidMembrane;
  G4LogicalVolume*    fLogicMembrane;
  G4VPhysicalVolume*  fPhysicMembrane;

  G4Box*              fSolidVetoArgon;
  G4LogicalVolume*    fLogicVetoArgon;
  G4VPhysicalVolume*  fPhysicVetoArgon;

  G4Box*              fSolidArgonVapor;
  G4LogicalVolume*    fLogicArgonVapor;
  G4VPhysicalVolume*  fPhysicArgonVapor;

  //photodetector elements and surfaces
  G4Box*                      fSolidPEN;
  G4LogicalVolume*            fLogicPEN;
  G4VPhysicalVolume*          fPhysicPEN; 

  G4OpticalSurface*           fOpLumirrorSurface;
  G4OpticalSurface*           fOpTyvekSurface;
  G4OpticalSurface*           fEPSSSurface;
  G4OpticalSurface*           fOpSurfacePENCryo;
  G4OpticalSurface*           fOpPENSurface;
  G4OpticalSurface*           fOpTPBSurface;
  G4OpticalSurface*           fOpESRSurface;

  // PDU dimensions
  G4double SiPMSide; // SiPMSide = 20.2*cm
  G4double SiPMHeight; // SiPMHeight = 0.5*cm
  G4double quadrantoffset;
  double myTPBThickness;

  G4String number;
  G4String nameactive;
  G4String namephotocathode;
  G4String namephotocathodeQ1;
  G4String namephotocathodeQ2;
  G4String namephotocathodeQ3;
  G4String namephotocathodeQ4;
  G4String namebase;
  G4String nametpb;
  G4String namepen;
  G4String nameacrylic;

  // Volume Numbers
  G4int volumeNumber_bulb;
  G4int volumeNumber_base;
  G4int volumeNumber_photocathode;

  G4int volumeNumber_tpb;
  G4int volumeNumber_pen;
  G4int volumeNumber_acrylic;

  G4Material* teflon_mat;
  G4Material* stainlesssteel_mat;
  G4Material* metalsilicon_mat;

  G4double xCoord_PDU_base;
  G4double yCoord_PDU_base;
  G4double zCoord_PDU_base;
  G4double PDU_wall_gap;
  G4double Top_Ring_Radius;
  G4double HDPEShieldThickness;
  G4RotationMatrix* RotationPDU;
  G4VPhysicalVolume* PDUMotherVolume;
  double theta;

  G4Box* MVSiPMOnCap;
  G4Box* MVSiPMActiveOnCap;
  G4Box* MVSiPMSilicaOnCap;
  G4Box* MVSolidTPB;
  G4Box* MVSolidPEN;
  G4Box* MVSolidAcrylic;

  G4LogicalVolume* myLogicShapeSiPM;
  G4LogicalVolume* myLogicShapeSiPMActive;
  G4LogicalVolume* myLogicShapeSiPMSilicaQ1;
  G4LogicalVolume* myLogicShapeSiPMSilicaQ2;
  G4LogicalVolume* myLogicShapeSiPMSilicaQ3;
  G4LogicalVolume* myLogicShapeSiPMSilicaQ4;
  G4LogicalVolume* fLogicTPB;
  G4LogicalVolume* fLogicPENonSiPM;
  G4LogicalVolume* fLogicAcrylic;

  G4double myPENThickness;
  G4double myAcrylicThickness;
  double PENOffset;
  double TPBOffset;
  double AcrylicOffset;

  int PDU_counter;
  int physSiPMIndex;

  static const int MaxPDUCount = 64; //if more than 64 PDUs, increase list length... no variable length arrays allowed
  G4VPhysicalVolume* physSiPM[MaxPDUCount];
  G4VPhysicalVolume* physSiPMActive[MaxPDUCount];
  G4VPhysicalVolume* physSiPMSilicaQ1[MaxPDUCount];
  G4VPhysicalVolume* physSiPMSilicaQ2[MaxPDUCount];
  G4VPhysicalVolume* physSiPMSilicaQ3[MaxPDUCount];
  G4VPhysicalVolume* physSiPMSilicaQ4[MaxPDUCount];

  G4VPhysicalVolume* physTPB[MaxPDUCount];
  G4VPhysicalVolume* physPEN[MaxPDUCount];
  G4VPhysicalVolume* physAcrylic[MaxPDUCount];
};

#endif
