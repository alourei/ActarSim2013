/////////////////////////////////////////////////////////////////
//*-- AUTHOR : David Perez-Loureiro    loureiro@ganil.fr
//*-- Date: 02/2013
//*-- Last Update: 06/02/13 by D. Perez-Loureiro
// --------------------------------------------------------------
// Description:
//   Paris detector description
//
// --------------------------------------------------------------
// Comments:
//
//   - 06/02/12 Adding PARIS clusters to ACTARSim
//
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#include "ActarSimParisDetectorConstruction.hh"
#include "ActarSimDetectorConstruction.hh"
#include "ActarSimParisDetectorMessenger.hh"
#include "ActarSimROOTAnalysis.hh"
#include "ActarSimSilSD.hh"
#include "ParisTrackerSD.hh"
using namespace std;

ActarSimParisDetectorConstruction::ActarSimParisDetectorConstruction(ActarSimDetectorConstruction* det)
  :detConstruction(det)
 
{
G4cout<<"\nDETECTOR#######################\n"<<flush;

  SetSideCoverage(1);

  SetXBoxParisHalfLength(100*cm);
  SetYBoxParisHalfLength(100*cm);
  SetZBoxParisHalfLength(100*cm);

  // create commands for interactive definition of the calorimeter
  parisMessenger = new ActarSimParisDetectorMessenger(this);

}


ActarSimParisDetectorConstruction::~ActarSimParisDetectorConstruction()
{
}

G4VPhysicalVolume* ActarSimParisDetectorConstruction::Construct(G4LogicalVolume* worldLog) {
  //
  // Wrap for the construction functions within the Silicon
  //

  //Introduce here other constructors for materials around the TOF (windows, frames...)
  //which can be controlled by the calMessenger
  //ConstructTOFWorld(worldLog);
  return ConstructParis(worldLog);
}





G4VPhysicalVolume* ActarSimParisDetectorConstruction::ConstructParis(G4LogicalVolume* worldLog){

  //________MATERIALS DEFINITION:::___________________
  //__________________________________________________
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  
  //***Elements ______________________________________________________________________________
  
  G4Element* H  = new G4Element( "H"  , "H"  , z=1. , a=  1.01*g/mole );
  G4Element* C  = new G4Element( "C"  , "C"  , z=6. , a= 12.01*g/mole );
  G4Element* N  = new G4Element( "N"  , "N"  , z=7. , a= 14.01*g/mole );
  G4Element* O  = new G4Element( "O"  , "O"  , z=8. , a= 16.00*g/mole );
  G4Element* Si = new G4Element("Si"  , "Si" , z=14., a= 28.08*g/mole );
  G4Element* La = new G4Element("La"  , "La" , z=57., a=138.91*g/mole );
  G4Element* Br = new G4Element("Br"  , "Br" , z=35., a= 79.90*g/mole );
  G4Element* Na = new G4Element("Na"  , "Na" , z=11., a= 22.99*g/mole );
  G4Element* I  = new G4Element( "I"  , "I"  , z=53., a=126.90*g/mole );
  //  G4Element* Li = new G4Element("Li"  , "Li" , z=3. , a=  6.94*g/mole );
  //  G4Element* Ge = new G4Element("Ge"  , "Ge" , z=32., a= 72.64*g/mole );
  //******************************************************************************************
  
  //***Materials /////////////////////////////////////////

  //Aluminum
  G4Material* Al = new G4Material ("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);
  
  //Cooper
  G4Material* Cu = new G4Material ("Cu",z=29.0,a=63.55*g/mole,density=8.96*g/cm3);
  
  //Vacuum
  G4Material* Vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
				      density=universe_mean_density,kStateGas,0.1*kelvin,
				      1.e-19*pascal); 
  
  //Air
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, 2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);
  
  //SiLi
  G4Material* SiLi = new G4Material ("SiLi", density= 2.330*g/cm3 , 1);
  SiLi -> AddElement (Si, 100.*perCent);
  
  //Gold
  G4Material* Gold = new G4Material ("Gold", z=79., a= 196.967*g/mole, density= 19.3*g/cm3);
  
  //HPGe
  G4Material* HPGe = new G4Material ("HPGe", z=32., a= 72.59*g/mole, density= 5.33*g/cm3);
  
  //LaBr3
  G4Material* LaBr3 = new G4Material ("LaBr3", density= 5.29*g/cm3 , 2);
  LaBr3->AddElement( La, 1);
  LaBr3->AddElement( Br, 3);
  
  //NaI
  G4Material* NaI = new G4Material ("NaI", density= 3.67*g/cm3 , 2);
  NaI->AddElement( Na, 1);
  NaI->AddElement(  I, 3);
  
  //Ti
  G4Material* Ti = new G4Material ("Ti", z=22., a= 47.867*g/mole, density = 4.506*g/cm3);
  
  //Iron
  G4Material* Iron = new G4Material ("Iron", z=26., a=55.845*g/mole, density = 7.86*g/cm3);
  
  //Kapton
  G4Material* Kapton = new G4Material ("Kapton", density=1.42*g/cm3, 4);
  Kapton -> AddElement ( C, 22 );
  Kapton -> AddElement ( H, 10 );
  Kapton -> AddElement ( N,  2 );
  Kapton -> AddElement ( O,  4 );
  
  //Bakelite
  G4Material* Bakelite = new G4Material ("Bakelite", density=1.45*g/cm3, 3 );
  Bakelite -> AddElement ( H, 9 );
  Bakelite -> AddElement ( C, 9 );
  Bakelite -> AddElement ( O, 1 );
  
  //Glass
  G4Material* Glass = new G4Material("Glass", density=1.032*g/cm3,2);
  Glass->AddElement(C,91.533*perCent);
  Glass->AddElement(H,8.467*perCent);
  
  //______ END OF MATERIALS DEFINITION _______________________________________
  ////////////////////////////////////////////////////////////////////////////
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // ______________________________ COLOURS ______________________________________
  
  G4Colour red		(1.,0.,0.);			// Define red color
  G4Colour green		(0.,1.,0.);		// Define green color
  G4Colour blue		(0.,0.,1.);		// Define blue color
  G4Colour violet	(1.,0.,1.);
  G4Colour turquoise	(0.,1.,1.);
  G4Colour white		(1.,1.,1.);
  G4Colour yellow	(1.,1.,0.);
  G4Colour orange	(1.,0.5,0.);
  G4Colour grey		(0.5,0.5,0.5);
  G4VisAttributes attred(red);        	// Define a red visualization attribute
  G4VisAttributes attgreen(green);
  G4VisAttributes attblue(blue);
  G4VisAttributes attviolet(violet);
  G4VisAttributes attturq(turquoise);
  G4VisAttributes* attturq_filled = new G4VisAttributes(G4Colour( 0., 1., 1., 1.));
  attturq_filled->SetForceSolid(true);
  G4VisAttributes attwhite(white);
  G4VisAttributes attyellow(yellow);
  G4VisAttributes attorange(orange);
  G4VisAttributes attgrey(grey);
  //////////////////////////////////////////////////////////////////////////////////
  
  
  
  //______________________________________________________________________________
  // --------------------------------------- VOLUMES ------------------------------
  //_______________________________________________________________________________
  
  
  
  //_________________________________ EXPERIMENTAL HALL ______________________________
  G4VPhysicalVolume *Paris_phys = new G4PVPlacement	(0,
							 G4ThreeVector(0.,0.,0.),worldLog,
							 "ParisPhys", 0, false, 10);
  
  // __________________________ END OF EXPERIMENTAL HALL ______________________________
  
  G4double cellX =  56.*mm; 
  G4double cellY =  56.*mm; 
  G4double cellZ = 226.*mm; 
  
  //--------------------------- the PARIS cluster ---------------------------------------
  //  dummy volume (Vacuum) to define the PARIS cluster ( 3 x 3 detectors... ) 
  
  G4double clustX =  56.*3.*mm; 
  G4double clustY =  56.*3.*mm; 
  G4double clustZ = 226.   *mm; 
  G4Box* cluster_box = new G4Box("cluster_box", clustX/2., clustY/2., clustZ/2. ); 
  G4LogicalVolume* cluster_log = new G4LogicalVolume ( cluster_box, Vacuum, "cluster_log"); 
  cluster_log->SetVisAttributes(G4VisAttributes::Invisible); 
  
  int j=0;
  if(sideCoverage & 0x0001){ // bit1 (lsb) beam output wall
    for(int i=8; i<12; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      rotateIT.rotateX( 180*deg );    
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }   
  }
if((sideCoverage >> 1) & 0x0001){ // bit2 lower [bottom] (gravity based) wall
    for(int i=16; i<20; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      rotateIT.rotateX( -90*deg );
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }
 }
if((sideCoverage >> 2) & 0x0001){ // bit3 upper [top] (gravity based) wall
    for(int i=4; i<8; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      rotateIT.rotateX( 90*deg );
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }
}
if((sideCoverage >> 3) & 0x0001){ // bit4 left (from beam point of view) wall
    for(int i=0; i<4; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      rotateIT.rotateY( -90*deg );
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }
}
if((sideCoverage >> 4) & 0x0001){ // right (from beam point of view) wall
    for(int i=12; i<16; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      rotateIT.rotateY( 90*deg );
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }
}
if((sideCoverage >> 5) & 0x0001){ // bit6 (msb) beam entrance wall
    for(int i=20; i<24; ++i){
      G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i);
      G4RotationMatrix rotateIT;
      //rotateIT.rotateY( 90*deg );
      G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
						     cluster_log, "cluster",worldLog, false, j++);
    }
}
    // for(int i=0; i<24; ++i) 
  //   {
  //     G4ThreeVector pos = ActarSimParisGeometry::GetClusterPos(i); 
  //     G4RotationMatrix rotateIT;
  //           if ( i<4 )
  //               rotateIT.rotateY( -90*deg );	// for FACE #1 (@ +X) 
  //           else if ( i<8 )	
  //               rotateIT.rotateX(  90*deg );	// for FACE #2 (@ +Y)
  //           else if ( i<12 )	
  //               rotateIT.rotateX( 180*deg );	// for FACE #3 (@ +Z)
  //           else if ( i<16 ) 
  //               rotateIT.rotateY(  90*deg );	// for FACE #4 (@ -X) 
  //           else if ( i<20 )	
  //               rotateIT.rotateX( -90*deg );	// for FACE #5 (@ -Y)
  //           else 
  //               	;	// NO ROTATION!!!  for FACE #6 (@ -Z) 

  //    G4VPhysicalVolume* cluster = new G4PVPlacement(G4Transform3D( rotateIT, pos),
  //    					cluster_log, "cluster",worldLog, false, j++); 
  //   }


//--------------------------- 9 detectors inside each cluster ---------------------------

G4Box* parcell = new G4Box("parcell", cellX/2., cellY/2., cellZ/2. );	// parcell = PARIS CELL 
G4LogicalVolume* parcell_log = new G4LogicalVolume ( parcell, Vacuum, "parcell_log"); 
		 parcell_log->SetVisAttributes(G4VisAttributes::Invisible); 
    int k=0;
    for(int l=0; l<9; ++l) 
    {
     G4ThreeVector detpos = ActarSimParisGeometry::GetDetPos(l); 
     G4RotationMatrix unrotated;
     G4VPhysicalVolume* detector = new G4PVPlacement(G4Transform3D( unrotated, detpos),
     					parcell_log, "detector", cluster_log, false, k++); 
    }


//--------------------------- Aluminum outer can ---------------------------------------

G4double i_wall[]  = {   0.0*mm,  27.5*mm,  27.5*mm,   27.5*mm,   28.0*mm };
G4double o_wall[]  = {  28.0*mm,  28.0*mm,  28.0*mm,   28.0*mm,   28.0*mm };
G4double z_wall[]  = { 113.0*mm, 112.5*mm, 112.5*mm, -113.0*mm, -113.0*mm };	
G4Polyhedra* p_wall = new G4Polyhedra ("p_wall",	
				   45.0*degree,	360.0*degree, 4, 5,	
				   z_wall, i_wall, o_wall ); 
G4LogicalVolume* wall_log = new G4LogicalVolume ( p_wall, Al, "wall_log"); 
		 wall_log -> SetVisAttributes(attwhite);
G4VPhysicalVolume* wall = new G4PVPlacement ( 0, G4ThreeVector(.0, .0, .0*mm),
					         wall_log, "wall", parcell_log, false, 0); 


//--------------------------- LaBr3 crystal --------------------------------------------

G4double brX = 52.*mm; 
G4double brY = 52.*mm; 
G4double brZ = 50.8*mm; 
G4Box* brbox = new G4Box("brbox", brX/2., brY/2., brZ/2. ); 
G4LogicalVolume* br_log = new G4LogicalVolume ( brbox, LaBr3, "br_log"); 
		 br_log -> SetVisAttributes(attred); 
G4VPhysicalVolume* LaBr = new G4PVPlacement ( 0, G4ThreeVector(.0, .0, (cellZ/2.-2.-brZ/2.)*mm),
					         br_log, "LaBr", parcell_log, false, 0); 


//------------------------------ NaI crystal -------------------------------------------

G4double naiZ = 152.4*mm;	// X & Y are the same as for the LaBr3 
G4Box* naibox = new G4Box("naibox", brX/2., brY/2., naiZ/2. ); 
G4LogicalVolume* nai_log = new G4LogicalVolume ( naibox, NaI, "nai_log"); 
		 nai_log -> SetVisAttributes(attblue); 
G4VPhysicalVolume* NaIcr = new G4PVPlacement ( 0, G4ThreeVector(.0, .0, (cellZ/2.-2.-brZ-naiZ/2.)*mm),
					         nai_log, "NaIcr", parcell_log, false, 0); 


//------------------------------ Light guide (See Saint-Gobain specs...) ---------------

G4double lgX = 49.*mm; 
G4double lgY = 49.*mm; 
G4double lgZ =  5.*mm; 
G4Box* lgbox = new G4Box("lgbox", lgX/2., lgY/2., lgZ/2. ); 
G4LogicalVolume* lg_log = new G4LogicalVolume ( lgbox, Glass, "lg_log");  // MATERIAL to be changed!!!!!!!!
		 lg_log -> SetVisAttributes(attgreen); 
G4VPhysicalVolume* LG = new G4PVPlacement ( 0, G4ThreeVector(.0, .0, (cellZ/2.-2.-brZ-naiZ-lgZ/2.)*mm),
					         lg_log, "LG", parcell_log, false, 0); 




//__________________________________________________________________________________
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

 ParisTrackerSD* aTrackerSD = detConstruction->GetParisSD();
  //SDman->AddNewDetector( aTrackerSD ); 
 //G4cout<<"###########HERE############"<<aTrackerSD<<G4endl;
 
  br_log -> SetSensitiveDetector( aTrackerSD ); 
  nai_log -> SetSensitiveDetector( aTrackerSD ); 
  
  
//__________________________________________________________________________________



  return Paris_phys;
  
}


void ActarSimParisDetectorConstruction::PrintDetectorParameters() {
  //
  // Prints Paris detector parameters. To be filled
  //

  G4cout << "##################################################################"
	 << G4endl
	 << "####  ActarSimParisDetectorConstruction::PrintDetectorParameters() ####"
	 << G4endl;
  G4cout << "##################################################################"
	 << G4endl;
}
