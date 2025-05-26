#include <iostream>
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"

#include "DSDetectorProtoProto.hh"
#include "DSIO.hh"
#include "DSLogger.hh"
#include "DSMaterial.hh"
#include "DSParameters.hh"
#include "DSStorage.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

using namespace std;

////////////////////////////////////////////////
//////        Detector Description      ////////
////////////////////////////////////////////////

// June 2024, R. Calabrese 
// September 2024, L. Flores & L. Pandola


DSDetectorProtoProto::DSDetectorProtoProto(G4VPhysicalVolume* myMotherVolume) :  
  fMotherVolume(myMotherVolume)
{
  //Colours and other attributes
  G4Colour myWhite(1.0, 1.0, 1.0);    // white
  G4Colour myGray(0.9, 0.9, 0.9);     // gray
  G4Colour myDarkGray(0.6, 0.6, 0.7); // dark gray
  G4Colour myBlack(0.0, 0.0, 0.0);    // black
  G4Colour myRed(1.0, 0.3, 0.3);      // red
  G4Colour myOrange(1.0, 0.6, 0.1);   // orange
  G4Colour myYellow(1.0, 0.8, 0.3);   // yellow
  G4Colour myGreen(0.6, 0.7, 0.4);    // green
  G4Colour myBlue(0.0, 0.5, 0.7);     // blue
  G4Colour myCyan(0.7, 0.9, 0.9);     // cyan
  G4Colour myMagenta(0.9, 0.3, 0.7);  // magenta
  G4Colour myViolet(0.3, 0.0, 0.7);   // Violet
  G4Colour myPink(1.0, 0.6, 0.7);     // pink
  G4Colour myFloorColour(0.569,0.561,0.561);
  G4Colour myRockColour(0.314,0.078,0.078);
  G4Colour myGArColour(0.745,0.902,1,0.4);
 
  DSLog(routine) << " Constructing DSProtoProto (aka Proto-0) Geometry" << endlog;

  //Read info from storage
  G4bool myCheckOverlap = DSStorage::Get()->GetCheckOverlap();
  G4bool buildFloor = DSStorage::Get()->GetIsProtoProtoBuildFloor();
  G4bool buildScintillator = 
    DSStorage::Get()->GetIsProtoProtoBuildScintillator();
  G4bool isSinglePhase = DSStorage::Get()->GetIsProtoProtoSinglePhase();

  fLogicWorld = fMotherVolume->GetLogicalVolume();
  fLogicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());


  //-------------------------//
  //  Cryostat side internal //
  //-------------------------//
  G4double cryostat_innerR = 0.288*m, cryostat_outerR = cryostat_innerR+3.6*mm, cryostat_zSize = 1.3*m;
  G4double cryostat_startAngle = 0.*deg, cryostat_spanningAngle = 360.*deg;

  G4Tubs* solidCryostat = new G4Tubs("Cryostat", cryostat_innerR, cryostat_outerR, 0.5*cryostat_zSize, 
				     cryostat_startAngle, cryostat_spanningAngle);
  G4LogicalVolume* logicCryostat = new G4LogicalVolume(solidCryostat, DSMaterial::Get()->GetStainlessSteel(), 
						       "Cryostat");                
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "Cryostat", logicCryostat, fMotherVolume, false, 0, myCheckOverlap); 
     
  logicCryostat->SetVisAttributes(myGray);


  //-------------------------//
  //  Cryostat side External //
  //-------------------------//
  
  G4double ext_cryostat_innerR = cryostat_innerR + 4*cm - 0.8*cm, 
    ext_cryostat_outerR = ext_cryostat_innerR +0.8*cm, 
    ext_cryostat_zSize = 1.3*m;
  G4double ext_cryostat_startAngle = 0.*deg, ext_cryostat_spanningAngle = 360.*deg;

  G4Tubs* solid_ext_Cryostat = new G4Tubs("extCryostat", ext_cryostat_innerR, ext_cryostat_outerR, 
					  0.5*ext_cryostat_zSize, ext_cryostat_startAngle, 
					  ext_cryostat_spanningAngle);
  G4LogicalVolume* logic_ext_Cryostat = new G4LogicalVolume(solid_ext_Cryostat, 
							    DSMaterial::Get()->GetStainlessSteel(), 
							    "extCryostat"); 
               
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "extCryostat", logic_ext_Cryostat, fMotherVolume, false, 
		    0, myCheckOverlap);      
  logicCryostat->SetVisAttributes(myGray);

  //-------------------------//
  //     Cryostat void       //
  //-------------------------//
  
  G4Tubs* solid_Vacuum = new G4Tubs("Vacuum", cryostat_outerR, ext_cryostat_innerR, 0.5*ext_cryostat_zSize, 
				    ext_cryostat_startAngle, ext_cryostat_spanningAngle);
  G4LogicalVolume* logic_Vacuum = new G4LogicalVolume(solid_Vacuum, DSMaterial::Get()->GetVacuum(), "Vacuum"); 
               
  G4VPhysicalVolume* fPhysic_Vacuum = 
    new G4PVPlacement(0, G4ThreeVector(), "Vacuum", logic_Vacuum, fMotherVolume, false, 0, myCheckOverlap);  
  logic_Vacuum->SetVisAttributes(myBlack);

  //-------------------------//
  //Cryostat aluminium foils//
  //-------------------------//
  G4double Al_foil_thickness = 30 * 65 * um; 
  G4double Al_foil_inner_radius = (cryostat_outerR + ext_cryostat_innerR)/2; 
  G4double Al_foul_ext_radius = Al_foil_inner_radius + Al_foil_thickness;

  G4Tubs* solid_AL_foils = new G4Tubs("Aluminum foils", Al_foil_inner_radius, Al_foul_ext_radius, 
				      0.5*ext_cryostat_zSize, ext_cryostat_startAngle, ext_cryostat_spanningAngle);
  G4LogicalVolume* logic_AL_foils = new G4LogicalVolume(solid_AL_foils, DSMaterial::Get()->GetAluminum(), 
							"Aluminum_foils_log");                
  new G4PVPlacement(0, G4ThreeVector(), "AlFoils", logic_AL_foils, fPhysic_Vacuum, false, 0, myCheckOverlap);      
  logic_AL_foils->SetVisAttributes(myDarkGray);


   //-------------------------//
  //   Cryostat top flange   //
  //-------------------------//
  G4double flange_innerR = 0.*m, flange_outerR = cryostat_innerR, flange_zSize = 3.*cm;
  G4double flange_startAngle = 0.*deg, flange_spanningAngle = 360.*deg;
 
  G4Tubs* solidFlange = new G4Tubs("Flange", flange_innerR, flange_outerR, 
				   0.5*flange_zSize, flange_startAngle, flange_spanningAngle);

  G4LogicalVolume* logicFlange = new G4LogicalVolume(solidFlange, DSMaterial::Get()->GetStainlessSteel(), 
						     "Top Flange");
  logicFlange->SetVisAttributes(myGray);

  new G4PVPlacement(0, G4ThreeVector(0, 0, cryostat_zSize*0.5-flange_zSize*0.5), logicFlange, 
		    "TopFlange" ,fLogicWorld, false, 0, myCheckOverlap);


  //-------------------------//
  //  Cryostat bottom flange //
  //-------------------------//
  G4double bflange_innerR = 0.*m, bflange_outerR = cryostat_innerR, bflange_zSize = 3.*cm;
  G4double bflange_startAngle = 0.*deg, bflange_spanningAngle = 360.*deg;

  G4Tubs* solidbFlange = new G4Tubs("BottomFlange", bflange_innerR, bflange_outerR, 
				    0.5*bflange_zSize, bflange_startAngle, bflange_spanningAngle);
  G4LogicalVolume* logicbFlange = new G4LogicalVolume(solidbFlange, 
						      DSMaterial::Get()->GetStainlessSteel(), "BottomFlange");
  logicbFlange->SetVisAttributes(myGray);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -cryostat_zSize*0.5+bflange_zSize*0.5), logicbFlange, 
		    "Bottom_Flange",  fLogicWorld, false, 0, myCheckOverlap);

  //-------------------------//
  //          Floor          //
  //-------------------------//
  G4double ConcreteFloor_xySize = 3*m, ConcreteFloor_zSize = 9.0*cm;
  G4double RockFloor_xySize = 3*m, RockFloor_zSize = 41.0*cm;
  G4ThreeVector ConcreteFloor_position = G4ThreeVector(0, 0, -cryostat_zSize*0.5-ConcreteFloor_zSize*0.5);
  G4ThreeVector RockFloor_position = G4ThreeVector(0, 0, -cryostat_zSize*0.5-ConcreteFloor_zSize-RockFloor_zSize*0.5);
  if (buildFloor)
    {           
      G4Box* solidConcreteFloor = new G4Box("Concrete Layer", 0.5*ConcreteFloor_xySize, 
					    0.5*ConcreteFloor_xySize, 0.5*ConcreteFloor_zSize);
      G4LogicalVolume* logicConcreteFloor = new G4LogicalVolume(solidConcreteFloor, 
								DSMaterial::Get()->GetConcrete(), "Concrete Layer");
      logicConcreteFloor->SetVisAttributes(myFloorColour);
      new G4PVPlacement(0, ConcreteFloor_position, logicConcreteFloor,"Concrete Layer", 
			fLogicWorld, false, 0, myCheckOverlap);
    }

  //
  // lead walls
  //
  /*  
  // lead cylindrical wall
  G4double lead_innerR = cryostat_outerR, lead_outerR = lead_innerR+1*cm, lead_zSize = cryostat_zSize/2;
      G4ThreeVector leadWall_position = G4ThreeVector(0, 0, -0.25*cryostat_zSize);
      G4Tubs* solidLeadWall =    
      new G4Tubs("leadWall",                    //its name
      lead_innerR,
      lead_outerR,
      0.5*lead_zSize,
      cryostat_startAngle,
      cryostat_spanningAngle);
      
      G4LogicalVolume* logicLeadWall =                         
      new G4LogicalVolume(solidLeadWall,            //its solid
      lead,             //its material
      "leadWall");         //its name
      
      new G4PVPlacement(0,                       //no rotation
      leadWall_position,         //
      logicLeadWall,                //its logical volume
      "leadWall",              //its name
      fLogicWorld,              //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      myCheckOverlap);          //overlaps checking
  */     
  /*
    G4ThreeVector leadWall_position = 
    G4ThreeVector(0.5*leadWall_ySize, -0.5*m+0.5*leadWall_ySize, -leadWall_zSize*0.5);
    G4Box* solidLeadWall = new G4Box("leadWall", 0.5*leadWall_xSize, 0.5*leadWall_ySize, 0.5*leadWall_zSize);
    
    G4LogicalVolume* logicLeadWall = new G4LogicalVolume(solidLeadWall, lead, "leadWall");
    G4VPhysicalVolume* leadWall_0 = new G4PVPlacement(0,
    leadWall_position,
    logicLeadWall,
    "leadWall",
    fLogicWorld,
    false,
    0,
    checkOverlaps);
    G4RotationMatrix* zRot = new G4RotationMatrix;
    zRot->rotateZ(M_PI/2.*rad);
    G4ThreeVector leadWall_position_1 = 
    G4ThreeVector(-0.5*m+0.5*leadWall_ySize, -0.5*leadWall_ySize, -leadWall_zSize*0.5);
    G4ThreeVector leadWall_position_2 = 
    G4ThreeVector(0.5*m-0.5*leadWall_ySize, 0.5*leadWall_ySize, -leadWall_zSize*0.5);
    G4ThreeVector leadWall_position_3 = 
    G4ThreeVector(-0.5*leadWall_ySize, 0.5*m-0.5*leadWall_ySize, -leadWall_zSize*0.5);
    G4VPhysicalVolume* leadWall_1 = new G4PVPlacement(zRot,
    leadWall_position_1,
    logicLeadWall,
    "leadWall",
    fLogicWorld,
    false,
    1,
    checkOverlaps);
    
    G4VPhysicalVolume* leadWall_2 = new G4PVPlacement(zRot,
    leadWall_position_2,
    logicLeadWall,
    "leadWall",
    fLogicWorld,
    false,
    2,
    checkOverlaps);
    
    G4VPhysicalVolume* leadWall_3 = new G4PVPlacement(0,
    leadWall_position_3,
    logicLeadWall,
    "leadWall",
    fLogicWorld,
    false,
    3,
    checkOverlaps);
  */
  if (buildFloor)
    {
      G4Box* solidRockFloor = new G4Box("Rock Layer", 
					0.5*RockFloor_xySize, 0.5*RockFloor_xySize, 0.5*RockFloor_zSize);
      G4LogicalVolume* logicRockFloor = new G4LogicalVolume(solidRockFloor, 
							    DSMaterial::Get()->GetNorite(), "Rock Layer");
      logicRockFloor->SetVisAttributes(myRockColour);
      new G4PVPlacement(0, RockFloor_position, logicRockFloor,"Rock Layer",  
			fLogicWorld, false, 0, myCheckOverlap); 
    }


  //-------------------------//
  //      LAr bath           //
  //-------------------------//
  G4double LArBathVolume_innerRadius = 0.*m, LArBathVolume_outerRadius = cryostat_innerR, 
    LArBathVolume_zSize = cryostat_zSize - 6*cm;
  G4double LArBathVolume_startAngle = 0.*deg, LArBathVolume_spanningAngle = 360.*deg;
  
  G4Tubs* solid_LArBath = new G4Tubs("solid_LArBath", LArBathVolume_innerRadius, 
				     LArBathVolume_outerRadius, 0.5*LArBathVolume_zSize, 
				     LArBathVolume_startAngle, LArBathVolume_spanningAngle);
  G4LogicalVolume* flogic_LArBath = new G4LogicalVolume(solid_LArBath, 
							DSMaterial::Get()->GetNSLiquidArgon(), "LArBath_Logic");
  flogic_LArBath->SetVisAttributes(myViolet);
  fLArBath = new G4PVPlacement(0, G4ThreeVector(), flogic_LArBath,
			       "LArBath_Physic", fLogicWorld, false, 0, myCheckOverlap);
  
  //-------------------------//
  //       Acrilic box       //
  //-------------------------//
  G4double cathode_lenght = 35.0 *cm;
  G4double cathode_height = 5.0 *cm;

  G4double acrilic_Cage_thickness = 2.3 *cm;
  G4double acrilic_Cage_height = 12.0 *cm;
  G4double acrilic_Cage_lenght = cathode_lenght;

  G4double Active_LAr_z_coordinate = - 31 *cm;
  G4double acrilic_box_z = Active_LAr_z_coordinate;


  G4double vessel_external_x = acrilic_Cage_lenght;
  G4double vessel_external_z = acrilic_Cage_height;

  G4Box* solid_vessel = new G4Box("TPC cage", vessel_external_x / 2., vessel_external_x / 2., vessel_external_z / 2.);
  G4LogicalVolume* flogic_vessel = new G4LogicalVolume(solid_vessel, 
						       DSMaterial::Get()->GetAcrylic(), "TPC cage");
  flogic_vessel->SetVisAttributes(myWhite);
  new G4PVPlacement(0, G4ThreeVector(0, 0, acrilic_box_z), 
		    flogic_vessel, "TPC_cage", 
		    flogic_LArBath, false, 0, myCheckOverlap);

  //-------------------------//
  //          Cathode        //
  //-------------------------//
  G4Box* solid_Cathode = new G4Box("Cathode", cathode_lenght/ 2., cathode_lenght/ 2., cathode_height/ 2.);
  G4LogicalVolume* flogic_Cathode = new G4LogicalVolume(solid_Cathode, 
							DSMaterial::Get()->GetAcrylic(), "Cathode");
  flogic_Cathode->SetVisAttributes(myWhite);
  new G4PVPlacement(0, G4ThreeVector(0, 0, acrilic_box_z - acrilic_Cage_height/2 - cathode_height/2), 
		    flogic_Cathode,"Cathode", flogic_LArBath, false, 0, myCheckOverlap);


  //-------------------------//
  //         LAr             //
  //-------------------------//
  G4double LAr_external_x = cathode_lenght - 2 * acrilic_Cage_thickness;
  G4double LAr_external_z = acrilic_Cage_height;

  G4Box* solid_LAr = new G4Box("solid_LAr", LAr_external_x / 2., LAr_external_x / 2., LAr_external_z / 2.);
  G4LogicalVolume* flogic_LAr = new G4LogicalVolume(solid_LAr, DSMaterial::Get()->GetNSLiquidArgon(), "LAr_Logic");
  flogic_LAr->SetVisAttributes(myViolet);
  new G4PVPlacement(0, 
		    G4ThreeVector(), flogic_LAr, "LArInsideCage", 
		    flogic_vessel, false, 0, myCheckOverlap);
  
   //-------------------------//
  // Acrilic reflective cage //
  //-------------------------//
  //G4double ESR_tickness = 100 * um;
  G4double TPB_tickness = 100 * um;

  G4double reflective_Cage_tickness = 0.8 *cm;
  G4double reflective_Cage_height = acrilic_Cage_height;
  G4double Active_LAr_lenght = 20*cm; 
  G4double reflective_Cage_lenght = Active_LAr_lenght + 2 * reflective_Cage_tickness + 
    TPB_tickness *2; /* + ESR_tickness*2;*/

  G4Box* solid_ReflectiveCage = new G4Box("Reflective Cage", reflective_Cage_lenght/ 2., 
					  reflective_Cage_lenght/ 2., reflective_Cage_height/ 2.);
  G4LogicalVolume* flogic_ReflectiveCage = new G4LogicalVolume(solid_ReflectiveCage, 
							       DSMaterial::Get()->GetAcrylic(), "ReflectiveCageLog");
  flogic_ReflectiveCage->SetVisAttributes(myWhite);
  fReflectiveCage = new G4PVPlacement(0, G4ThreeVector(), 
		    flogic_ReflectiveCage, "ReflectiveCage", 
		    flogic_LAr, false, 0, myCheckOverlap);

  //
  // Drop the ESR layer: use optical surface to account for it
  /*    
  //-------------------------//
  //        ESR layer        //
  //-------------------------//

  G4double ESR_Cage_lenght = Active_LAr_lenght + TPB_tickness *2 + ESR_tickness*2;
  G4double ESR_Cage_height = acrilic_Cage_height;
  
  G4Box* solid_ESR = new G4Box("ESR layer", ESR_Cage_lenght/ 2., ESR_Cage_lenght/ 2., ESR_Cage_height/ 2.);
  G4LogicalVolume* flogic_ESR = new G4LogicalVolume(solid_ESR, DSMaterial::Get()->GetESR(), "ESRlayer_log"); 
  fPhysicLayerESR = new G4PVPlacement(0, G4ThreeVector(), flogic_ESR, "ESR_layer", 
				      flogic_ReflectiveCage, false, 0, myCheckOverlap); 
  flogic_ESR->SetVisAttributes(myRed);
  */

  //-------------------------//
  //        TPB layer        //
  //-------------------------//
  G4double TPB_Cage_lenght = Active_LAr_lenght + TPB_tickness *2;
  G4double TPB_Cage_height = acrilic_Cage_height;
  
  G4Box* solid_TPBCage = new G4Box("TPB layer", TPB_Cage_lenght/ 2., TPB_Cage_lenght/ 2., TPB_Cage_height/ 2.);
  G4LogicalVolume* flogic_TPBCage = new G4LogicalVolume(solid_TPBCage, DSMaterial::Get()->GetTPB(), "TPBlayerLog"); 
  fPhysTPBLayer = new G4PVPlacement(0, G4ThreeVector(), flogic_TPBCage, "TPB_layer",  
				    /* flogic_ESR,*/ 
				    flogic_ReflectiveCage, false, 0, myCheckOverlap); 
  flogic_TPBCage->SetVisAttributes(myCyan);

  //-------------------------//
  //   Active Lar Volume     //
  //-------------------------//
  G4double ActiveLAr_height = acrilic_Cage_height-TPB_tickness;
  
  G4Box* solid_ActiveLAr = new G4Box("Active LAr", Active_LAr_lenght/2., Active_LAr_lenght/2., ActiveLAr_height/2.);
  G4LogicalVolume* flogic_ActiveLAr = new G4LogicalVolume(solid_ActiveLAr, DSMaterial::Get()->GetLiquidArgon(), 
							  "ActiveLAr_logic");
  flogic_ActiveLAr->SetVisAttributes(myMagenta);
  fPhysicActiveLAr = new G4PVPlacement(0, G4ThreeVector(0, 0, TPB_tickness/2), flogic_ActiveLAr, 
		    "ActiveLAr", flogic_TPBCage, false, 0, myCheckOverlap); 

  //Add the active volume in a dedicated region
  G4Region* fLArRegion = new G4Region("LAr_Logic");
  flogic_ActiveLAr->SetRegion(fLArRegion);
  fLArRegion->AddRootLogicalVolume(flogic_ActiveLAr);

  //This is the material index of Active LAr
  DSStorage::Get()->SetLiquidArgonIndex((int)flogic_ActiveLAr->GetMaterial()->GetIndex());


  //-------------------------//
  //         Anode Cage      //
  //-------------------------//
  G4double Anode_lenght = 39 *cm; 
  G4double Top_Reflective_cage_z = Active_LAr_z_coordinate + reflective_Cage_height/2;

  G4double GridToAnodeThickness = DSStorage::Get()->GetProtoProtoGridToAnodeThickness();
  DSLog(routine) << "Grid to anode distance: " << GridToAnodeThickness/mm << " mm" << endlog;
  //G4double GridToAnodeThickness = 10.* mm; // Grid to anode distance (LAr+Gar pocket)

  G4double Anode_thickness = 5*cm ;
  G4double Top_Anode_z = Top_Reflective_cage_z+GridToAnodeThickness+Anode_thickness;
  G4double Anode_z = (Top_Reflective_cage_z + Top_Anode_z)/2;
  G4double Anode_total_thickness = (-Top_Reflective_cage_z + Top_Anode_z);
 
  G4Box* solid_Anode = new G4Box("Anode", Anode_lenght/ 2., Anode_lenght/ 2., Anode_total_thickness/ 2.);
  G4LogicalVolume* flogic_Anode = new G4LogicalVolume(solid_Anode, DSMaterial::Get()->GetAcrylic(), 
						      "Anode");
 
  //G4double Anode_distamce_from_the_top_flange = 77.0 *cm; // riferito rispetto alla parte superiore dell'anodo
  G4double Anode_distamce_from_the_top_flange = cryostat_zSize/2-Top_Anode_z;
  DSLog(trace) << "Distance between the top face of the anode and the top of the cryostat: " << 
    Anode_distamce_from_the_top_flange/cm << " cm" << endlog;
  
  flogic_Anode->SetVisAttributes(myWhite);
  new G4PVPlacement(0, G4ThreeVector(0, 0, Anode_z), 
		    flogic_Anode, "Anode", flogic_LArBath, false, 0, 
		    myCheckOverlap);
  
  //-------------------------//
  //      LAr pocket         //
  //-------------------------//
  G4double LAr_pocket_thickness = 0.3 *cm;
  G4double LAr_pocket_lenght = acrilic_Cage_lenght;
  G4double LAr_pocket_z = - Anode_total_thickness/2 + LAr_pocket_thickness/2;

  G4Box* solid_LAr_pocket = new G4Box("LAr_pocket", LAr_pocket_lenght/ 2., 
				      LAr_pocket_lenght/ 2., LAr_pocket_thickness/ 2.);
  G4LogicalVolume* flogic_LAr_pocket = new G4LogicalVolume(solid_LAr_pocket, 
							   DSMaterial::Get()->GetLArAboveGrid(), "LAr_pocket");
  flogic_LAr_pocket->SetVisAttributes(myViolet);
  fLArPocket = new G4PVPlacement(0, G4ThreeVector(0, 0, LAr_pocket_z), flogic_LAr_pocket, 
		    "LAr_Pocket",  flogic_Anode, false, 0, myCheckOverlap);
  DSStorage::Get()->SetLArAboveGridIndex(flogic_LAr_pocket->GetMaterial()->GetIndex());

  //-------------------------//
  //      GAr Pocket         //
  //-------------------------//
  G4double GAr_thickness = (Anode_total_thickness - Anode_thickness) - LAr_pocket_thickness;
  if (isSinglePhase)
    DSLog(routine) << "No Gas pocket: additional LAr thickness: " << GAr_thickness/mm << " mm" << endlog;
  else
    DSLog(routine) << "Gas pocket thickness: " << GAr_thickness/mm << " mm" << endlog;

  //Set parameters required to generate S2
  DSStorage::Get()->SetDS20kGasPocketThickness(GAr_thickness);
  DSStorage::Get()->SetLArGArBoundaryPosZ(-247.*mm);

  G4double GAr_lenght = acrilic_Cage_lenght;
  G4double GAr_z = LAr_pocket_z + LAr_pocket_thickness/2 + GAr_thickness/2;

  G4Box* solid_GAr = new G4Box("GAr_Pocket", GAr_lenght/ 2., GAr_lenght/ 2., GAr_thickness/ 2.);

  //If it's single phase, only liquid, otherwise it is gas
  G4String aName = (isSinglePhase) ? "LAr_Pocket2" : "GAr_Pocket";
  G4Material* aPocketMat = 
    (isSinglePhase) ? DSMaterial::Get()->GetLArAboveGrid() : DSMaterial::Get()->GetGaseousArgon();

  G4LogicalVolume* flogic_GAr = new G4LogicalVolume(solid_GAr, aPocketMat,
						    "GAr_Pocket");
  flogic_GAr->SetVisAttributes(myGArColour);
  new G4PVPlacement(0, G4ThreeVector(0, 0, GAr_z), 
		    flogic_GAr, aName, flogic_Anode, false, 
		    0, myCheckOverlap);

  //-------------------------//
  // TPB Layer on the Anode  //
  //-------------------------//
  G4double TPB_external_x = Active_LAr_lenght;
  G4double TPB_external_z = 100 * um;
  G4double TPB_z_coordinate = LAr_pocket_z + LAr_pocket_thickness/2 + GAr_thickness + 
    TPB_external_z/2;

  G4Box* solid_TPB = new G4Box("TPB Upper layer", TPB_external_x / 2., TPB_external_x / 2., 
			       TPB_external_z / 2.);
  G4LogicalVolume* flogic_TPB = new G4LogicalVolume(solid_TPB, DSMaterial::Get()->GetTPB(), "TPB_Upper_layer");
  flogic_TPB->SetVisAttributes(myCyan);
  new G4PVPlacement(0, G4ThreeVector(0, 0, TPB_z_coordinate), flogic_TPB, "TPB_Upper_layer", 
		    flogic_Anode, false, 0, myCheckOverlap);

  //---------------------//
  //      SiPM array     //
  //---------------------//

  // Top Array
  G4double SiPM_lenght = 20*cm, SiPM_thickness = 1*mm;

  G4Box* fSolidSiPMTop = new G4Box("SolidSiPMTop",SiPM_lenght/ 2., SiPM_lenght/ 2., 
				   SiPM_thickness/2);
  G4LogicalVolume* fLogicSiPMTop = new G4LogicalVolume(fSolidSiPMTop, 
						       DSMaterial::Get()->GetMetalSilicon(), "SiPMTop_Logic");
  fLogicSiPMTop->SetVisAttributes(myYellow);
  G4double anodeToSiPMdistance = 1.0*cm; // set minimum distance of 0.1 mm

  G4double vertical_coordinate = Anode_z + Anode_total_thickness/ 2. + anodeToSiPMdistance + SiPM_thickness/2.; 
  fSiPMTop = new G4PVPlacement(0,G4ThreeVector(0, 0, vertical_coordinate), fLogicSiPMTop, "SiPMTop", flogic_LArBath, false, 
		    0, myCheckOverlap);

  // Bottom Array  
  if (DSStorage::Get()->GetIsProtoProtoBottomSiPM()) {
    G4Box* fSolidSiPMBottom = new G4Box("SolidSiPMBottom", SiPM_lenght / 2., SiPM_lenght/ 2., SiPM_thickness/2);
    G4LogicalVolume* fLogicSiPMBottom = new G4LogicalVolume(fSolidSiPMBottom, 
							  DSMaterial::Get()->GetMetalSilicon(), "SiPMBottom_Logic");
    fLogicSiPMBottom->SetVisAttributes(myYellow);
    //Here distance is 2.4 cm 
    G4double cathodeToSiPMdistance = 2.4*cm;
    vertical_coordinate = acrilic_box_z - acrilic_Cage_height/2 -  cathode_height - cathodeToSiPMdistance 
      -  SiPM_thickness/2;
    fSiPMBottom = new G4PVPlacement(0, G4ThreeVector(0, 0, vertical_coordinate), fLogicSiPMBottom,"SiPMBottom", 
				    flogic_LArBath, false, 0, myCheckOverlap);   
  }
  // make SiPM as pe storing material
  DSStorage::Get()->SetPMTMaterialIndex(fLogicSiPMTop->GetMaterial()->GetIndex());

  if (buildScintillator)
    BuildScintillator(ext_cryostat_outerR);

  DefineSurfaces();
}

DSDetectorProtoProto::~DSDetectorProtoProto() {
  ;  
}

void DSDetectorProtoProto::DefineSurfaces() 
{
  //=======================================================================================
  //   Grid
  //=======================================================================================
  // Grid not defined as a physical volume: implement grid transparency as an optical 
  // surface between LAr pocket and active LAr volume  
  G4double gridTransparency = 0.94; 
  DSLog(routine) << "Grid transparency is set to: " << gridTransparency << endlog;

  G4OpticalSurface* fGridSurface = new G4OpticalSurface("GridOpSurface");
  new G4LogicalBorderSurface("GridBorderSurface",fPhysicActiveLAr,fLArPocket,fGridSurface);

  const G4int NUM = 4;
  G4double pp[NUM] = {0.1 * eV, 8.0 * eV, 8.3 * eV, 20.0 * eV};  // vis, vis, UV, UV 
  G4double reflectivity[NUM] = {0.0, 0.0, 0.0, 0.0};             //  To set 1-absorption
  G4double transmitivity[NUM] = {gridTransparency, gridTransparency, gridTransparency, 
				 gridTransparency};    //  To set reflection vs. transmission, overridding Fresnel
  //Here the remaining 1-gridTransparency is absorbance
  G4MaterialPropertiesTable* fGridSurfProp = new G4MaterialPropertiesTable();
  fGridSurfProp->AddProperty("REFLECTIVITY", pp, reflectivity, NUM);
  fGridSurfProp->AddProperty("TRANSMITTANCE", pp, transmitivity, NUM);
  fGridSurface->SetMaterialPropertiesTable(fGridSurfProp);
  

  //=======================================================================================
  // Passive LAr -> SiPM
  //=======================================================================================
  G4OpticalSurface* fOpNewLArSiPMSurface = new G4OpticalSurface("OpNewLArSiPMSurface");
  new G4LogicalBorderSurface("NewLArSiPMSurface", fLArBath,fSiPMBottom, fOpNewLArSiPMSurface);
  new G4LogicalBorderSurface("NewLArSiPMSurface", fSiPMTop, fLArBath, fOpNewLArSiPMSurface);
  fOpNewLArSiPMSurface->SetType(dielectric_dielectric);
  fOpNewLArSiPMSurface->SetModel(glisur);
  fOpNewLArSiPMSurface->SetFinish(polished);

  G4MaterialPropertiesTable* fNewLArSiPMSurfProp = new G4MaterialPropertiesTable();
  fNewLArSiPMSurfProp->AddConstProperty("DONewLArSiPM", 1, true);
  fOpNewLArSiPMSurface->SetMaterialPropertiesTable(fNewLArSiPMSurfProp);

  //=======================================================================================
  // TPB --> Reflector (Reflector)
  //  Should be no Reflector --> TPB as surface is defined as dielectric_metal.
  //=======================================================================================

  G4double ReflectorTPBENE[4] = {0.1 * eV, 8.0 * eV, 8.3 * eV, 20.0 * eV};
  G4double TREFUV = DSParameters::Get()->GetTeflonTPBUVRef();
  G4double TREFVIS = DSParameters::Get()->GetTeflonTPBVisRef();
  G4double ReflectorTPBREF[4] = {TREFVIS, TREFVIS, TREFUV, TREFUV};

  DSLog(trace) << "UV Reflectivity of reflector (teflon): " << TREFUV << endlog;
  DSLog(trace) << "Vis Reflectivity of reflector (teflon): " << TREFVIS << endlog;

  G4OpticalSurface* fOpTPBReflectorSurface = new G4OpticalSurface("OpTBPReflectorSurface");
  new G4LogicalBorderSurface("TPBReflectorSurface", fPhysTPBLayer, 
			     fReflectiveCage, fOpTPBReflectorSurface);

  ///////////////
  // TPB Skin --> anything  (Reflector)
  //  Will do bi-directional to prevent light from the veto to enter the TPC
  ////////////////////////////////////////
  fOpTPBReflectorSurface->SetType(dielectric_metal);
  fOpTPBReflectorSurface->SetModel(unified);

  // PDM: though I can't see how, the following settings are giving the desired
  // Lambertian reflection
  fOpTPBReflectorSurface->SetFinish(polished);

  G4MaterialPropertiesTable* fTPBReflectorSurfProp = new G4MaterialPropertiesTable();
  fTPBReflectorSurfProp->AddProperty("REFLECTIVITY", ReflectorTPBENE, ReflectorTPBREF, 4);
  fOpTPBReflectorSurface->SetMaterialPropertiesTable(fTPBReflectorSurfProp);



  /*
  //-------------------------//
  //        TPB <-- GA       //
  //-------------------------//
  G4OpticalSurface* fOpTPBGArSurface = new G4OpticalSurface("OpTPB-GAr Surface");
  new G4LogicalBorderSurface("GAr-TPB Surface", fPhysic_GAr, fPhysic_TPB, fOpTPBGArSurface);
  new G4LogicalBorderSurface("TPB-GAr Surface", fPhysic_TPB, fPhysic_GAr, fOpTPBGArSurface);

  fOpTPBGArSurface->SetType(dielectric_dielectric);
  fOpTPBGArSurface->SetModel(unified);
  fOpTPBGArSurface->SetFinish(ground);
  fOpTPBGArSurface->SetSigmaAlpha(0.3);

  G4double VISTRAN = DSParameters::Get()->GetArTPBVisTran();
  const G4int NUM = 4;
  G4double pp[NUM] = {0.1 * eV, 8.0 * eV, 8.3 * eV, 20.0 * eV};  // vis, vis, UV, UV 
  G4double specularlobe[NUM] = {0., 0., 0., 0.};                 //--
  G4double specularspike[NUM] = {0., 0., 0., 0.};                //----  gives all reflection to Lambertian lobe
  G4double backscatter[NUM] = {0., 0., 0., 0.};                  //--
  G4double reflectivity[NUM] = {1.0, 1.0, 1.0, 1.0};             //  To set 1-absorption
  G4double transmitivity[NUM] = {VISTRAN, VISTRAN, 1.0, 1.0};    //  To set reflection vs. transmission, overridding Fresnel
                                                                 //  For now, no angle dependence.
  G4MaterialPropertiesTable* fTPBGArSurfProp = new G4MaterialPropertiesTable();
  fTPBGArSurfProp->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
  fTPBGArSurfProp->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
  fTPBGArSurfProp->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
  fTPBGArSurfProp->AddProperty("REFLECTIVITY", pp, reflectivity, NUM);
  fTPBGArSurfProp->AddProperty("TRANSMITTANCE", pp, transmitivity, NUM);
  fTPBGArSurfProp->AddConstProperty("DOArTPB", 1);
  fOpTPBGArSurface->SetMaterialPropertiesTable(fTPBGArSurfProp);
*/
  }

void DSDetectorProtoProto::BuildScintillator(G4double cryostatOuterRadius)
{
  DSLog(trace) << " Constructing EJ209 scintillator of Proto-0" << endlog;
  G4bool checkOverlaps = DSStorage::Get()->GetCheckOverlap();
  G4VisAttributes* C_Metals = new G4VisAttributes(G4Colour(0.761, 0.761, 0.761, 1.));
  G4VisAttributes* Red = new G4VisAttributes(G4Colour(0.788, 0.071, 0.071, 1.));

  //---------------------//
  //      3-inch LSci     //
  //---------------------//
  //General
     
  G4double RB_thickness = 1.52 *mm, Td = 1 *nm;
  G4double Box_h = 500. *mm, Box_l = 120. *mm;
  G4double st_angle = 0. *deg, sp_angle = 360. *deg;
      
  //External part      
  G4double RB_E_outerD = 58.25 *mm;
  G4double RB_D_outerD = RB_E_outerD + 2*9.5 *mm; 
  G4double RB_C_outerD = RB_D_outerD;
  G4double RB_B_outerD = RB_D_outerD + 2*9.1 *mm;
  G4double RB_A_outerD = RB_B_outerD - 2*6.2 *mm;
  G4double RB_outerD = RB_A_outerD - 2*4.9 *mm; 
      
  G4double RB_innerD = 0., RB_A_innerD = RB_outerD + Td, RB_B_innerD = 0., RB_C_innerD = 0., 
    RB_D_innerD = RB_E_outerD, RB_E_innerD = 0.;
      
  G4double RB_A_h = 21. *mm, RB_B_h = 15. *mm, RB_C_h = 51.2 *mm, RB_D_h = 9.5 *mm, RB_E_h = 156.75 *mm;
  G4double RB_h = RB_A_h + 70.75*mm;
  
  //Inner part      
  G4double Scint_innerD = 0. *mm, Scint_outerD = RB_outerD - 2*RB_thickness, 
    Scint_h = 70.9 *mm - 2*RB_thickness -Td; //scintillatore
  G4double OW_innerD = 0. *mm, OW_outerD = Scint_outerD, OW_h = RB_h- Scint_h -Td - RB_thickness;  //optical window
  //B is solid
  G4double RB_C_in_innerD = 0., RB_C_in_outerD = RB_C_outerD - 2*RB_thickness, RB_C_in_h = RB_C_h; // C empty
  G4double RB_D_in_innerD = RB_D_innerD - 2*RB_thickness, RB_D_in_outerD = RB_D_outerD - 2*RB_thickness, 
    RB_D_in_h = RB_D_h;  // D empty
  G4double RB_E_in_innerD = 0., RB_E_in_outerD = RB_E_outerD - 2*RB_thickness, 
    RB_E_in_h = RB_E_h -2*RB_thickness; // E empty
      
  //G4VSolids
      
  //Box/Envelope 
  G4Box* Solid_RBox= new G4Box("Solid_RBox", Box_l/2, Box_l/2, Box_h/2);
      
  //External
  G4Tubs* SolidRB = new G4Tubs("SolidRB", RB_innerD/2, RB_outerD/2, RB_h/2, st_angle, sp_angle);
  G4Tubs* SolidRB_A = new G4Tubs("SolidRB_A", RB_A_innerD/2, RB_A_outerD/2, RB_A_h/2, st_angle, sp_angle);
  G4Tubs* SolidRB_B = new G4Tubs("SolidRB_B", RB_B_innerD/2, RB_B_outerD/2, RB_B_h/2, st_angle, sp_angle);
  G4Tubs* SolidRB_C = new G4Tubs("SolidRB_C", RB_C_innerD/2, RB_C_outerD/2, RB_C_h/2, st_angle, sp_angle);
  G4Cons* SolidRB_D = new G4Cons("SolidRB_D", 0., RB_D_innerD/2, 0., RB_D_outerD/2, RB_D_h/2, 
				 st_angle, sp_angle); // D è il cono mozzato
  G4Tubs* SolidRB_E = new G4Tubs("SolidRB_E", RB_E_innerD/2, RB_E_outerD/2, RB_E_h/2, st_angle, sp_angle);
      
  //Inner
  G4Tubs* SolidScint = new G4Tubs("SolidScint", Scint_innerD/2, Scint_outerD/2, Scint_h/2, st_angle, sp_angle);
  G4Tubs* SolidOW = new G4Tubs("SolidOW", OW_innerD/2, OW_outerD/2, OW_h/2, st_angle, sp_angle);
  //B è pieno
  G4Tubs* SolidRB_C_in = new G4Tubs("SolidRB_C_in", RB_C_in_innerD/2, RB_C_in_outerD/2, RB_C_in_h/2, 
				    st_angle, sp_angle);
  G4Cons* SolidRB_D_in = new G4Cons("SolidRB_D", 0., RB_D_in_innerD/2, 0., RB_D_in_outerD/2, RB_D_in_h/2, 
				    st_angle, sp_angle);//D è il cono mozzato (vuoto)
  G4Tubs* SolidRB_E_in = new G4Tubs("SolidRB_E_in", RB_E_in_innerD/2, RB_E_in_outerD/2, RB_E_in_h/2, 
				    st_angle, sp_angle);
  //Logical Volumes      
  //Box
  G4LogicalVolume* logic_RBox = new G4LogicalVolume(Solid_RBox, DSMaterial::Get()->GetAir(), "logic_RBox");
  logic_RBox->SetVisAttributes(G4VisAttributes::GetInvisible());
      
  //Internal
  G4Material* Al = DSMaterial::Get()->GetAluminum(); 
  G4LogicalVolume* logicRB = new G4LogicalVolume(SolidRB, Al, "logicRB"); logicRB->SetVisAttributes(C_Metals);
  G4LogicalVolume* logicRB_A = new G4LogicalVolume(SolidRB_A, Al, "logicRB_A"); logicRB_A->SetVisAttributes(C_Metals);
  G4LogicalVolume* logicRB_B = new G4LogicalVolume(SolidRB_B, Al, "logicRB_B"); logicRB_B->SetVisAttributes(Red);
  G4LogicalVolume* logicRB_C = new G4LogicalVolume(SolidRB_C, Al, "logicRB_C"); logicRB_C->SetVisAttributes(C_Metals);
  G4LogicalVolume* logicRB_D = new G4LogicalVolume(SolidRB_D, Al, "logicRB_D"); logicRB_D->SetVisAttributes(C_Metals);
  G4LogicalVolume* logicRB_E = new G4LogicalVolume(SolidRB_E, Al, "logicRB_E"); logicRB_E->SetVisAttributes(C_Metals);
  
  //Internal
  G4LogicalVolume* logicScint = new G4LogicalVolume(SolidScint, DSMaterial::Get()->GetEJ309(), "logicScint"); 
  G4LogicalVolume* logicOW = new G4LogicalVolume(SolidOW, DSMaterial::Get()->GetBK7(), "logicOW");
  G4LogicalVolume* logicRB_C_in = new G4LogicalVolume(SolidRB_C_in, DSMaterial::Get()->GetVacuum(), "logicRB_C_in");
  G4LogicalVolume* logicRB_D_in = new G4LogicalVolume(SolidRB_D_in, DSMaterial::Get()->GetVacuum(), "logicRB_D_in");
  G4LogicalVolume* logicRB_E_in = new G4LogicalVolume(SolidRB_E_in, DSMaterial::Get()->GetVacuum(), "logicRB_E_in");
      
  //Physical Volumes
  //Box
      
  G4RotationMatrix* box_rot = new G4RotationMatrix();
  box_rot->rotateY(90. *deg); box_rot->rotateX(0.*deg); box_rot->rotateZ(0.*deg);
  G4double cryo_to_scint_gap = 3.*mm; //Distance between the cryostat and the close face of the scintillator
  G4double RBox_dist = cryostatOuterRadius + Box_h/2. + cryo_to_scint_gap;  
  G4double RBox_z = -30.95 *cm;
  G4ThreeVector RBox_pos = G4ThreeVector( RBox_dist , 0 , RBox_z);
      
  new G4PVPlacement(box_rot, RBox_pos, logic_RBox, "RBox", 
		    fLogicWorld, false, 0, checkOverlaps);
      
  //External
  G4double RBz = Box_h/2 - RB_h/2;
  G4double RB_Az = RBz - RB_h/2 + RB_A_h/2 - Td;
  G4double RB_Bz = RB_Az - RB_A_h/2 - RB_B_h/2 -Td;
  G4double RB_Cz = RB_Bz - RB_B_h/2 - RB_C_h/2 -Td;
  G4double RB_Dz = RB_Cz - RB_C_h/2 - RB_D_h/2 -Td;
  G4double RB_Ez = RB_Dz - RB_D_h/2 - RB_E_h/2 -Td;
  
  G4ThreeVector RB_pos =  G4ThreeVector(0., 0., RBz);
  G4ThreeVector RB_A_pos =  G4ThreeVector(0., 0., RB_Az);
  G4ThreeVector RB_B_pos =  G4ThreeVector(0., 0., RB_Bz);
  G4ThreeVector RB_C_pos =  G4ThreeVector(0., 0., RB_Cz);
  G4ThreeVector RB_D_pos =  G4ThreeVector(0., 0., RB_Dz);
  G4ThreeVector RB_E_pos =  G4ThreeVector(0., 0., RB_Ez);
  
  new G4PVPlacement(0, RB_pos, logicRB, "RB", logic_RBox, 
		    false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_A_pos, logicRB_A, "RB_A", logic_RBox, 
		    false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_B_pos, logicRB_B, "RB_B", logic_RBox, 
		    false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_C_pos, logicRB_C, "RB_C", logic_RBox, 
		    false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_D_pos, logicRB_D, "RB_D", logic_RBox, 
		    false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_E_pos, logicRB_E, "RB_E", logic_RBox, 
		    false, 0, checkOverlaps);
      
  //Internal
  G4ThreeVector Scint_pos =  G4ThreeVector(0., 0., RB_h/2 - Scint_h/2 - RB_thickness/2);
  G4ThreeVector OW_pos =  G4ThreeVector(0., 0., -RB_h/2 + OW_h/2);
  G4ThreeVector RB_C_in_pos = G4ThreeVector(0., 0., 0.);
  G4ThreeVector RB_D_in_pos =  G4ThreeVector(0., 0., 0.);
  G4ThreeVector RB_E_in_pos =  G4ThreeVector(0., 0., 0.);
      
  new G4PVPlacement(0, Scint_pos, logicScint, "Scint", 
		    logicRB, false, 0, checkOverlaps);
  new G4PVPlacement(0, OW_pos, logicOW, "OW", 
		    logicRB, false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_C_in_pos, logicRB_C_in, 
		    "RB_C_in", logicRB_C, false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_D_in_pos, logicRB_D_in, "RB_D_in", 
		    logicRB_D, false, 0, checkOverlaps);
  new G4PVPlacement(0, RB_E_in_pos, logicRB_E_in, "RB_E_in", 
		    logicRB_E, false, 0, checkOverlaps);
  return;

}
