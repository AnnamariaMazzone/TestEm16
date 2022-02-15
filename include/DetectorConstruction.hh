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
/// \file electromagnetic/TestEm16/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4VisAttributes.hh"

class G4Material;
class G4UserLimits;
class DetectorMessenger;
class G4GlobalMagFieldMessenger;
class G4Box;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

  public:

     virtual G4VPhysicalVolume* Construct();

     void SetTargetSize        (G4double);
     void SetTargetMaterial    (G4String);
    

  public:
     G4double           GetTargetSize() const      {return fTargetDepth;};
     G4Material*        GetTargetMaterial() const  {return fTargetMater;};
     const G4VPhysicalVolume* GetTracker() const  {return fpTracker;};
     void               PrintParameters();

  private:

     DetectorMessenger*  fDetectorMessenger;
     G4VPhysicalVolume*  fpWorld;
     G4LogicalVolume*    flTarget;   // pointer to the logical Target
     G4LogicalVolume*    flTracker;    // pointer to the physical Tracker
     G4VPhysicalVolume*  fpTracker;  // pointer to the physical Tracker

     G4Material*         fTargetMater;  // pointer to the target  material
     G4Material*         fTrackerMater; // pointer to the tracker material
     G4Material*         fWorldMater;   // pointer to the tracker material
   
     G4double fTargetLength;           // half length of target
     G4double fTargetDepth;            // half depth of target
    

    
    
  private:

     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

