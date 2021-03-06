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
/// \file electromagnetic/TestEm9/include/PhysicsList.hh
/// \brief Definition of the PhysicsList class
//
// $Id: PhysicsList.hh 66241 2012-12-13 18:34:42Z gunter $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//---------------------------------------------------------------------------
//
// ClassName:   PhysicsList
//
// Author:      V.Ivanchenko 14.10.2002
//
// Modified:
// 17.11.06 Use components from physics_lists subdirectory (V.Ivanchenko)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class StepMax;
class PhysicsListMessenger;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();

  virtual void ConstructParticle();

  virtual void ConstructProcess();

  virtual void SetCuts();

  void AddPhysicsList(const G4String& name);
  void AddStepMax();

  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForProton(G4double);

  void SetVertexCut(G4double val);
  void SetMuonCut(G4double val);

private:

  // hide assignment operator
  PhysicsList & operator=(const PhysicsList &right);
  PhysicsList(const PhysicsList&);

  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;
  G4double fCutForProton;
  G4double fCutForVertexDetector;
  G4double fCutForMuonDetector;

  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fDecayPhysicsList;

  std::vector<G4VPhysicsConstructor*> fHadronPhys;
  G4String fEmName;

  StepMax* fStepMaxProcess;

  PhysicsListMessenger* fMessenger;
  G4ProductionCuts* fVertexDetectorCuts;
  G4ProductionCuts* fMuonDetectorCuts;

  G4bool fHelIsRegisted;
  G4bool fBicIsRegisted;
  G4bool fGnucIsRegisted;
  G4bool fStopIsRegisted;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

