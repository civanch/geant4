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
// $Id: G4He3EvaporationProbability.cc 67983 2013-03-13 10:42:03Z gcosmo $
//
// J.M. Quesada (August2008). Based on:
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//
// Modified:
// 03-09-2008 J.M. Quesada for external choice of inverse cross section option
// 17-11-2010 V.Ivanchenko integer Z and A

#include "G4He3EvaporationProbability.hh"
#include "G4SystemOfUnits.hh"

G4He3EvaporationProbability::G4He3EvaporationProbability() :
   G4EvaporationProbability(3,2,2,&theCoulombBarrier) // A,Z,Gamma,&theCoulombBarrier
{
  ResidualA = ResidualZ = theA = theZ = FragmentA = 0;
  ResidualAthrd = FragmentAthrd = 0.0;
}

G4He3EvaporationProbability::~G4He3EvaporationProbability() 
{}

G4double G4He3EvaporationProbability::CalcAlphaParam(const G4Fragment & fragment)  
  { return 1.0 + CCoeficient(fragment.GetZ_asInt()-GetZ());}
	
G4double G4He3EvaporationProbability::CalcBetaParam(const G4Fragment & )  
  { return 0.0; }


G4double G4He3EvaporationProbability::CCoeficient(G4int aZ) 
{
  // Data comes from 
  // Dostrovsky, Fraenkel and Friedlander
  // Physical Review, vol 116, num. 3 1959
  // 
  // const G4int size = 5;
  // G4double Zlist[5] = { 10.0, 20.0, 30.0, 50.0, 70.0};
  //	G4double Calpha[5] = { 0.10, 0.10, 0.10, 0.08, 0.06};
  // C for He3 is equal to C for alpha times 4/3
  G4double C = 0.0;
	
  if (aZ <= 30)
    {
      C = 0.10;
    }
  else if (aZ <= 50)
    {
      C = 0.1 - (aZ - 30)*0.001;
    }
  else if (aZ < 70)
    {
      C = 0.08 - (aZ - 50)*0.001;
    }
  else
    {
      C = 0.06;
    }
  return C*(4.0/3.0);
}

///////////////////////////////////////////////////////////////////////////////////
//J. M. Quesada (Dec 2007-June 2008): New inverse reaction cross sections 
//OPT=0 Dostrovski's parameterization
//OPT=1,2 Chatterjee's paramaterization 
//OPT=3,4 Kalbach's parameterization 
// 
G4double 
G4He3EvaporationProbability::CrossSection(const  G4Fragment & fragment, G4double K)
{

  theA=GetA();
  theZ=GetZ();
  ResidualA=fragment.GetA_asInt()-theA;
  ResidualZ=fragment.GetZ_asInt()-theZ; 
  
  ResidualAthrd=fG4pow->Z13(ResidualA);
  FragmentA=fragment.GetA_asInt();
  FragmentAthrd=fG4pow->Z13(FragmentA);

  if (OPTxs==0) {std::ostringstream errOs;
  errOs << "We should'n be here (OPT =0) at evaporation cross section calculation (He3's)!!"  
	<<G4endl;
  throw G4HadronicException(__FILE__, __LINE__, errOs.str());
  return 0.;}
  if( OPTxs==1 || OPTxs==2) return G4He3EvaporationProbability::GetOpt12( K);
  else if (OPTxs==3 || OPTxs==4)  return G4He3EvaporationProbability::GetOpt34( K);
  else{
    std::ostringstream errOs;
    errOs << "BAD He3's CROSS SECTION OPTION AT EVAPORATION!!"  <<G4endl;
    throw G4HadronicException(__FILE__, __LINE__, errOs.str());
    return 0.;
  }
}

//********************* OPT=1,2 : Chatterjee's cross section *****************
//(fitting to cross section from Bechetti & Greenles OM potential)

G4double G4He3EvaporationProbability::GetOpt12(const  G4double K)
{
  G4double Kc = K;

  // JMQ xsec is set constat above limit of validity
  if (K > 50*MeV) { Kc = 50*MeV; }

  G4double landa ,mu ,nu ,p , Ec,q,r,ji,xs;

  G4double     p0 = -3.06;
  G4double     p1 = 278.5;
  G4double     p2 = -1389.;
  G4double     landa0 = -0.00535;
  G4double     landa1 = -11.16;
  G4double     mum0 = 555.5;
  G4double     mu1 = 0.40;
  G4double     nu0 = 687.4;
  G4double     nu1 = -476.3;
  G4double     nu2 = 0.509;    
  G4double     delta=1.2;              

  Ec = 1.44*theZ*ResidualZ/(1.5*ResidualAthrd+delta);
  p = p0 + p1/Ec + p2/(Ec*Ec);
  landa = landa0*ResidualA + landa1;

  G4double resmu1 = fG4pow->powZ(ResidualA,mu1); 
  mu = mum0*resmu1;
  nu = resmu1*(nu0 + nu1*Ec + nu2*(Ec*Ec));
  q = landa - nu/(Ec*Ec) - 2*p*Ec;
  r = mu + 2*nu/Ec + p*(Ec*Ec);
  
  ji=std::max(Kc,Ec);
  if(Kc < Ec) { xs = p*Kc*Kc + q*Kc + r;}
  else {xs = p*(Kc - ji)*(Kc - ji) + landa*Kc + mu + nu*(2 - Kc/ji)/ji ;}
  
  if (xs <0.0) {xs=0.0;}
              
  return xs;

}

// *********** OPT=3,4 : Kalbach's cross sections (from PRECO code)*************
G4double G4He3EvaporationProbability::GetOpt34(const  G4double K)
//c     ** 3he from o.m. of gibson et al
{
  G4double landa, mu, nu, p , signor(1.),sig;
  G4double ec,ecsq,xnulam,etest(0.),a; 
  G4double b,ecut,cut,ecut2,geom,elab;

  G4double     flow = 1.e-18;
  G4double     spill= 1.e+18;

  G4double     p0 = -2.88;
  G4double     p1 = 205.6;
  G4double     p2 = -1487.;
  G4double     landa0 = 0.00459;
  G4double     landa1 = -8.93;
  G4double     mum0 = 611.2;
  G4double     mu1 = 0.35;
  G4double     nu0 = 473.8;
  G4double     nu1 = -468.2;
  G4double     nu2 = -2.225;      
  
  G4double      ra=0.80;
        
  //JMQ 13/02/09 increase of reduced radius to lower the barrier
  // ec = 1.44 * theZ * ResidualZ / (1.5*ResidualAthrd+ra);
  ec = 1.44 * theZ * ResidualZ / (1.7*ResidualAthrd+ra);
  ecsq = ec * ec;
  p = p0 + p1/ec + p2/ecsq;
  landa = landa0*ResidualA + landa1;
  a = fG4pow->powZ(ResidualA,mu1);
  mu = mum0 * a;
  nu = a* (nu0+nu1*ec+nu2*ecsq);  
  xnulam = nu / landa;
  if (xnulam > spill) { xnulam=0.; }
  if (xnulam >= flow) { etest = 1.2 *std::sqrt(xnulam); }
  
  a = -2.*p*ec + landa - nu/ecsq;
  b = p*ecsq + mu + 2.*nu/ec;
  ecut = 0.;
  cut = a*a - 4.*p*b;
  if (cut > 0.) ecut = std::sqrt(cut);
  ecut = (ecut-a) / (p+p);
  ecut2 = ecut;
  //JMQ 290310 for avoiding unphysical increase below minimum (at ecut)
  // ecut<0 means that there is no cut with energy axis, i.e. xs is set
  // to 0 bellow minimum
  //  if (cut < 0.) ecut2 = ecut - 2.;
  if (cut < 0.) { ecut2 = ecut; }
  elab = K * FragmentA /G4double(ResidualA);
  sig = 0.;
  
  if (elab <= ec) { //start for E<Ec
    if (elab > ecut2) { sig = (p*elab*elab+a*elab+b) * signor; }
  }           //end for E<Ec
  else {           //start for E>Ec
    sig = (landa*elab+mu+nu/elab) * signor;
    geom = 0.;
    if (xnulam < flow || elab < etest) { return sig; }
    geom = std::sqrt(theA*K);
    geom = 1.23*ResidualAthrd + ra + 4.573/geom;
    geom = 31.416 * geom * geom;
    sig = std::max(geom,sig);
  }           //end for E>Ec
  return sig;
  
}
