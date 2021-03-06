/////////////////////////////////////////////////////////////////
//*-- AUTHOR : Hector Alvarez-Pol  hapol@fpddux.usc.es
//*-- Date: 11/2004
//*-- Last Update: 28/10/05
// --------------------------------------------------------------
// Description:
//   General physics package
//
// --------------------------------------------------------------
// Comments:
//   
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#ifndef ActarSimPhysicsList_h
#define ActarSimPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class ActarSimPhysicsListMessenger;
class G4StepLimiterBuilder;

//DPL
class G4VPhysicsConstructor;

class ActarSimPhysicsList: public G4VModularPhysicsList {

private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4int    verbose;
  G4bool   emBuilderIsRegisted;
  G4bool   decayIsRegisted;
  G4bool   stepLimiterIsRegisted;
  G4bool   helIsRegisted;
  G4bool   bicIsRegisted;
  G4bool   ionIsRegisted;
  G4bool   gnucIsRegisted;
  
  ActarSimPhysicsListMessenger* pMessenger;
  G4StepLimiterBuilder* steplimiter;
  
  //DPL
  G4VPhysicsConstructor*  emPhysicsList;
 
public:
  ActarSimPhysicsList();
  ~ActarSimPhysicsList();
  
  // SetCuts()   
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);

  void AddPhysicsList(const G4String&);
  void SetVerbose(G4int val);
};

#endif



