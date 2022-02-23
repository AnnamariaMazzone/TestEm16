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
/// \file electromagnetic/TestEm16/TestEm16.cc
/// \brief Main program of the electromagnetic/TestEm16 example
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Types.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4SystemOfUnits.hh"
//#include "HistoManager.hh"
#include "G4AnalysisManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

extern int fGammaEmessi;
int main(int argc,char** argv) {
    
    //detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc,argv);
    
    //Use SteppingVerbose with Unit
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);
    
    //Creating run manager
    auto runManager = G4RunManagerFactory::CreateRunManager();
    
    if (argc==3) {
        G4int nThreads = G4UIcommand::ConvertToInt(argv[2]);
        runManager->SetNumberOfThreads(nThreads);
    }
    
    //set mandatory initialization classes
    DetectorConstruction* det;
    runManager->SetUserInitialization(det = new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization(det));
    
    //initialize visualization
    G4VisManager* visManager = nullptr;
    
    //get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/macroPath /Users/annamaria/lavoro/xray_shielding/TestEm16");
    UImanager->ApplyCommand("/control/execute run01.mac");
    // start a run
    int numberOfEvent = 1000000;
    
    //    G4double lim_inf_S=0.*mm;
    G4double lim_sup_S=50*mm;
    G4double fSpessore=20*mm;
    G4double step_S= 5*mm; //(lim_sup_S-lim_inf_S)/20.;
    
    det->SetTargetSize(fSpessore);
    runManager->ReinitializeGeometry();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("PS50_W45_CNT5_100keV");
    analysisManager->OpenFile();
    analysisManager->CreateH2("spessore","PS50_W45_CNT5_100keV", 100, 0*mm, 60*mm, 100, 0., 1);
    
    while (fSpessore<=lim_sup_S) {
        runManager->SetUserInitialization(new ActionInitialization(det));
        runManager->Initialize();
        runManager->BeamOn(numberOfEvent);
        G4double yvalue=(G4double)fGammaEmessi/numberOfEvent;
        analysisManager->FillH2(0,fSpessore,yvalue);
        fSpessore=fSpessore+step_S;
        det->SetTargetSize(fSpessore);
        runManager->ReinitializeGeometry();
    }
    //save histograms
    
  //  if ( analysisManager->IsActive() ) {
      analysisManager->Write();
      analysisManager->CloseFile();
  //  }
     
    if (ui)  {
        //interactive mode
        visManager = new G4VisExecutive;
        visManager->Initialize();
        
        // define icons before SessionStart
        //    if (ui->IsGUI()) { UImanager->ApplyCommand("/control/execute gui.mac"); }
        ui->SessionStart();
        delete ui;
    } else  {
        //batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    
    //job termination
    delete visManager;
    delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
