/////////////////////////////////////////////////////////////////
//*-- AUTHOR : Hector Alvarez Pol     hapol@fpddux.usc.es
//*-- Date: 05/2005
//*-- Last Update: 25/10/06 by Hector Alvarez Pol
// --------------------------------------------------------------
// Description:
//   The ACTAR Silicon detectorpart of the ROOT Analysis
//
// --------------------------------------------------------------
// Comments:
//
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#include "ActarSimROOTAnalParis.hh"

#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4TrackStatus.hh"
#include "G4Step.hh"
#include "G4Types.hh"

//ROOT INCLUDES
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"

ActarSimROOTAnalParis::ActarSimROOTAnalParis() {
  //
  // Constructor
  //

  //The simulation file
  simFile = ((ActarSimROOTAnalysis*)gActarSimROOTAnalysis)->GetSimFile();
  simFile->cd();

  //The tree
  eventTree = ((ActarSimROOTAnalysis*)gActarSimROOTAnalysis)->GetEventTree();

  //The clones array of Sci Hits
  //TODO-> Repeat here the trick SimpleHit/HitSim/Hit that is existing in ActarSimAnalParis.cc
  parisHitCA = new TClonesArray("ActarSimParisHit",2);

  parisHitsBranch = eventTree->Branch("parisHits",&parisHitCA);
  parisHitsBranch->SetAutoDelete(kTRUE);

}


ActarSimROOTAnalParis::~ActarSimROOTAnalParis() {
  //
  // Destructor
  //

}


void ActarSimROOTAnalParis::GeneratePrimaries(const G4Event *anEvent){
  //
  // Actions to perform in the paris anal when generating the primaries
  //

  if(anEvent){;} /* keep the compiler "quiet" */
}


void ActarSimROOTAnalParis::BeginOfRunAction(const G4Run *aRun) {
  //
  // Actions to perform in the parisicon anal at the begining of the run
  //
  if (aRun){;} /* keep the compiler "quiet" */

  //Storing the runID
  SetTheRunID(aRun->GetRunID());

  char newDirName[255];
  sprintf(newDirName,"%s%i","Histos",aRun->GetRunID());
  simFile->cd(newDirName);

  dirName = new char[255];

  sprintf(dirName,"%s","Paris");
  gDirectory->mkdir(dirName,dirName);
  gDirectory->cd(dirName);

  simFile->cd();
}



void ActarSimROOTAnalParis::BeginOfEventAction(const G4Event *anEvent) {
  //
  // Actions to perform in the parisicon anal at the begining of the event
  //

  SetTheEventID(anEvent->GetEventID());
}


void ActarSimROOTAnalParis::EndOfEventAction(const G4Event *anEvent) {
  //
  // Actions to perform in the parisicon anal at the beginning of the run
  // Defining the ActarSimParisHit from the ActarSimParisGeantHits
  //

  FillingHits(anEvent);
}


void ActarSimROOTAnalParis::FillingHits(const G4Event *anEvent) {
  //
  // Defining the ActarSimParisHits from the ActarSimParisGeantHits
  //
  // A simple algorithm checking the number of primaries
  // reaching the Paris and calculating their mean parameters
  // taking into account their energy deposition on the parisicons
  // NOTE that only primaries can produce Hits following this scheme
  //
  //Hit Container ID for ActarSimParisGeantHit
  G4int hitsCollectionID =
    G4SDManager::GetSDMpointer()->GetCollectionID("ParisCollection");

  G4HCofThisEvent* HCofEvent = anEvent->GetHCofThisEvent();

  ParisSingleHitsCollection* hitsCollection =
    (ParisSingleHitsCollection*) HCofEvent->GetHC(hitsCollectionID);

  //Number of ParisSingleHit (or steps) in the hitsCollection
  G4int NbHits = hitsCollection->entries();
  //G4cout<<"# Paris Hits "<< NbHits<<G4endl;
  G4int indepHits = 0; //number of Hits
  G4int* trackIDtable; //stores the trackID of primary particles for each (valid) GeantHit
  trackIDtable = new G4int[NbHits];
  G4int* detIDtable;   //stores the detIDs for each (valid) GeantHit
  detIDtable = new G4int[NbHits];
  G4int* IDtable;         //stores the order in previous array for each (valid) GeantHit
  IDtable = new G4int[NbHits];

  for (G4int i=0;i<NbHits;i++) {
    //G4cout<<i<<" Paris Hit "<<(*hitsCollection)[i]->GetParentID()<<" "<<(*hitsCollection)[i]->GetDetID()<<" "<<(*hitsCollection)[i]->GetEdep()/MeV<<G4endl;
    if((*hitsCollection)[i]->GetParentID()>=0) { //step from primary
      if(indepHits==0) { //only for the first Hit
	trackIDtable[indepHits] = (*hitsCollection)[i]->GetTrackID();
	detIDtable[indepHits] = (*hitsCollection)[i]->GetDetID();
	IDtable[i] = indepHits;
	indepHits++;
      }
      else { // this part is never reached. Maybe because there is always only one indepHits that has parentID equals to 0.
	for(G4int j=0; j<indepHits;j++) {
	  if( (*hitsCollection)[i]->GetTrackID() == trackIDtable[j] &&
	      (*hitsCollection)[i]->GetDetID() == detIDtable[j]) { //checking trackID and detID
	    IDtable[i] = j;
	    break; //not a new Hit
	  }
	  if(j==indepHits-1){ //we got the last hit and there was no match!
	    trackIDtable[indepHits] = (*hitsCollection)[i]->GetTrackID();
	    detIDtable[indepHits] = (*hitsCollection)[i]->GetDetID();
	    IDtable[i] = indepHits;
	    indepHits++;
	  }
	}
      }
    }
  }

  //G4cout<<"# Independent PARIS hits "<<indepHits<<G4endl;
  //Let us create as many ActarSimSilHit as independent primary particles
  theParisHit = new ActarSimParisHit*[indepHits];
  for (G4int i=0;i<indepHits;i++)
    theParisHit[i] = new ActarSimParisHit();

  //Clear the ClonesArray before filling it
  parisHitCA->Clear();

  //a variable to check if the Hit was already created
  G4int* existing;
  existing = new G4int[indepHits];
  for(G4int i=0;i<indepHits;i++) existing[i] = 0;

  for(G4int i=0;i<NbHits;i++) {
    if( (*hitsCollection)[i]->GetParentID()>=0 ) { //step from primary
      //the IDtable[i] contains the order in the indepHits list
      if( existing[IDtable[i]]==0) { //if the indepHits does not exist
	AddParisHit(theParisHit[IDtable[i]],(*hitsCollection)[i],0);
	existing[IDtable[i]] = 1;
      }
      else
	AddParisHit(theParisHit[IDtable[i]],(*hitsCollection)[i],1);
    }
  }

  //at the end, fill the ClonesArray
  for (G4int i=0;i<indepHits;i++){
    //G4cout<<"ActarSimROOTAnalParis----------->FillingHits() "<<parisHitCA<<G4endl;
    new((*parisHitCA)[i])ActarSimParisHit(*theParisHit[i]);
    //G4cout<<"ActarSimROOTAnalParis----------->FillingHits() "<<silHitCA<<G4endl;
  }
  delete [] trackIDtable;
  delete [] IDtable;
  delete [] existing;
  delete [] detIDtable;
  for (G4int i=0;i<indepHits;i++) delete theParisHit[i];
  delete [] theParisHit;
}

void ActarSimROOTAnalParis::AddParisHit(ActarSimParisHit* cHit,
			       ParisSingleHit* gHit,
			       G4int mode) {
  //
  // Function to move the information from the ActarSimParisGeantHit (a step hit)
  // to ActarSimParisHit (a simple primary representation in the silicon)
  // Two modes are possible:
  // mode == 0 : creation; the ActarSimParisHit is void and is
  //             filled by the data from the ActarSimParisGeantHit
  // mode == 1 : addition; the ActarSimParisHit was already created
  //             by other ActarSimParisGeantHit and some data members are updated

  if(mode == 0) { //creation
    cHit->SetDetectorID(gHit->GetDetID());
    cHit->SetMotherID(gHit->GetMotherID());


    cHit->SetXPos(gHit->GetPos().x()/mm);
    cHit->SetYPos(gHit->GetPos().y()/mm);
    cHit->SetZPos(gHit->GetPos().z()/mm);

    cHit->SetTime(gHit->GetToF()/ns);
    cHit->SetEnergy(gHit->GetEdep()/MeV);

    cHit->SetTrackID(gHit->GetTrackID());
    cHit->SetEventID(GetTheEventID());
    cHit->SetRunID(GetTheRunID());

    cHit->SetParticleID(gHit->GetPDGcode());
    cHit->SetDetectorName(gHit->GetDetName());
    cHit->SetProcessName(gHit->GetProcessName());
    //cHit->SetParticleCharge(gHit->GetParticleCharge());
    //cHit->SetParticleMass(gHit->GetParticleMass());

    //cHit->SetStepsContributing(1);

  }

  else if(mode==1){ //addition
    cHit->SetEnergy(cHit->GetEnergy() + gHit->GetEdep());

    //cHit->SetStepsContributing(cHit->GetStepsContributing()+1);
    // The mean value of a distribution {x_i} can also be computed iteratively
    // if the values x_i are drawn one-by-one. After a new value x, the new mean is:
    // mean(t) = mean(t-1) + (1/t)(x-mean(t-1))
    //cHit->SetXPos(cHit->GetXPos() +
    //		  (gHit->GetLocalPrePos().x()-cHit->GetXPos())/((G4double)cHit->GetStepsContributing()));
    //cHit->SetYPos(cHit->GetYPos() +
    //		  (gHit->GetLocalPrePos().y()-cHit->GetYPos())/((G4double)cHit->GetStepsContributing()));
    //cHit->SetZPos(cHit->GetZPos() +
    //		  (gHit->GetLocalPrePos().z()-cHit->GetZPos())/((G4double)cHit->GetStepsContributing()));

    //taking the shorter time of the geantHits
    if(cHit->GetTime()>gHit->GetToF()) cHit->SetTime(gHit->GetToF());
  }
}


void ActarSimROOTAnalParis::UserSteppingAction(const G4Step *aStep){
  //
  // Actions to perform in the ACTAR gas detector analysis after each step
  //

  if(aStep){;} /* keep the compiler "quiet" */

}
