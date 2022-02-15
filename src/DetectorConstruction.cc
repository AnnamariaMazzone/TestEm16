//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm16/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fDetectorMessenger(0),fpWorld(0),flTarget(0),fTargetMater(0),  fTrackerMater(0),fWorldMater(0),fTargetLength(0), fTargetDepth(0)
{
    
    fTargetLength  = 20.*mm;
    fTargetDepth   = 2.0*mm;
    DefineMaterials();
    SetTargetMaterial("PS50_C50");
    // create commands for interactive definition of the detector
    fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    G4double a, z, density, fractionmass;
    G4int ncomponents, natoms;
    G4String name, symbol;
    new G4Material("Beryllium", z= 4., a= 9.012182*g/mole, density= 1.848*g/cm3);
    new G4Material("Carbon",    z= 6., a= 12.011*g/mole,   density= 2.265*g/cm3);
    new G4Material("Iron",      z=26., a= 55.85*g/mole,    density= 7.870*g/cm3);
    new G4Material("Copper",    z=29., a= 63.546*g/mole,    density= 8.920*g/cm3);
    // define a vacuum with a restgas pressure  typical for accelerators
    G4NistManager::Instance()->FindOrBuildMaterial( "G4_Galactic" );
    // G4Material* Be = G4NistManager::Instance()->FindOrBuildMaterial( "G4_Be" );
    //
    //    G4Material* Cu = G4NistManager::Instance()->FindOrBuildMaterial( "G4_Cu");
    
    G4Material* Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    //carbonato di bario - BaCO3
    G4Element* Ba = G4NistManager::Instance()->FindOrBuildElement("Ba");
    G4Element* C = G4NistManager::Instance()->FindOrBuildElement("C");
    G4Element* O = G4NistManager::Instance()->FindOrBuildElement("O");
    G4Element* H = G4NistManager::Instance()->FindOrBuildElement("H");
    density = 4.29 *g/cm3;
    G4Material* BaCO3 = new G4Material(name="BaCO3", density, ncomponents=3);
    BaCO3->AddElement(Ba, natoms=1);
    BaCO3->AddElement(C, natoms=1);
    BaCO3->AddElement(O, natoms=3);
    
    G4Material* PS = G4NistManager::Instance()->FindOrBuildMaterial( "G4_POLYSTYRENE");
    
    density = 1.664 *g/cm3;
    G4Material* PS50_C50 = new G4Material(name="PS50_C50", density, ncomponents=2);
    PS50_C50->AddMaterial(BaCO3, fractionmass=50.*perCent);
    PS50_C50->AddMaterial(PS , fractionmass=50.*perCent);

    density = 2.419 *g/cm3;
    G4Material* PS25_C75 = new G4Material(name="PS25_C75", density, ncomponents=2);
    PS25_C75->AddMaterial(BaCO3, fractionmass=75.*perCent);
    PS25_C75->AddMaterial(PS , fractionmass=25.*perCent);
    
    //solfato di bario - BaSO4
    G4Element* S = G4NistManager::Instance()->FindOrBuildElement("S");
    density = 4.5 *g/cm3;
    G4Material* BaSO4 = new G4Material(name="BaSO4", density, ncomponents=3);
    BaSO4->AddElement(Ba, natoms=1);
    BaSO4->AddElement(S, natoms=1);
    BaSO4->AddElement(O, natoms=4);
    
    density = 1.653 *g/cm3;
    G4Material* PS50_S50 = new G4Material(name="PS50_S50", density, ncomponents=2);
    PS50_S50->AddMaterial(BaSO4, fractionmass=50.*perCent);
    PS50_S50->AddMaterial(PS , fractionmass=50.*perCent);
    
    density = 2.345 *g/cm3;
    G4Material* PS25_S75 = new G4Material(name="PS25_S75", density, ncomponents=2);
    PS25_S75->AddMaterial(BaSO4, fractionmass=25.*perCent);
    PS25_S75->AddMaterial(PS , fractionmass=75.*perCent);
    
    //Ossido di Bismuto - Bi2O3
    G4Element* Bi = G4NistManager::Instance()->FindOrBuildElement("Bi");
    density = 8.9 *g/cm3;
    G4Material* Bi2O3 = new G4Material(name="Bi2O3", density, ncomponents=2);
    Bi2O3->AddElement(Bi, natoms=2);
    Bi2O3->AddElement(O, natoms=3);
    
    density = 1.8 *g/cm3;
    G4Material* PS50_Bi50 = new G4Material(name="PS50_Bi50", density, ncomponents=2);
    PS50_Bi50->AddMaterial(Bi2O3, fractionmass=50.*perCent);
    PS50_Bi50->AddMaterial(PS , fractionmass=50.*perCent);
    
    density = 2.99 *g/cm3;
    G4Material* PS25_Bi75 = new G4Material(name="PS25_Bi75", density, ncomponents=2);
    PS25_Bi75->AddMaterial(Bi2O3, fractionmass=25.*perCent);
    PS25_Bi75->AddMaterial(PS , fractionmass=75.*perCent);
    
//    Campioni ternari
    //triossido di tungsteno - WO3
    G4Element* W = G4NistManager::Instance()->FindOrBuildElement("W");
    density = 7.16 *g/cm3;
    G4Material* WO3 = new G4Material(name="WO3", density, ncomponents=2);
    WO3->AddElement(W, natoms=1);
    WO3->AddElement(O, natoms=3);
    
    density = 1.742 *g/cm3;
    G4Material* PS50_C25_W25 = new G4Material(name="PS50_C25_W25", density, ncomponents=3);
    PS50_C25_W25->AddMaterial(WO3, fractionmass=25.*perCent);
    PS50_C25_W25->AddMaterial(BaCO3, fractionmass=25.*perCent);
    PS50_C25_W25->AddMaterial(PS , fractionmass=50.*perCent);
    
    //nanotubi di carbonio - CNT
    density = 2.1 *g/cm3;
    G4Material* CNT = new G4Material(name="CNT", density, ncomponents=1);
    CNT->AddElement(C, natoms=1);
    
    
    //    //--------- material names of the principal geometrical components --------
    //
    G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial( "G4_AIR");
    G4Material* Sci =
    new G4Material("EJ-200", density= 1.023*g/cm3, ncomponents=2);
    Sci->AddElement(C, natoms=21);
    Sci->AddElement(H, natoms=19);
    G4Material* Si = G4NistManager::Instance()->FindOrBuildMaterial( "G4_Si");
    fWorldMater   = Air;
    fTargetMater=PS50_C50;
    fTrackerMater =Si;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    G4Box*             sWorld;    // pointer to the solid envelope
    G4LogicalVolume*   lWorld;    // pointer to the logical envelope
//    G4VPhysicalVolume* pWorld;    // pointer to the physical envelope
    G4VisAttributes*   vWorld;      // pointer to visualization attributes
    G4double worldLength;            // half length of the world volume
    
//    //--------- Sizes of the principal geometrical components (solids)  ---------
//      
      // world volume
      worldLength   = 1.*m;
    
    G4Box*             sTarget;   // pointer to the solid Target
//    G4LogicalVolume*   lTarget;   // pointer to the logical Target
    G4VPhysicalVolume* pTarget;   // pointer to the physical Target
    G4VisAttributes*   vTarget;     // pointer to visualization attributes
   
    

    G4Box*             sTracker;  // pointer to the solid Tracker
//    G4LogicalVolume*   lTracker;  // pointer to the logical Tracker
//    G4VPhysicalVolume* pTracker;  // pointer to the physical Tracker
    G4VisAttributes*   vTracker;    // pointer to visualization attributes
    G4double trackerLength;          // half length of tracker
    G4double trackerDepth;           // half depth of tracker
    
      // CCD
      trackerLength = 20.0*mm;
      trackerDepth = 10.*mm;

    //--------- positions of the principal geometrical components --------
    G4double xPosTarget;       // x coordinate of target position
    G4double yPosTarget;       // y          -"-
    G4double zPosTarget;       // z          -"-
    G4double xPosTracker;      // x coordinate of tracker position
    G4double yPosTracker;      // y          -"-
    G4double zPosTracker;      // z          -"-
    
      // target
      xPosTarget = 0.0*cm;
      yPosTarget = 0.0*cm;
      zPosTarget = 0.0*cm;

      // tracker
      xPosTracker = 0.0*cm;
      yPosTracker = 0.0*cm;
    zPosTracker = fTargetDepth+trackerDepth;//2.0*cm;

    //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
      

      //------------------------------
      // World
      //------------------------------

      sWorld= new G4Box("world",worldLength,worldLength,worldLength);
      lWorld= new G4LogicalVolume(sWorld,fWorldMater,"World",0,0,0);
      
      //  Must place the World Physical volume unrotated at (0,0,0).
      //
      fpWorld = new G4PVPlacement(0,               // no rotation
                                     G4ThreeVector(), // at (0,0,0)
                                     lWorld,      // its logical volume
                     "World",         // its name
                                     0,               // its mother  volume
                                     false,           // no boolean operations
                                     0);              // copy number

                 
      //------------------------------
      // Target
      //------------------------------
      
      G4ThreeVector positionTarget =
        G4ThreeVector(xPosTarget,yPosTarget,zPosTarget);
       
    sTarget = new G4Box("target",fTargetLength/2.,fTargetLength/2.,fTargetDepth/2.);
    flTarget = new G4LogicalVolume(sTarget,fTargetMater,"Target",0,0,0);
      pTarget = new G4PVPlacement(0,               // no rotation
                      positionTarget,  // at (x,y,z)
                      flTarget,     // its logical volume
                      "Target",        // its name
                      lWorld,      // its mother  volume
                      false,           // no boolean operations
                      0);              // copy number


      //------------------------------
      // Tracker
      //------------------------------
      
      G4ThreeVector positionTracker =
        G4ThreeVector(xPosTracker,yPosTracker,zPosTracker);
      
      sTracker =
    new G4Box("tracker",trackerLength/2.,trackerLength/2.,trackerDepth/2.);
      flTracker =
    new G4LogicalVolume(sTracker,fTrackerMater,"Tracker",0,0,0);
      fpTracker = new G4PVPlacement(0,              // no rotation
                      positionTracker, // at (x,y,z)
                      flTracker,    // its logical volume
                      "Tracker",       // its name
                      lWorld,      // its mother  volume
                      false,           // no boolean operations
                      0);              // copy number

    //--------- Visualization attributes -------------------------------

      // use this to make world volume invisible
      vWorld = new G4VisAttributes();
      vWorld->SetVisibility(false);
      lWorld->SetVisAttributes(vWorld);

      // render target in redish color
      vTarget = new G4VisAttributes();
    //   visTarget->SetColor(G4Color(1.0,0.3,0.3));  // redish
      vTarget->SetColor(G4Color(1.0,1.0,1.0));  // black
      flTarget->SetVisAttributes(vTarget);

      // render tracker in blueish color
      vTracker = new G4VisAttributes();
    //   visTracker->SetColor(G4Color(0.3,0.3,1.0));   // blueish
      vTracker->SetColor(G4Color(1.0,1.0,1.0));   // black
      flTracker->SetVisAttributes(vTracker);      
      return fpWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
    G4cout << "\n The Target is " << G4BestUnit(fTargetDepth,"Length")
    << " of " << fTargetMater->GetName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
    // search the material by its name
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
    if (pttoMaterial) {
        fTargetMater = pttoMaterial;
        if ( flTarget ) flTarget->SetMaterial(fTargetMater);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
    else {
      G4cout << "\n--> warning from DetectorConstruction::SetDetMaterial : "
             << materialChoice << " not found" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetSize(G4double value)
{
    fTargetDepth = value;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
