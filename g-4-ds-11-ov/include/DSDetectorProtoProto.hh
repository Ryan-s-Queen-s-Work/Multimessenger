#ifndef DSDetectorProtoProto_H
#define DSDetectorProtoProto_H

#include "G4Ellipsoid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VPhysicalVolume.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"

class DSMaterial;

class DSDetectorProtoProto {

 public:
  DSDetectorProtoProto(G4VPhysicalVolume*);
  ~DSDetectorProtoProto(); 

 private:
  void DefineSurfaces();
  void BuildScintillator(G4double outercryoradius);

  G4VPhysicalVolume* fMotherVolume;
  G4LogicalVolume* fLogicWorld;

  //Physical volumes, required for the optical part
  G4VPhysicalVolume* fLArBath = {nullptr}; //
  G4VPhysicalVolume* fSiPMTop = {nullptr};
  G4VPhysicalVolume* fSiPMBottom = {nullptr};
  G4VPhysicalVolume* fPhysTPBLayer = {nullptr};
  G4VPhysicalVolume* fPhysicActiveLAr = {nullptr};
  G4VPhysicalVolume* fReflectiveCage = {nullptr};
  G4VPhysicalVolume* fLArPocket = {nullptr};
};

#endif
