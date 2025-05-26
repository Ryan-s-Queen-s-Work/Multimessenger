#include "DSDetectorDuneCryostat.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "DSMaterial.hh"
#include "DSLogger.hh"
#include "DSStorage.hh"
#include <iostream>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4UIcommand.hh"
#include "G4UnionSolid.hh"
#include "DSParameters.hh"

using namespace std;

DSDetectorDuneCryostat::DSDetectorDuneCryostat(G4VPhysicalVolume *myMotherVolume) {
  G4Colour  myWhite     (1.0, 1.0, 1.0) ;  // white
  G4Colour  myGray      (0.5, 0.5, 0.5) ;  // gray
  G4Colour  myBlack     (0.0, 0.0, 0.0) ;  // black
  G4Colour  myRed       (1.0, 0.0, 0.0) ;  // red
  G4Colour  myGreen     (0.0, 1.0, 0.0) ;  // green
  G4Colour  myBlue      (0.0, 0.0, 1.0) ;  // blue
  G4Colour  myCyan      (0.0, 1.0, 1.0) ;  // cyan
  G4Colour  myMagenta   (1.0, 0.0, 1.0) ;  // magenta
  G4Colour  myYellow    (1.0, 1.0, 0.0) ;  // yellow
  G4Colour  myLightBlue (0.502, 0.698, 0.99); // lightblue
  G4Colour  myOrange    (1.0, 0.478, 0.0, 0.94); // orange
  G4Colour  myPink      (1.0, 0, 0.937, 0.41); // pink 
  G4VisAttributes* myInvisible = new G4VisAttributes(G4VisAttributes::GetInvisible());
 
  DSLog(routine) << " Constructing Dune Cryostat Geometry" << endlog ;

  myMotherVolume->GetLogicalVolume()  ->SetVisAttributes(myInvisible) ;
  const G4double OVvolume_side = 8548 * mm;
  const G4double OVvolume_height = 7900 * mm;
  const G4double SkinWidth = 14.8 * mm;
  const G4double Ply1Width = 8.8 * mm;
  const G4double Foam1Width = 294.6 * mm;
  const G4double SecdryMembWidth = 0.295 * mm;
  const G4double Foam2Width = 87 * mm;
  const G4double Ply2Width = 22.4 * mm;
  const G4double Foam3Width = 346.8 * mm;
  const G4double Ply3Width = 10.3 * mm;
  const G4double InnerMembWidth = 1.16 * mm;
  myPENThickness = DSStorage::Get()->GetDS20kWLSPENThickness();
  const G4double RoofMembWidth = 2 * mm;
  const G4double RoofFoamHeight = 787 * mm;

  CheckOverlap = DSStorage::Get()->GetCheckOverlap();

  //////////////////////////////////////////////////////////
  //                                                      //
  //             CONSTRUCT CRYOSTAT LAYERS                //
  //                                                      //
  //////////////////////////////////////////////////////////

  // Air mother volume
  G4double side   = OVvolume_side +1.6*m + 1.6*m ;
  G4double height = OVvolume_height +1.6*m + 1.6*m ;

  G4Box* fHallC_Air = new G4Box("HallC_Air_Solid", side / 2., side / 2., height / 2.);
  G4LogicalVolume* fLogicHallC_Air = new G4LogicalVolume(fHallC_Air, DSMaterial::Get()->GetAir(), "HallC_Air_Logic");
  G4VPhysicalVolume* fPhysicHallC_Air = new G4PVPlacement(0,
    G4ThreeVector(0, 0, 20 * cm - (myPENThickness + Ply1Width + Foam1Width + SecdryMembWidth + Foam2Width + Ply2Width + Foam3Width + Ply3Width + InnerMembWidth - RoofFoamHeight - RoofMembWidth) / 2.), //place global origin at center of LAr (origin of SS vessel)
    "LNGS_Basement", fLogicHallC_Air, myMotherVolume, false, 0, CheckOverlap);

  //Test net. Two bar dimensions.
  //
  //       22 / 13.5
  //      _________
  //     |         | 2.8 / 1.2
  //     ----  ----
  //        |  |
  //        |  | 1.7 / 0.8 thickness
  //        |  |
  //        |  |
  //        |  |
  //     ___|  |___
  //    |          |
  //     ----------   61.0 / 27.5 tall

  //increase by 30% the amount of material
  double increase = 1.;

  G4Box* Vertical_I_cap = new G4Box("Vertical_I_cap", increase * 1.4 * cm, 11 * cm, 10756 / 2. * mm);
  G4Box* Vertical_I_body = new G4Box("Vertical_I_up", 61 / 2. * cm, increase * 1.4 * cm, 10756 / 2. * mm);
  G4UnionSolid* solid_half = new G4UnionSolid("Vertical_half", Vertical_I_cap, Vertical_I_body, 0, G4ThreeVector(-30.5 * cm, 0, 0));
  G4UnionSolid* solid_big = new G4UnionSolid("Vertical_big", Vertical_I_cap, solid_half, 0, G4ThreeVector(61 * cm, 0, 0));

  G4Box* Vertical_I_cap_small = new G4Box("Vertical_I_cap_small", increase * 0.6 * cm, 6.5 * cm, 10756 / 2. * mm);
  G4Box* Vertical_I_body_small = new G4Box("Vertical_I_up_small", 27.5 / 2. * cm, increase * 0.6 * cm, 10756 / 2. * mm);
  G4UnionSolid* solid_half_small = new G4UnionSolid("Vertical_half_small", Vertical_I_cap_small, Vertical_I_body_small, 0, G4ThreeVector(-27.5 / 2. * cm, 0, 0));
  G4UnionSolid* solid_small = new G4UnionSolid("Vertical_small", Vertical_I_cap_small, solid_half_small, 0, G4ThreeVector(+27.5 * cm, 0, 0));

  G4RotationMatrix* grot = new G4RotationMatrix;
  grot->rotateX(90 * deg);
  grot->rotateZ(90 * deg);

  G4RotationMatrix* grot2 = new G4RotationMatrix;
  grot2->rotateZ(90 * deg);
  grot2->rotateX(90 * deg);

  G4double small_half_dist = 10.4 / 12. * m;

  fLogicTest = new G4LogicalVolume(solid_big, DSMaterial::Get()->GetStainlessSteel(), "test_steel_beam");
  fLogicTest_sma = new G4LogicalVolume(solid_small, DSMaterial::Get()->GetStainlessSteel(), "test_small_beam");

  // Activate the placement of Additional inter-beam plastic/water shielding
  IsPlaceAdditionalShield = false;

  G4Box* BodyShield = new G4Box("BodyShield", 12.5 * cm, 14.5 * cm, 10756 / 2. * mm);
  G4LogicalVolume* fLogicBodyShield = new G4LogicalVolume(BodyShield, DSMaterial::Get()->GetHDPE(), "logic_body");

  for (int i = 0; i < 22; ++i) {
    if (IsPlaceAdditionalShield) {
      new G4PVPlacement(0, G4ThreeVector(-5.51 * m, -4.5 * m + small_half_dist / 4 + small_half_dist / 2 * i, 0),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//-x

      new G4PVPlacement(0, G4ThreeVector(+5.49 * m, -4.5 * m + small_half_dist / 4 + small_half_dist / 2 * i, 0),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//+x

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist / 4 + small_half_dist / 2 * i, 0, 5.09 * m),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//=z

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist / 4 + small_half_dist / 2 * i, 0, -5.1 * m),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//-z

      new G4PVPlacement(grot2, G4ThreeVector(0, 5.51 * m, -4.65 * m + small_half_dist * 0.97 / 4 + small_half_dist * 0.97 / 2 * i),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//-y

      new G4PVPlacement(grot2, G4ThreeVector(0, -5.49 * m, -4.65 * m + small_half_dist * 0.97 / 4 + small_half_dist * 0.97 / 2 * i),
        "phys_body", fLogicBodyShield, fPhysicHallC_Air, false, 0, CheckOverlap);//+y
    }

    if (i < 12) {
      new G4PVPlacement(0, G4ThreeVector(-5.8 * m, -4.5 * m + small_half_dist * i, 0),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(0, G4ThreeVector(+5.2 * m, -4.5 * m + small_half_dist * i, 0),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist * i, 0, 4.8 * m),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist * i, 0, -5.4 * m),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot2, G4ThreeVector(0, 5.8 * m, -4.65 * m + small_half_dist * i * 0.97),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot2, G4ThreeVector(0, -5.2 * m, -4.65 * m + small_half_dist * i * 0.97),
        "Test_steel_beam", fLogicTest, fPhysicHallC_Air, false, 0, CheckOverlap);
    }

    if (i < 11) {
      new G4PVPlacement(0, G4ThreeVector(-5.8 * m + 15 * cm, -4.5 * m + small_half_dist * (i + 0.5), 0),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(0, G4ThreeVector(+5.2 * m + 15 * cm, -4.5 * m + small_half_dist * (i + 0.5), 0),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist * (i + 0.5), 0, 4.8 * m + 15 * cm),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot, G4ThreeVector(-4.7 * m + small_half_dist * (i + 0.5), 0, -5.4 * m + 16 * cm),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot2, G4ThreeVector(0, 5.8 * m - 15 * cm, -4.65 * m + small_half_dist * (i + 0.5) * 0.97),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);

      new G4PVPlacement(grot2, G4ThreeVector(0, -5.2 * m - 15 * cm, -4.65 * m + small_half_dist * 0.97 * (i + 0.5)),
        "Test_steel_beam_small", fLogicTest_sma, fPhysicHallC_Air, false, 0, CheckOverlap);
    }
  }

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width + 2 * Foam2Width + 2 * SecdryMembWidth + 2 * Foam1Width + 2 * Ply1Width + 2 * SkinWidth;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width + Foam2Width + SecdryMembWidth + Foam1Width + Ply1Width + 2 * SkinWidth;
  fSolidSteelTank = new G4Box("SteelTank_Solid", side / 2., side / 2., height / 2.);
  fLogicSteelTank = new G4LogicalVolume(fSolidSteelTank, DSMaterial::Get()->GetStainlessSteel(), "SteelTank_Logic");
  fPhysicSteelTank = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
    "SteelTank", fLogicSteelTank, fPhysicHallC_Air, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width + 2 * Foam2Width + 2 * SecdryMembWidth + 2 * Foam1Width + 2 * Ply1Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width + Foam2Width + SecdryMembWidth + Foam1Width + Ply1Width;
  fSolidOuterPlyWood = new G4Box("OuterPlyWood_Solid", side / 2., side / 2., height / 2.);
  fLogicOuterPlyWood = new G4LogicalVolume(fSolidOuterPlyWood, DSMaterial::Get()->GetPlyWood(), "OuterPlyWood_Logic");
  fPhysicOuterPlyWood = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
    "OuterPlyWood", fLogicOuterPlyWood, fPhysicSteelTank, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width + 2 * Foam2Width + 2 * SecdryMembWidth + 2 * Foam1Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width + Foam2Width + SecdryMembWidth + Foam1Width;
  fSolidOuterInsulatingFoam = new G4Box("OuterInsulatingFoam_Solid", side / 2., side / 2., height / 2.);
  fLogicOuterInsulatingFoam = new G4LogicalVolume(fSolidOuterInsulatingFoam, DSMaterial::Get()->GetInsulatingFoam(), "OuterInsulatingFoam_Logic");
  fPhysicOuterInsulatingFoam = new G4PVPlacement(0, G4ThreeVector(0, 0, Ply1Width / 2.), 
    "OuterInsulatingFoam", fLogicOuterInsulatingFoam, fPhysicOuterPlyWood, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width + 2 * Foam2Width + 2 * SecdryMembWidth;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width + Foam2Width + SecdryMembWidth;
  fSolidSecondaryMembrane = new G4Box("SecondaryMembrane_Solid", side / 2., side / 2., height / 2.);
  fLogicSecondaryMembrane = new G4LogicalVolume(fSolidSecondaryMembrane, DSMaterial::Get()->GetAluminum(), "SecondaryMembrane_Logic");
  fPhysicSecondaryMembrane = new G4PVPlacement(0, G4ThreeVector(0, 0, Foam1Width / 2.), 
    "SecondaryMembrane", fLogicSecondaryMembrane, fPhysicOuterInsulatingFoam, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width + 2 * Foam2Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width + Foam2Width;
  fSolidInsulatingFoam2 = new G4Box("InsulatingFoam_Solid", side / 2., side / 2., height / 2.);
  fLogicInsulatingFoam2 = new G4LogicalVolume(fSolidInsulatingFoam2, DSMaterial::Get()->GetInsulatingFoam(), "InsulatingFoam_Logic");
  fPhysicInsulatingFoam2 = new G4PVPlacement(0, G4ThreeVector(0, 0, SecdryMembWidth / 2.), 
    "InsulatingFoam2", fLogicInsulatingFoam2, fPhysicSecondaryMembrane, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width + 2 * Ply2Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width + Ply2Width;
  fSolidPlyWood2 = new G4Box("PlyWood_Solid", side / 2., side / 2., height / 2.);
  fLogicPlyWood2 = new G4LogicalVolume(fSolidPlyWood2, DSMaterial::Get()->GetPlyWood(), "PlyWood_Logic");
  fPhysicPlyWood2 = new G4PVPlacement(0, G4ThreeVector(0, 0, Foam2Width / 2.), 
    "PlyWood2", fLogicPlyWood2, fPhysicInsulatingFoam2, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width + 2 * Foam3Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width + Foam3Width;
  fSolidInsulatingFoam3 = new G4Box("InsulatingFoam_Solid", side / 2., side / 2., height / 2.);
  fLogicInsulatingFoam3 = new G4LogicalVolume(fSolidInsulatingFoam3, DSMaterial::Get()->GetInsulatingFoam(), "InsulatingFoam_Logic");
  fPhysicInsulatingFoam3 = new G4PVPlacement(0, G4ThreeVector(0, 0, Ply2Width / 2.), 
    "InsulatingFoam3", fLogicInsulatingFoam3, fPhysicPlyWood2, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth + 2 * Ply3Width;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth + RoofFoamHeight + Ply3Width;
  fSolidPlyWood3 = new G4Box("PlyWood_Solid", side / 2., side / 2., height / 2.);
  fLogicPlyWood3 = new G4LogicalVolume(fSolidPlyWood3, DSMaterial::Get()->GetPlyWood(), "PlyWood_Logic");
  fPhysicPlyWood3 = new G4PVPlacement(0, G4ThreeVector(0, 0, (Foam3Width) / 2.), 
    "PlyWood3", fLogicPlyWood3, fPhysicInsulatingFoam3, false, 0, CheckOverlap);

  side = OVvolume_side;
  height = RoofFoamHeight * mm;
  fSolidInsulatingFoamRoof = new G4Box("InsulatingFoam_SolidRoof", side / 2., side / 2., height / 2.);
  fLogicInsulatingFoamRoof = new G4LogicalVolume(fSolidInsulatingFoamRoof, DSMaterial::Get()->GetInsulatingFoamRoof(), "InsulatingFoam_LogicRoof");
  fPhysicInsulatingFoamRoof = new G4PVPlacement(0, G4ThreeVector(0, 0, (OVvolume_height + InnerMembWidth + RoofMembWidth + Ply3Width) / 2.), 
    "InsulatingFoamRoof", fLogicInsulatingFoamRoof, fPhysicPlyWood3, false, 0, CheckOverlap);

  side = OVvolume_side + 2 * InnerMembWidth;
  height = OVvolume_height + InnerMembWidth + RoofMembWidth;
  fSolidMembrane = new G4Box("Membrane_Solid", side / 2., side / 2., height / 2.);
  fLogicMembrane = new G4LogicalVolume(fSolidMembrane, DSMaterial::Get()->GetStainlessSteel(), "Membrane_Logic");
  fPhysicMembrane = new G4PVPlacement(0, G4ThreeVector(0, 0, (Ply3Width - RoofFoamHeight) / 2.), 
    "Membrane", fLogicMembrane, fPhysicPlyWood3, false, 0, CheckOverlap);

  isPENonCryo = DSStorage::Get()->GetDS20kPENonCryo();
  isPENonGAr = DSStorage::Get()->GetDS20kPENonGAr();
  if (isPENonCryo) { //Default
    side = OVvolume_side;
    height = OVvolume_height;
    fSolidPEN = new G4Box("PENmuonveto_Solid", side / 2., side / 2., height / 2.);
    fLogicPEN = new G4LogicalVolume(fSolidPEN, DSMaterial::Get()->GetPEN(), "PENmuonveto_Logic");
    fPhysicPEN = new G4PVPlacement(0, G4ThreeVector(0, 0, -(RoofMembWidth - InnerMembWidth) / 2.), 
      "PENmuonvteo", fLogicPEN, fPhysicMembrane, false, 0, CheckOverlap);

    side = OVvolume_side - 2 * myPENThickness;
    height = OVvolume_height - 400 * mm - myPENThickness;
    fSolidVetoArgon = new G4Box("VetoArgon_Solid", side / 2., side / 2., height / 2.);
    fLogicVetoArgon = new G4LogicalVolume(fSolidVetoArgon, DSMaterial::Get()->GetOVLiquidArgon(), "VetoArgon_Logic");
    fPhysicVetoArgon = new G4PVPlacement(0, G4ThreeVector(0, 0, -200 * mm + myPENThickness / 2.), 
      "VetoArgon", fLogicVetoArgon, fPhysicPEN, false, 0, CheckOverlap);

    if (isPENonGAr) {
      side = OVvolume_side - 2 * myPENThickness;
      height = 400 * mm - myPENThickness;
      fSolidArgonVapor = new G4Box("ArgonVapor_Solid", side / 2., side / 2., height / 2.);
      fLogicArgonVapor = new G4LogicalVolume(fSolidArgonVapor, DSMaterial::Get()->GetGaseousArgon(), "ArgonVapor_Logic");
      fPhysicArgonVapor = new G4PVPlacement(0, G4ThreeVector(0, 0, (OVvolume_height - height) / 2 * mm - myPENThickness),
        "ArgonVapor", fLogicArgonVapor, fPhysicPEN, false, 0, CheckOverlap);
    }
    else {
      side = OVvolume_side;
      height = 400 * mm;
      fSolidArgonVapor = new G4Box("ArgonVapor_Solid", side / 2., side / 2., height / 2.);
      fLogicArgonVapor = new G4LogicalVolume(fSolidArgonVapor, DSMaterial::Get()->GetGaseousArgon(), "ArgonVapor_Logic");
      fPhysicArgonVapor = new G4PVPlacement(0, G4ThreeVector(0, 0, (OVvolume_height - height) / 2 * mm),
        "ArgonVapor", fLogicArgonVapor, fPhysicPEN, false, 0, CheckOverlap);
    }
  }
  else {
    side = OVvolume_side;
    height = OVvolume_height - 400 * mm;
    fSolidVetoArgon = new G4Box("VetoArgon_Solid", side / 2., side / 2., height / 2.);
    fLogicVetoArgon = new G4LogicalVolume(fSolidVetoArgon, DSMaterial::Get()->GetOVLiquidArgon(), "VetoArgon_Logic");
    fPhysicVetoArgon = new G4PVPlacement(0, G4ThreeVector(0, 0, -200 * mm), 
      "VetoArgon", fLogicVetoArgon, fPhysicMembrane, false, 0, CheckOverlap);

    side = OVvolume_side;
    height = 400 * mm;
    fSolidArgonVapor = new G4Box("ArgonVapor_Solid", side / 2., side / 2., height / 2.);
    fLogicArgonVapor = new G4LogicalVolume(fSolidArgonVapor, DSMaterial::Get()->GetGaseousArgon(), "ArgonVapor_Logic");
    fPhysicArgonVapor = new G4PVPlacement(0, G4ThreeVector(0, 0, (OVvolume_height - height) / 2 * mm),
      "ArgonVapor", fLogicArgonVapor, fPhysicMembrane, false, 0, CheckOverlap);

    if (isPENonGAr) DSLog(routine) << "No PEN on Cryostat has been chosen, but PENonGAr is set to 1. PENonGAr has not been applied!!!" << endlog;
  }

  fLogicArgonVapor              ->SetVisAttributes(myInvisible) ; // myYellow
  fLogicVetoArgon               ->SetVisAttributes(myLightBlue) ; // myLightBlue
  if (isPENonCryo) fLogicPEN    ->SetVisAttributes(myInvisible) ; // myGreen
  //fLogicMembrane                ->SetVisAttributes(myInvisible) ;
  fLogicInsulatingFoamRoof      ->SetVisAttributes(myInvisible) ; // myRed
  fLogicPlyWood3                ->SetVisAttributes(myInvisible) ;
  fLogicInsulatingFoam3         ->SetVisAttributes(myInvisible) ; // myGreen
  fLogicPlyWood2                ->SetVisAttributes(myInvisible) ;
  fLogicInsulatingFoam2         ->SetVisAttributes(myInvisible) ; // myGreen
  fLogicSecondaryMembrane       ->SetVisAttributes(myInvisible) ; // myGray
  fLogicOuterInsulatingFoam     ->SetVisAttributes(myInvisible) ; // myWhite
  fLogicOuterPlyWood            ->SetVisAttributes(myInvisible) ; // myYellow
  fLogicSteelTank               ->SetVisAttributes(myWhite) ; //myWhite
  fLogicBodyShield              ->SetVisAttributes(myBlue);
  fLogicTest                    ->SetVisAttributes(myRed); // myRed
  fLogicTest_sma                ->SetVisAttributes(myYellow); // myYellow
  fLogicHallC_Air           ->SetVisAttributes(myInvisible) ;

  //////////////////////////////////////////////////////////
  //                                                      //
  //             CONSTRUCT AND PLACE OV PDUs              //
  //                                                      //
  //////////////////////////////////////////////////////////

  DSLog(routine) << " Constructing Outer/Muon Veto PDU Geometry" << endlog;

  // Volume Numbers
  volumeNumber_bulb = 11000;
  volumeNumber_base = 12000;
  volumeNumber_photocathode = 13000;
  volumeNumber_tpb = 20000;
  volumeNumber_pen = 20000;
  volumeNumber_acrylic = 30000;

  // Load Materials 
  teflon_mat = DSMaterial::Get()->GetTeflon();
  stainlesssteel_mat = DSMaterial::Get()->GetStainlessSteel();
  metalsilicon_mat = DSMaterial::Get()->GetMetalSilicon();

  PDUWLSMaterial = DSStorage::Get()->GetDS20kCryoPDUWLS();
  HDPEShieldThickness = DSStorage::Get()->GetDS20kHDPEShellThickness();

  // SiPM dimensions
  SiPMSide = 20.2 * cm; //DSStorage::Get()->GetDS20kSiPMSide(); // SiPMSide = 20.2*cm
  SiPMHeight = DSStorage::Get()->GetDS20kSiPMHeight(); // SiPMHeight = 0.5*cm
  quadrantoffset = SiPMSide / 4. - 0.05 * cm;
  myTPBThickness = DSStorage::Get()->GetDS20kTPBThickness();
  myAcrylicThickness = DSStorage::Get()->GetDS20kPDUAcrylicThickness();

  // SiPM faces the -z axis
  MVSiPMOnCap = new G4Box("MVSiPMOnCap", SiPMSide / 2., SiPMSide / 2., SiPMHeight / 2.);
  MVSiPMActiveOnCap = new G4Box("MVSiPMActiveOnCap", SiPMSide / 2. - 0.1 * cm, SiPMSide / 2. - 0.1 * cm, 0.2 * cm);
  MVSiPMSilicaOnCap = new G4Box("MVSiPMSilicaOnCap", SiPMSide / 4. - 0.05 * cm, SiPMSide / 4. - 0.05 * cm, 300. * micrometer); // SiPM Silica volume corresponds to single quadrant of PDU

  // TPB/PEN 
  MVSolidTPB = new G4Box("MVSolidTPB", SiPMSide / 2., SiPMSide / 2., myTPBThickness / 2.);
  fLogicTPB = new G4LogicalVolume(MVSolidTPB, DSMaterial::Get()->GetTPB(), "MV_TPB_Logic");
  fLogicTPB->SetVisAttributes(myInvisible); // myBlue

  MVSolidPEN = new G4Box("MVSolidPEN", SiPMSide / 2., SiPMSide / 2., myPENThickness / 2.);
  fLogicPENonSiPM = new G4LogicalVolume(MVSolidPEN, DSMaterial::Get()->GetPEN(), "MV_PEN_Logic");
  fLogicPENonSiPM->SetVisAttributes(myInvisible); // myGreen

  MVSolidAcrylic = new G4Box("MVSolidAcrylic", SiPMSide / 2., SiPMSide / 2., myAcrylicThickness / 2.);
  fLogicAcrylic = new G4LogicalVolume(MVSolidAcrylic, DSMaterial::Get()->GetAcrylic(), "MV_Acrylic_Logic");
  fLogicAcrylic->SetVisAttributes(myYellow); //myYellow

  //Set the index of the cathode material
  DSStorage::Get()->SetMuPMTMaterialIndex(metalsilicon_mat->GetIndex());
  DSLog(routine) << "OV Photosensor type: SiPM.  Material of photosensor: " << metalsilicon_mat << endlog;


  AcrylicOffset = SiPMHeight / 2. + myAcrylicThickness / 2. + 0.1 * mm;
  PENOffset = SiPMHeight / 2. + myPENThickness / 2. + 0.1 * mm;
  TPBOffset = AcrylicOffset + myAcrylicThickness / 2. + myTPBThickness / 2.;

  // PDU index counters
  PDU_counter = 0, physSiPMIndex = 0;


  //////////////////////////////////////////////////
  //                                              //
  //            Place HDPE Shield PDUs            //
  //                                              //
  //////////////////////////////////////////////////
  int HDPEShield_PDU_Config = DSStorage::Get()->GetDS20kHDPEShieldPDUConfig();
  PDU_wall_gap = 5 * cm;

  if (HDPEShield_PDU_Config > 0) {
    // HDPE PDU configuration selection
    
    PDUMotherVolume = fPhysicVetoArgon;
    const double R_HDPEShield = 2380 * mm + 5 * cm + 2 * cm + HDPEShieldThickness;
    const double H_HDPEShield = 4500 * mm;
    const double TopCapH_HDPEShield = 840 * mm + HDPEShieldThickness;
    const double CapAngle = atan((TopCapH_HDPEShield - HDPEShieldThickness) / (R_HDPEShield - 60* cm)); // 60 cm = cap radius
    int num_rings = 3;
    double z_initial_hdpe = -H_HDPEShield / 3.;
    Top_Ring_Radius = R_HDPEShield / sqrt(2);
    if (HDPEShield_PDU_Config == 1) {
      DSLog(debugging) << "Placing PDUs on HDPE shield. 18 Wall PDUs, 7 Top, 7 Bottom PDUs" << endlog;
      int num_in_each_ring = 6;

      //////////////////////////////////////
      //            HDPE WALLS            //
      //////////////////////////////////////
      for (int ring = 0; ring < num_rings; ring++) {
        zCoord_PDU_base = z_initial_hdpe + ring * H_HDPEShield / 3.;
        theta = (ring % 2 == 0) ? M_PI / 6 : 0; //M_PI / 12
        DSLog(debugging) << "Placing PDUs on HDPE wall ring at z = " << zCoord_PDU_base << endlog;
        for (int i = 0; i < num_in_each_ring; i++) {
          xCoord_PDU_base = (R_HDPEShield + PDU_wall_gap) * cos(theta);
          yCoord_PDU_base = (R_HDPEShield + PDU_wall_gap) * sin(theta);

          RotationPDU = new G4RotationMatrix();
          RotationPDU->rotateY(M_PI / 2);
          RotationPDU->rotateX(-theta);

          PlaceSiPM();

          theta += M_PI / 3;
        }
      }

      //////////////////////////////////////
      //             HDPE Top             //
      //////////////////////////////////////
      
      zCoord_PDU_base = H_HDPEShield / 2 + TopCapH_HDPEShield * (R_HDPEShield - Top_Ring_Radius) / (R_HDPEShield - 60 * cm) + PDU_wall_gap * cos(CapAngle);
      
      theta = 0;
      DSLog(debugging) << "Placing PDUs on HDPE top" << endlog;
      for (int i = 0; i < num_in_each_ring; i++) {
        xCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * cos(theta);
        yCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * sin(theta);

        RotationPDU = new G4RotationMatrix();
        RotationPDU->rotateX(M_PI); //M_PI
        RotationPDU->rotateZ(theta);
        RotationPDU->rotateY(CapAngle);

        PlaceSiPM();

        theta += M_PI / 3;
      }
      /////////Center/////////
      xCoord_PDU_base = 0 * mm;
      yCoord_PDU_base = 0 * mm;
      zCoord_PDU_base = H_HDPEShield / 2 + TopCapH_HDPEShield + PDU_wall_gap;
      RotationPDU = new G4RotationMatrix();
      RotationPDU->rotateX(M_PI);

      PlaceSiPM();

      //////////////////////////////////////
      //            HDPE Bottom           //
      //////////////////////////////////////
      zCoord_PDU_base = -(H_HDPEShield / 2 + TopCapH_HDPEShield * (R_HDPEShield - Top_Ring_Radius) / (R_HDPEShield - 60 * cm) + PDU_wall_gap * cos(CapAngle));
      theta = 0;
      DSLog(debugging) << "Placing PDUs on HDPE bottom" << endlog;
      for (int i = 0; i < num_in_each_ring; i++) {
        xCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * cos(theta);
        yCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * sin(theta);

        RotationPDU = new G4RotationMatrix();
        RotationPDU->rotateZ(-theta);
        RotationPDU->rotateY(CapAngle);

        PlaceSiPM();

        theta += M_PI / 3;
      }
      /////////Center/////////
      xCoord_PDU_base = 0 * mm;
      yCoord_PDU_base = 0 * mm;
      zCoord_PDU_base = -(H_HDPEShield / 2 + TopCapH_HDPEShield + PDU_wall_gap);
      RotationPDU = new G4RotationMatrix();

      PlaceSiPM();
    }
    else if (HDPEShield_PDU_Config == 2) {
      DSLog(debugging) << "Placing PDUs on HDPE shield. 24 Wall PDUs, 8 Bottom PDUs" << endlog;
      //////////////////////////////////////
      //            HDPE WALLS            //
      //////////////////////////////////////

      int num_in_each_ring = 8;
      theta = M_PI / 6;
      
      for (int ring = 0; ring < num_rings; ring++) {
        zCoord_PDU_base = z_initial_hdpe + ring * H_HDPEShield / 3.;
        DSLog(debugging) << "Placing PDUs on HDPE wall ring at z = " << zCoord_PDU_base << endlog;
        for (int i = 0; i < num_in_each_ring; i++) {
          xCoord_PDU_base = (R_HDPEShield + PDU_wall_gap) * cos(theta);
          yCoord_PDU_base = (R_HDPEShield + PDU_wall_gap) * sin(theta);

          RotationPDU = new G4RotationMatrix();
          RotationPDU->rotateY(M_PI / 2);
          RotationPDU->rotateX(-theta);

          PlaceSiPM();

          if (i%2 == 0) theta += M_PI / 6;
          else theta += M_PI / 3;
        }
        theta += M_PI / 6;
      }

      //////////////////////////////////////
      //            HDPE Bottom           //
      //////////////////////////////////////
      zCoord_PDU_base = -(H_HDPEShield / 2 + TopCapH_HDPEShield * (R_HDPEShield - Top_Ring_Radius) / (R_HDPEShield - 60 * cm) + PDU_wall_gap * cos(CapAngle));
      num_in_each_ring = 6;
      theta = 0;
      DSLog(debugging) << "Placing PDUs on HDPE bottom" << endlog;

      for (int i = 0; i < num_in_each_ring; i++) {
        xCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * cos(theta);
        yCoord_PDU_base = (Top_Ring_Radius + PDU_wall_gap * sin(CapAngle)) * sin(theta);

        RotationPDU = new G4RotationMatrix();
        RotationPDU->rotateZ(-theta);
        RotationPDU->rotateY(CapAngle);

        PlaceSiPM();

        theta += M_PI / 3;
      }

      /////////Center/////////
      xCoord_PDU_base = 0 * mm;
      yCoord_PDU_base = 30 * cm;
      zCoord_PDU_base = -(H_HDPEShield / 2 + TopCapH_HDPEShield + PDU_wall_gap);

      RotationPDU = new G4RotationMatrix();

      PlaceSiPM();

      yCoord_PDU_base = -30 * cm;

      PlaceSiPM();
    }

  }//PDUs on SSV
  DSLog(debugging) << "Defining Surfaces..." << endlog;
  DefineSurfaces();
}


DSDetectorDuneCryostat::~DSDetectorDuneCryostat(){
  ; //delete fMessenger;
}


void DSDetectorDuneCryostat::DefineSurfaces() {
  // Define ESR
  G4double TREFVIS = DSParameters::Get()->GetTeflonTPBVisRef();
  G4double myvalue, myene, TeflonTPBENE[831], TeflonTPBREF[831];
  G4int dim = 0;

  ifstream fESR_reflectivity("../data/detector/ESR_reflectivity_2_AstroCent.dat");

  if (!fESR_reflectivity.is_open())
    DSLog(fatal) << "ERROR: Could not open ESR reflectivity file" << endlog;
  while (!fESR_reflectivity.eof()) {
    fESR_reflectivity >> myene >> myvalue;
    if (fESR_reflectivity.eof()) break;
    TeflonTPBENE[dim] = myene * eV;
    TeflonTPBREF[dim] = myvalue;
    dim++;
  }
  fESR_reflectivity.close();

  for (int i = 0; i < dim; i++) TeflonTPBREF[i] *= TREFVIS / 100.;

  ///////////////////////////////////////
  //  PEN on Cryostat
  ///////////////////////////////////////

  isPENonCryo = DSStorage::Get()->GetDS20kPENonCryo();

  if (isPENonCryo) {
    fOpSurfacePENCryo = new G4OpticalSurface("OpSPENonCryo");
    fOpSurfacePENCryo->SetType(dielectric_dielectric);
    fOpSurfacePENCryo->SetModel(unified);
    fOpSurfacePENCryo->SetFinish(ground);//groundfrontpainted
    //fOpSurfacePENCryo->SetSigmaAlpha(0.1); //new alpha
    fOpSurfacePENCryo->SetMaterialPropertiesTable(DSMaterial::Get()->GetmyPEN());

    new G4LogicalBorderSurface("CryoPENOut",
      fPhysicPEN,
      fPhysicVetoArgon,
      fOpSurfacePENCryo);

    new G4LogicalBorderSurface("CryoPENIn",
      fPhysicVetoArgon,
      fPhysicPEN,
      fOpSurfacePENCryo);

    new G4LogicalBorderSurface("CryoPENOutVapor",
      fPhysicPEN,
      fPhysicArgonVapor,
      fOpSurfacePENCryo);

    new G4LogicalBorderSurface("CryoPENInVapor",
      fPhysicArgonVapor,
      fPhysicPEN,
      fOpSurfacePENCryo);
  }

  DSLog(development) << "Defining PDU Surfaces..." << endlog;

  //Define Op LAr SiPM Surface
  G4OpticalSurface* fOpNewLArSiPMSurface = new G4OpticalSurface("OpNewLArSiPMSurface");
  fOpNewLArSiPMSurface->SetType(dielectric_dielectric);
  fOpNewLArSiPMSurface->SetModel(glisur);
  fOpNewLArSiPMSurface->SetFinish(polished);
  G4MaterialPropertiesTable* fNewLArSiPMSurfProp = new G4MaterialPropertiesTable();
  fNewLArSiPMSurfProp->AddConstProperty("DONewLArSiPM", 1, true);
  fOpNewLArSiPMSurface->SetMaterialPropertiesTable(fNewLArSiPMSurfProp);

  //Define surface for each PDU quadrant
  for (int i = 0; i < MaxPDUCount; i++) {
    new G4LogicalBorderSurface("Argon-SiPMQ1",
      fPhysicVetoArgon,
      physSiPMSilicaQ1[i],
      fOpNewLArSiPMSurface);

    new G4LogicalBorderSurface("Argon-SiPMQ2",
      fPhysicVetoArgon,
      physSiPMSilicaQ2[i],
      fOpNewLArSiPMSurface);

    new G4LogicalBorderSurface("Argon-SiPMQ3",
      fPhysicVetoArgon,
      physSiPMSilicaQ3[i],
      fOpNewLArSiPMSurface);

    new G4LogicalBorderSurface("Argon-SiPMQ4",
      fPhysicVetoArgon,
      physSiPMSilicaQ4[i],
      fOpNewLArSiPMSurface);
  }

  DSLog(development) << "PDU Surfaces Defined" << endlog;


  // Add TPB on cryostat and SS Vessel photosensors 
  PDUWLSMaterial = DSStorage::Get()->GetDS20kCryoPDUWLS();
  if (PDUWLSMaterial == 1) {
    fOpTPBSurface = new G4OpticalSurface("OpTBPSurface");

    fOpTPBSurface->SetType(dielectric_dielectric);
    fOpTPBSurface->SetModel(unified);
    fOpTPBSurface->SetFinish(ground);
    fOpTPBSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetmyTPB());

    for (int i = 0; i < MaxPDUCount; i++) {
      new G4LogicalBorderSurface("TPBSurface",
        fPhysicVetoArgon,
        physTPB[i],
        fOpTPBSurface);

      new G4LogicalBorderSurface("TPBSurface",
        fPhysicVetoArgon,
        physTPB[i],
        fOpTPBSurface);
      // must handle TPB-to-Ar transmission
    }
    DSLog(development) << "Cryo TPB/PEN Surfaces Defined" << endlog;
  }

  //Add PEN on cryostat and SS Vessel photosensors
  if (PDUWLSMaterial == 2) {
    fOpPENSurface = new G4OpticalSurface("OpPENSurface");
    fOpPENSurface->SetType(dielectric_dielectric);
    fOpPENSurface->SetModel(unified);
    fOpPENSurface->SetFinish(ground);//groundfrontpainted
    //fOpSurfacePENCryo->SetSigmaAlpha(0.1); //new alpha
    fOpPENSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetmyPEN());

    for (int i = 0; i < MaxPDUCount; i++) {
      //sides
      new G4LogicalBorderSurface("PENSurface",
        fPhysicVetoArgon,
        physPEN[i],
        fOpPENSurface);
      // need to handle transmission at PEN-to-Ar
    }
  }


  // Add reflector on cryostat 
  int ReflectorMaterial = DSStorage::Get()->GetDS20kCryoReflectorMaterial();
  if (ReflectorMaterial == 0) {
    DSLog(routine) << "Adding reflector. Material: EPS" << endlog;

    fEPSSSurface = new G4OpticalSurface("EPSSSurface");
    fEPSSSurface->SetType(dielectric_metal);
    fEPSSSurface->SetModel(unified);
    fEPSSSurface->SetFinish(ground);
    fEPSSSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetElectropolishedStainlessSteelMPT());

    if (isPENonCryo) {
      new G4LogicalBorderSurface("CryoSurfaceEPSS",
        fPhysicPEN,
        fPhysicMembrane,
        fEPSSSurface);
    }
    else {
      new G4LogicalBorderSurface("CryoSurfaceEPSS",
        fPhysicVetoArgon,
        fPhysicMembrane,
        fEPSSSurface);
    }
  }
  else if (ReflectorMaterial == 1) { // Lumirror
    DSLog(routine) << "Adding reflector. Material: Lumirror" << endlog;

    fOpLumirrorSurface = new G4OpticalSurface("OpLumirrorSurface");
    fOpLumirrorSurface->SetType(dielectric_metal);
    fOpLumirrorSurface->SetModel(unified);
    fOpLumirrorSurface->SetFinish(ground);
    fOpLumirrorSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetLumirrorMPT());

    //fPhysicPEN->G4VPhysicalVolume::GetLogicalVolume()->SetVisAttributes(myLightBlue);

    new G4LogicalBorderSurface("CryostatInnerSurface",
      fPhysicPEN,
      fPhysicMembrane,
      fOpLumirrorSurface);
  }
  else if (ReflectorMaterial == 2) { // Tyvek
    DSLog(routine) << "Adding reflector. Material: Tyvek" << endlog;

    fOpTyvekSurface = new G4OpticalSurface("OpTyvekSurface");
    fOpTyvekSurface->SetType(dielectric_metal);
    fOpTyvekSurface->SetModel(unified);
    fOpTyvekSurface->SetFinish(ground);
    fOpTyvekSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetTyvekMPT());

    //fPhysicPEN->G4VPhysicalVolume::GetLogicalVolume()->SetVisAttributes(myInvisible); //myYellow);

    if (isPENonCryo) {
      new G4LogicalBorderSurface("CryostatInnerSurface",
        fPhysicPEN,
        fPhysicMembrane,
        fOpTyvekSurface);
    }
    else {
      new G4LogicalBorderSurface("CryostatInnerSurface",
        fPhysicVetoArgon,
        fPhysicMembrane,
        fOpTyvekSurface);
    }
  }
  else if (ReflectorMaterial == 3) { // ESR

    fOpESRSurface = new G4OpticalSurface("OpESRSurface");
    fOpESRSurface->SetType(dielectric_metal);
    fOpESRSurface->SetModel(unified);
    fOpESRSurface->SetFinish(polished);
    G4MaterialPropertiesTable* fOpESRSurfProp = new G4MaterialPropertiesTable();
    fOpESRSurfProp->AddProperty("REFLECTIVITY", TeflonTPBENE, TeflonTPBREF, dim);
    fOpESRSurface->SetMaterialPropertiesTable(fOpESRSurfProp);

    if (isPENonCryo) {
      new G4LogicalBorderSurface("CryostatInnerSurface",
        fPhysicPEN,
        fPhysicMembrane,
        fOpESRSurface);
    }
    else {
      new G4LogicalBorderSurface("CryostatInnerSurface",
        fPhysicVetoArgon,
        fPhysicMembrane,
        fOpESRSurface);
    }
  }

  //add ElectropolishedStainlessSteel (no effect if PENonGAr)
  fEPSSSurface = new G4OpticalSurface("EPSSSurface");
  fEPSSSurface->SetType(dielectric_metal);
  fEPSSSurface->SetModel(unified);
  fEPSSSurface->SetFinish(ground);
  fEPSSSurface->SetMaterialPropertiesTable(DSMaterial::Get()->GetElectropolishedStainlessSteelMPT());

  new G4LogicalBorderSurface("CryoSurfaceEPSS",
    fPhysicArgonVapor,
    fPhysicMembrane,
    fEPSSSurface);

  DSLog(routine) << "All surfaces defined." << endlog;
}

void DSDetectorDuneCryostat::PlaceSiPM() {
  G4Colour  myRed(1.0, 0.0, 0.0);  // red
  G4Colour  myGreen(0.0, 1.0, 0.0);  // green
  G4Colour  myBlue(0.0, 0.0, 1.0);  // blue
  G4Colour  myMagenta(1.0, 0.0, 1.0);  // magenta
  G4VisAttributes* myInvisible = new G4VisAttributes(G4VisAttributes::GetInvisible());
  number = G4UIcommand::ConvertToString(PDU_counter);

  myLogicShapeSiPM = new G4LogicalVolume(MVSiPMOnCap, teflon_mat, "MVSiPM" + number);
  myLogicShapeSiPMActive = new G4LogicalVolume(MVSiPMActiveOnCap, metalsilicon_mat, "MVSiPMActive" + number);
  myLogicShapeSiPMSilicaQ1 = new G4LogicalVolume(MVSiPMSilicaOnCap, metalsilicon_mat, "MVSiPMSilica" + number + "Q01");
  myLogicShapeSiPMSilicaQ2 = new G4LogicalVolume(MVSiPMSilicaOnCap, metalsilicon_mat, "MVSiPMSilica" + number + "Q02");
  myLogicShapeSiPMSilicaQ3 = new G4LogicalVolume(MVSiPMSilicaOnCap, metalsilicon_mat, "MVSiPMSilica" + number + "Q03");
  myLogicShapeSiPMSilicaQ4 = new G4LogicalVolume(MVSiPMSilicaOnCap, metalsilicon_mat, "MVSiPMSilica" + number + "Q04");

  myLogicShapeSiPM->SetVisAttributes(myInvisible);
  myLogicShapeSiPMActive->SetVisAttributes(myInvisible);
  myLogicShapeSiPMSilicaQ1->SetVisAttributes(myRed);
  myLogicShapeSiPMSilicaQ2->SetVisAttributes(myGreen);
  myLogicShapeSiPMSilicaQ3->SetVisAttributes(myBlue);
  myLogicShapeSiPMSilicaQ4->SetVisAttributes(myMagenta);

  physSiPM[physSiPMIndex] = new G4PVPlacement(RotationPDU,
    G4ThreeVector(xCoord_PDU_base, yCoord_PDU_base, zCoord_PDU_base),
    "OVPDUbase_" + number, myLogicShapeSiPM, PDUMotherVolume,
    true, volumeNumber_base + PDU_counter, CheckOverlap);

  DSLog(debugging) << physSiPM[physSiPMIndex]->GetName() << " placed vertically at " << xCoord_PDU_base << ", " << yCoord_PDU_base << ", " << zCoord_PDU_base << endlog;

  physSiPMActive[physSiPMIndex] = new G4PVPlacement(0,
    G4ThreeVector(0, 0, -SiPMHeight / 2. + 0.2 * cm),
    "OVPDUactive_" + number, myLogicShapeSiPMActive, physSiPM[physSiPMIndex],
    true, volumeNumber_bulb + PDU_counter, CheckOverlap);

  // quadrant 1 in upper left; 2 in upper right; 3 in lower right; 4 in lower right
  physSiPMSilicaQ1[physSiPMIndex] = new G4PVPlacement(0,
    G4ThreeVector(quadrantoffset, -quadrantoffset, -0.2 * cm + 300 * micrometer),
    "OVPDU_" + G4UIcommand::ConvertToString(PDU_counter * 4), myLogicShapeSiPMSilicaQ1, physSiPMActive[physSiPMIndex],
    true, volumeNumber_photocathode + (PDU_counter) * 4, CheckOverlap);

  physSiPMSilicaQ2[physSiPMIndex] = new G4PVPlacement(0,
    G4ThreeVector(quadrantoffset, quadrantoffset, -0.2 * cm + 300 * micrometer),
    "OVPDU_" + G4UIcommand::ConvertToString(PDU_counter * 4 + 1), myLogicShapeSiPMSilicaQ2, physSiPMActive[physSiPMIndex],
    true, volumeNumber_photocathode + (PDU_counter) * 4 + 1, CheckOverlap);

  physSiPMSilicaQ3[physSiPMIndex] = new G4PVPlacement(0,
    G4ThreeVector(-quadrantoffset, quadrantoffset, -0.2 * cm + 300 * micrometer),
    "OVPDU_" + G4UIcommand::ConvertToString(PDU_counter * 4 + 2), myLogicShapeSiPMSilicaQ3, physSiPMActive[physSiPMIndex],
    true, volumeNumber_photocathode + (PDU_counter) * 4 + 2, CheckOverlap);

  physSiPMSilicaQ4[physSiPMIndex] = new G4PVPlacement(0,
    G4ThreeVector(-quadrantoffset, -quadrantoffset, -0.2 * cm + 300 * micrometer),
    "OVPDU_" + G4UIcommand::ConvertToString(PDU_counter * 4 + 3), myLogicShapeSiPMSilicaQ4, physSiPMActive[physSiPMIndex],
    true, volumeNumber_photocathode + (PDU_counter) * 4 + 3, CheckOverlap);

  if (PDUWLSMaterial == 1) {
    physAcrylic[physSiPMIndex] = new G4PVPlacement(RotationPDU,
      G4ThreeVector(xCoord_PDU_base, yCoord_PDU_base, zCoord_PDU_base) + G4ThreeVector(0, 0, -AcrylicOffset).rotate(RotationPDU->axis(), -RotationPDU->delta()),
      "Acrylic_" + number, fLogicAcrylic, PDUMotherVolume,
      true, volumeNumber_acrylic + PDU_counter, CheckOverlap);

    physTPB[physSiPMIndex] = new G4PVPlacement(RotationPDU,
      G4ThreeVector(xCoord_PDU_base, yCoord_PDU_base, zCoord_PDU_base) + G4ThreeVector(0, 0, -TPBOffset).rotate(RotationPDU->axis(), -RotationPDU->delta()),
      "TPB_" + number, fLogicTPB, PDUMotherVolume,
      true, volumeNumber_tpb + PDU_counter, CheckOverlap);
  }
  else if (PDUWLSMaterial == 2) {
    physPEN[physSiPMIndex] = new G4PVPlacement(RotationPDU,
      G4ThreeVector(xCoord_PDU_base, yCoord_PDU_base, zCoord_PDU_base) + G4ThreeVector(0, 0, -PENOffset).rotate(RotationPDU->axis(), -RotationPDU->delta()),
      "PEN_" + number, fLogicPENonSiPM, PDUMotherVolume,
      true, volumeNumber_pen + PDU_counter, CheckOverlap);
  }
  DSLog(debugging) << "Placement of PDU # " << PDU_counter << " completed!" << endl << endlog;
  physSiPMIndex++;
  PDU_counter++;
}
