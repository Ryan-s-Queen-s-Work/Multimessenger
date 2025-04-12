//Supernova in argon detectors - Michela Lai - Lucy Kotsioupolus - Shawn Westerdale

//load libraries
#include "TFile.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TMath.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include <string>
#include "TF1.h"
#include <stdlib.h>

#include "constants.hh"

using namespace TMath ;
using namespace std;

//----------------------------------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------------------------------

TRandom3 *ran = new TRandom3;

TH2D *hNuEFlux ;
TH2D *hNuEbFlux ;
TH2D *hNuXFlux ;
TH2D *h2_en_NOS_Bollig_flux; //for electron neutrinos, no oscillations
TH2D *h2_en_NO_Bollig_flux; //for electron neutrinos, Normal Ordering
TH2D *h2_en_IO_Bollig_flux; //for electron neutrinos, Inverted Ordering


TH2D *h2_xn_NOS_Bollig_flux; //for x neutrinos, no oscillations
TH2D *h2_xn_NO_Bollig_flux; //for x neutrinos, Normal Ordering
TH2D *h2_xn_IO_Bollig_flux; //for x neutrinos, Inverted Ordering


TH2D *h2_en_bar_NOS_Bollig_flux; //for electron antineutrinos, no oscillations
TH2D *h2_en_bar_NO_Bollig_flux; //for electron antineutrinos, Normal Ordering
TH2D *h2_en_bar_IO_Bollig_flux; //for electron antineutrinos, Inverted Ordering

TH2D *h2_xn_bar_NOS_Bollig_flux; //for x antineutrinos, no oscillations
TH2D *h2_xn_bar_NO_Bollig_flux; //for x antineutrinos, Normal Ordering
TH2D *h2_xn_bar_IO_Bollig_flux; //for x antineutrinos, Inverted Ordering


TH2D *hCEvNS ;

TH2D *hCEvNS_NuE  ;
TH2D *hCEvNS_NuEb ;
TH2D *hCEvNS_NuX  ;

TH2D *hNuE_abs;


const double MeV_to_keV = 1e3;
const double keV_to_GeV = 1e-6;
double G_F = 1.1663787e-5 * 1.03; // GeV^-2 (from NIST, G_F/(hbar*c)^3 )
//G_F *= 1.03; // Increase by 3% due to radiative corrections for charged-current semi-leptonic processes (Capozzi et al.)
const double hbarc = 1.97326979e-14; // GeV*cm
const double Vud = 0.9736; // (+/- 0.0006), from PhysRevLett.74.1521
const double Qgs_MeV = 1.505; // Ground state mass difference 40K - 40Ar
const double mec2_keV = 510.9989;



//Here choose your detector, as the electron drift time depends on its height.
//double height = 2.6 ; // height of TPC in DS20K [m]
double height = 3.5 ; // height of TPC in ARGO [m]
double drift = height*1e-3; //  electron drift time [s ]


double GeV_to_MeV = 1e3; 
double eV_to_GeV = 1e-9;
double kpc_to_km = 3.086*1e16;
double dist = 10; // kpc



//----------------------------------------------------------------------------------------------------------
// Helm Form Factor for the CEvNS cross section
//----------------------------------------------------------------------------------------------------------
double FF2(double Er) {
  // Er here is in MeV
  double  pi     = TMath::Pi();
  double  skin   = 0.9*fm;
  double  alpha  = 0.52;
  double q   = TMath::Sqrt(2. * Er * mN*pow(cspeed,-2) );
  double c1  = (1.23 * TMath::Power(A, 1./3.) - 0.6)*fm;
  double rn  = TMath::Sqrt(TMath::Power(c1, 2.) + 7./3. * TMath::Power(TMath::Pi() * alpha*fm, 2.) - 5*pow(skin,2));
  double qrn = q * rn;
  return 3. * (TMath::Sin(qrn/hbar) - qrn/hbar * TMath::Cos(qrn/hbar)) / pow(qrn/hbar, 3.) * TMath::Exp(-0.5 * pow(q * skin/hbar, 2.));
}


//Neutrino aborption cross-section by Shawn
double fermi_function(double T_keV);

const int nLvls = 15;
double ene_lvls[nLvls] = { // keV
  2333 , 2775 , 3204,
  3503 , 3870 , 4384,
  4421 , 4763 , 5162,
  5681 , 6118 , 6790,
  7468 , 7795 , 7952
};
// The energy of the Fermi transition is from Bhattachrya et al. (1998)
double BFs[nLvls] = {
  0, 0, 0,
  0, 0, 4,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0
};
// Following Capozzi et al., these numbers agree with Bhattacharya et al. multiplied by the
// axial coupling constant squared (with gA = -1.26)
double BGTs[nLvls] = {
  1.64, 1.49, 0.06,
  0.16, 0.44, 0.00,
  0.86, 0.48, 0.59,
  0.21, 0.48, 0.71,
  0.06, 0.14, 0.97
};

//----------------------------------------------------------------------------------------------------------
// Build fluxes
//----------------------------------------------------------------------------------------------------------
TH2D* buildFlux(string filename,  string hname) {

  double fluxE;
  double rT = 0 ;
  double rE =0  ;
  double recoil = 0;
  char buffer[400];

  ifstream ff3(filename.c_str());
  for(int i=0;i<5;++i)  ff3.getline (buffer,400);

  n_ene = 200;
  min_e = 0;
  max_e = 200; //MeV
  cout << hname << endl ;

  TH2D *hflux = new TH2D (hname.c_str(), hname.c_str(), n_time, min_t , max_t, n_ene, min_e, max_e);


  int cc = 0 ;
  while(1) {
    double t0, lum, media, media2;
    ff3 >> t0 >> lum >> media >> media2 ;
    if(ff3.eof()) break;

    int tbin = hflux->GetXaxis()->FindBin(t0);
    for(int ebin =0;ebin<n_ene;++ebin) {
      double ene = min_e + ebin*enestep  + enestep/2. ;
      double foe = 6.3e56; // in MeV
      double distanza = 1e4*parsec;
      double alfa = (2*pow(media, 2) - media2)/(media2-pow(media,2));
      double beta = 1 + alfa ;
      double gamma = Gamma(beta);
      double prod = -beta*ene/media ;
      double flux = (1/media)*(pow(ene/media,alfa))*(exp(prod))*((pow(beta,beta)))/gamma ;  //in MeV^-1
      double difflux = (lum*foe*flux)/(4*Pi()*pow(distanza,2)*media)*enestep*timestep; //in Mev^-1 s^-1 m-2
      hflux->SetBinContent(tbin, ebin,  difflux); //output in MeV
      ++cc ;

    }

  }
  ff3.close();
  cout << "Flux file: read... " << endl;
  return hflux ;

}


//----------------------------------------------------------------------------------------------------------
// Retrieve fluxes root files
//----------------------------------------------------------------------------------------------------------

void collect_fluxes(bool Garching = true, bool Bollig = true){
  //TFile *Garching_fluxes11Msun= TFile::Open("/home/michela/multi-messenger/fluxes/Garching1D_11Msun_10kpc.root", "READ");

  //hNuEFlux  = (TH2D*) Garching_fluxes11Msun->Get("hNuEFlux");
  //hNuEbFlux = (TH2D*) Garching_fluxes11Msun->Get("hNuEbFlux");
 // hNuXFlux  = (TH2D*) Garching_fluxes11Msun->Get("hNuXFlux");
    
    hNuEFlux = buildFlux("./fluxes_data/Garching1D/11Msun/neutrino_signal_nu_e-LS220-s11.2c.data","hNuEFlux");
    hNuEbFlux = buildFlux("./fluxes_data/Garching1D/11Msun/neutrino_signal_nubar_e-LS220-s11.2c.data","hNuEbFlux");
    hNuXFlux = buildFlux("./fluxes_data/Garching1D/11Msun/neutrino_signal_nu_x_LS220-s11.2c.data","hNuXFlux");

    TFile *fout = new TFile("./fluxes/Garching11D_11Msun.root","recreate");
    hNuEFlux->Write("hNuEFlux");
    hNuEbFlux->Write("hNuEbFlux");
    hNuXFlux->Write("hNuXFlux");
    fout->Close();
    cout << "Flux library: done" << endl ;
  

  TFile *fflux = TFile::Open("./fluxes/Garching11D_11Msun.root");
  hNuEFlux  = (TH2D*) fflux->Get("hNuEFlux");
  hNuEbFlux = (TH2D*) fflux->Get("hNuEbFlux");
  hNuXFlux  = (TH2D*) fflux->Get("hNuXFlux");

  hNuEFlux->GetXaxis()->SetTitle("Time [s]");
  hNuEFlux->GetYaxis()->SetTitle("Neutrino Energy [MeV]");
  hNuEFlux->SetTitle("Differential Nu_e Flux [MeV^-1 s^-1 m^-2]");


  hNuEbFlux->GetXaxis()->SetTitle("Time [s] ");
  hNuEbFlux->GetYaxis()->SetTitle("Neutrino Energy [MeV]");
  hNuEbFlux->SetTitle("Differential Nu_Eb Flux [MeV^-1 s^-1 m^-2]");


  hNuXFlux->GetXaxis()->SetTitle("Time [s] ");
  hNuXFlux->GetYaxis()->SetTitle("Neutrino Energy [MeV]");
  hNuXFlux->SetTitle("Differential Nu_X Flux [MeV^-1 s^-1 m^-2]");


  TFile *snewpy_fluxes;
  snewpy_fluxes = TFile::Open("./fluxes/Bollig_27Msun_10kpc_fluxes.root", "READ");

\
  //----------Get histograms for SNEWPy fluxes - Lucy Kotsioupolus 

  //Bollig et al model, assuming Adiabatic Oscillations, 10 kpc distance _ check mass of the model

  h2_en_NOS_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_NOS_Bollig_flux");
  h2_en_NO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_NO_Bollig_flux");
  h2_en_IO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_IO_Bollig_flux");

  h2_xn_NOS_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_NOS_Bollig_flux");
  h2_xn_NO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_NO_Bollig_flux");
  h2_xn_IO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_IO_Bollig_flux");


  h2_en_bar_NOS_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_bar_NOS_Bollig_flux");
  h2_en_bar_NO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_bar_NO_Bollig_flux");
  h2_en_bar_IO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_en_bar_IO_Bollig_flux");

  h2_xn_bar_NOS_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_bar_NOS_Bollig_flux");
  h2_xn_bar_NO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_bar_NO_Bollig_flux");
  h2_xn_bar_IO_Bollig_flux = (TH2D*) snewpy_fluxes -> Get("h2_xn_bar_IO_Bollig_flux");


}





//----------------------------------------------------------------------------------------------------------
// Build CEvNS cross section
//----------------------------------------------------------------------------------------------------------
void buildCEvNS(double max_erec, int n_erec, TH2D *hNuEFlux) {
  double erecstep = max_erec / n_erec ; //MeV
  int n_ene = hNuEFlux->GetYaxis()->GetNbins();
  double enestep = (max_e-min_e)/n_ene; //MeV
  double max_cs = 0;

  double int_cs = 0;
  hCEvNS = new TH2D ("hCEvNS", " Differential cross section [m^2/MeV]", n_erec, 0, max_erec, n_ene, min_e, max_e);

  int counter = 0 ;
  double Qw = A - Z - ( 1- 4*0.2386)*Z;
  for (int i = 0; i < n_ene ; ++ i ){
    double Enu = (i*enestep + enestep/3.) ;
    for(int l = 0 ; l < n_erec ; ++l ){

      if(!(counter%100000)) cout << float(counter)/n_ene/n_erec*100 << "%  " <<endl ;
      ++counter;
      double erec = (l*erecstep + erecstep/3.);
      double Emin = sqrt(mN*erec/2);
      double par =  1. - mN*erec/(2*pow(Enu,2));

      if (par <=0) continue  ;

      if( Enu < Emin) continue;


      double form = FF2(erec) ;
      double cs = pow(GF,2)*mN/(4*TMath::Pi()) *pow(Qw,2) *par* pow(form,2)*pow(htc,2)*enestep*erecstep; // m^2/MeV


      if( cs > max_cs) max_cs = cs ;
      hCEvNS->SetBinContent(l+1, i+1 , cs );
    }
  }

  TFile *fout = new TFile("./cross_sections/CEvNS.root","recreate");
  hCEvNS->Write("hCEvNS");

  cout << "Cross section library written in cross_sections/CEvNS.root, also normalized to the maximum value" << endl ;
  fout->Close();


}



//----------------------------------------------------------------------------------------------------------
// Build Electron neutrino absorption differential cross-section
//----------------------------------------------------------------------------------------------------------
void build_NueAbsorption(TH2D *hNuEFlux){
  // Calculate the electron neutrino absorption cross section for Fermi+Gamow-Teller transitions
  // From M. Bhattacharya et al. Phys Rev. C 80, 055501 (2009)
  // Enu is the energy of the incident neutrino.
  // Following the implementation in Capozzi et al. arXiv:1808:08232 (2019)


  //double Enu_MeV = (Er_keV/MeV_to_keV) + Qgs_MeV;
  int n_el = (hNuEFlux->GetYaxis()->GetNbins());
  double min_el = 0;
  double max_el = hNuEFlux->GetYaxis()->GetBinUpEdge(hNuEFlux->GetYaxis()->GetNbins()) ; //MeV
  double ene_elstep = (max_el - min_el)/n_el; //MeV
  //double ene_erstep = 1000; //keV
  double ene_erstep = 1; //MeV
  double min_er = 0 ;
  
  //double max_er = (max_el - Qgs_MeV)*MeV_to_keV;
  double max_er = (max_el - Qgs_MeV);

  int n_er = int((max_er - min_er)/ene_erstep);
  cout << max_er << " " << n_er << " " << ene_erstep << endl;
  cout << max_el << " " << n_el << " " << ene_elstep << endl;
  hNuE_abs = new TH2D("hNuE_abs", "Neutrino absorption Cross-Section", n_er, min_er, max_er, n_el, min_el, max_el);
  for (int el = 0; el < n_el; ++el){
    double Enu_MeV =(el*ene_elstep + ene_elstep/3.); // ene_elstep*el ;
    //double Er_keV = (Enu_MeV - Qgs_MeV)*MeV_to_keV; //in keV
    double Er_keV = (Enu_MeV - Qgs_MeV); //in MeV
    
    int er = int((Er_keV - ene_erstep/3. )/ene_erstep);
      if(Enu_MeV*MeV_to_keV < ene_lvls[0]){
        double sigma = 0;
      }
      double prefactor = pow(G_F * hbarc * Vud, 2)/TMath::Pi(); //  GeV^-2 * cm^2
      double sumpart = 0;
      for(int i = 0; i < nLvls; i++){
        double MatrixElement2 = BFs[i] + BGTs[i];
        double Qi_keV = Qgs_MeV*MeV_to_keV + ene_lvls[i];
        double We_keV = Enu_MeV*MeV_to_keV - Qi_keV + mec2_keV;
        if(We_keV < mec2_keV) continue;
        double pe_keV = sqrt(pow(We_keV,2) - pow(mec2_keV,2));
        double F = fermi_function(Enu_MeV*MeV_to_keV - Qi_keV);
        sumpart += pe_keV*We_keV*F*MatrixElement2; // keV^2
      }
      sumpart *= pow(keV_to_GeV,2); // GeV^2
      // 40K energy levels, B(F) + B(GT)
      //for(int er = 0; er < n_er ; ++er){
      double sigma = prefactor * sumpart *ene_elstep; // cm^2
      hNuE_abs->SetBinContent(er +1, el+1, sigma*1e-4); //now in m^2
    //} // loop recoil ene
   } //loop incident ene
  TFile *fout = new TFile("./cross_sections/NuE_absorb.root","recreate");
  hNuE_abs->GetYaxis()->SetTitle("Neutrinos Energy [MeV] ");
  hNuE_abs->GetXaxis()->SetTitle("Recoil Energy [MeV]");
  hNuE_abs->SetTitle("Cross section [m^{2}]");

  hNuE_abs->Write("hNuE_abs");
  fout->Close();
  cout << "Electron Neutrino absorption Cross section library written in cross_sections/NuE_absorb.root" << endl ;


}


double fermi_function(double T_keV){
  // Analytical approximation of the Fermi Function from
  // G. K. Schenter & P. Vogel, "A Simple Approximation of the Fermi Function in Nuclear Beta Decay"
  // https://www-tandfonline-com/doi/abs/10.13182/NSE83-A17574
  // They estimate that except for where  p and Z are small (e.g. non-relativistic case), the
  // maximum deviation from the full Femri Function is <3%

  const double m_electron = 510.998923; // keV/c^2
  const double c = 1;
  double Z = 18; // for Ar
  double alpha, beta;
  if(T_keV < 1.2*m_electron*c*c){
    alpha = -0.811 + 4.46e-2*Z + 1.08e-4*Z*Z;
    beta  =  0.673 - 1.82e-2*Z + 6.38e-5*Z*Z;
  }
  else{
    alpha = -8.46e-2 + 2.48e-2*Z + 2.37e-4*Z*Z;
    beta  =  1.15e-2 + 3.58e-4*Z - 6.17e-5*Z*Z;
  }

  double E_keV = T_keV + m_electron*c*c;
  double p_keV = sqrt(pow(E_keV/c,2) - pow(m_electron*c,2));

  double F = (E_keV/p_keV)*TMath::Exp(alpha + beta*sqrt(E_keV/(m_electron*c*c) - 1));
  return F;
}

TF1 *get_fermi_tf1(){
  return new TF1("fermi_func",[&](double *x, double *p){ return fermi_function(x[0]); }, 0, 10e3, 0);
}



//create differential rate for the chosen cross-section 

//----------------------------------------------------------------------------------------------------------
// Build differential rates in 1 ton unit
//----------------------------------------------------------------------------------------------------------
TH2D* buildInteractions(TH2D *hflux, TH2D *hcs, string hname, bool oscillation=false) {
  TH2D *hint = new TH2D("hint","",
    hflux->GetXaxis()->GetNbins(), // time
    hflux->GetXaxis()->GetBinLowEdge(1),
    hflux->GetXaxis()->GetBinUpEdge(hflux->GetXaxis()->GetNbins()),
    hcs->GetXaxis()->GetNbins(), // erec
    hcs->GetXaxis()->GetBinLowEdge(1),
    hcs->GetXaxis()->GetBinUpEdge(hcs->GetXaxis()->GetNbins())
  );
  cout << hname << endl ;
  int counter = 0 ;
  int max = hflux->GetYaxis()->GetNbins()
           *hflux->GetXaxis()->GetNbins()
     *hcs->GetXaxis()->GetNbins();
  for(int i=1;i<hflux->GetYaxis()->GetNbins();++i)  { // Enu: i
    double Enu = hflux->GetYaxis()->GetBinCenter(i) ;
    for(int j=1;j<hflux->GetXaxis()->GetNbins();++j) {  // t0: j
      double t0 = hflux->GetXaxis()->GetBinCenter(j) ;
      for(int k=1;k<hcs->GetXaxis()->GetNbins();++k) { // Erec: k in MeV
        double Erec = hcs->GetXaxis()->GetBinCenter(k) ;
        double flux = hflux->GetBinContent(j,i); //this should be in MeV^-1 s^1 m^-2. I want it in keV^-1 s^1 m^-2
        double cs   = hcs->GetBinContent(k,i); //this goes in m^2, as the flux is in m^-2
         //cout << i << "  " << j <<"  " << k << "  " << t0 << "  " <<  Erec << "  " << cs << "  " <<  flux << endl ;
        hint->Fill(t0, Erec, ton*cs*flux); 
  ++counter ;
      }
    }
    cout << i << " out of " << hflux->GetYaxis()->GetNbins() << endl;
  }
  hint->SetName(hname.c_str());
  return hint ;

}


//make all the plots

void plot_all(bool compare_flux = true, bool compare_cross_section = true){
	
  TH1D *hNuEFlux_pEnu = (TH1D*) hNuEFlux->ProjectionY();
  TH1D *hNuEbFlux_pEnu = (TH1D*) hNuEbFlux->ProjectionY();
  TH1D *hNuXFlux_pEnu = (TH1D*) hNuXFlux->ProjectionY();


  hNuEFlux_pEnu->SetLineColor(2);
  hNuEbFlux_pEnu->SetLineColor(4);
  hNuXFlux_pEnu->SetLineColor(8);


  TH1D *hNuEFlux_pt = (TH1D*) hNuEFlux->ProjectionX();
  TH1D *hNuEbFlux_pt = (TH1D*) hNuEbFlux->ProjectionX();
  TH1D *hNuXFlux_pt = (TH1D*) hNuXFlux->ProjectionX();

  hNuEFlux_pt->SetLineColor(2);
  hNuEbFlux_pt->SetLineColor(4);
  hNuXFlux_pt->SetLineColor(8);

  TH1D *h2_en_NOS_Bollig_flux_pEnu = (TH1D*) h2_en_NOS_Bollig_flux->ProjectionY();
  TH1D *h2_en_NO_Bollig_flux_pEnu = (TH1D*) h2_en_NO_Bollig_flux->ProjectionY();
  TH1D *h2_en_IO_Bollig_flux_pEnu = (TH1D*) h2_en_IO_Bollig_flux->ProjectionY();

  h2_en_NOS_Bollig_flux_pEnu->SetLineColor(9);
  h2_en_NO_Bollig_flux_pEnu->SetLineColor(46);
  h2_en_IO_Bollig_flux_pEnu->SetLineColor(41);

  TH1D *h2_en_NOS_Bollig_flux_pt = (TH1D*) h2_en_NOS_Bollig_flux->ProjectionX();
  TH1D *h2_en_NO_Bollig_flux_pt = (TH1D*) h2_en_NO_Bollig_flux->ProjectionX();
  TH1D *h2_en_IO_Bollig_flux_pt = (TH1D*) h2_en_IO_Bollig_flux->ProjectionX();


  h2_en_NOS_Bollig_flux_pt->SetLineColor(9);
  h2_en_NO_Bollig_flux_pt->SetLineColor(46);
  h2_en_IO_Bollig_flux_pt->SetLineColor(41);

 
  cout << "Garching    " <<  hNuEFlux->Integral()  << " Bollig_no_osc    "  << h2_en_NOS_Bollig_flux->Integral()  << " Bollig_NMO_osc    "  << h2_en_NO_Bollig_flux->Integral()  << " Bollig_IMO_osc    "   << h2_en_IO_Bollig_flux->Integral()   << endl;

  TCanvas *c1 = new TCanvas("c1", "comparison_fluxes_time", 700, 700);


  h2_en_NOS_Bollig_flux_pt->Draw("histo");
  h2_en_NO_Bollig_flux_pt->Draw("histo, same");
  h2_en_IO_Bollig_flux_pt->Draw("histo, same");
  hNuEFlux_pt->Draw("histo, same");

  TLegend *leg1 = new TLegend(0.5 , 0.6 , 0.85, 0.85);
  leg1->SetBorderSize(0);
  leg1->SetFillColor(0);
  leg1->AddEntry(hNuEFlux_pt,  "Garching1D_11Msun" ,      "l");
  leg1->AddEntry(h2_en_NOS_Bollig_flux_pt, "Bollig_no_osc" ,  "l");
  leg1->AddEntry(h2_en_NO_Bollig_flux_pt,  "Bollig_NMO" ,      "l");
  leg1->AddEntry(h2_en_IO_Bollig_flux_pt,  "Bollig_IMO" ,      "l");
  leg1->Draw();


  TCanvas *c2 = new TCanvas("c2", "comparison fluxes neutrino energy", 700, 700);

  
  h2_en_NOS_Bollig_flux_pEnu->Draw("histo");
  hNuEFlux_pEnu->Draw("histo, same");
  h2_en_NO_Bollig_flux_pEnu->Draw("histo, same");
  h2_en_IO_Bollig_flux_pEnu->Draw("histo, same");

  TLegend *leg2 = new TLegend(0.5 , 0.6 , 0.85, 0.85);
  leg2->SetBorderSize(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(hNuEFlux_pEnu,  "Garching1D_11Msun" ,      "l");
  leg2->AddEntry(h2_en_NOS_Bollig_flux_pEnu, "Bollig_no_osc" ,  "l");
  leg2->AddEntry(h2_en_NO_Bollig_flux_pEnu,  "Bollig_NMO" ,      "l");
  leg2->AddEntry(h2_en_IO_Bollig_flux_pEnu,  "Bollig_IMO" ,      "l");
  leg2->Draw();





  TCanvas *c3 = new TCanvas("c3", "comparison cross-sections neutrino energy", 700, 700);

  TFile *fCEvNS = TFile::Open("./cross_sections/CEvNS.root");
  hCEvNS = (TH2D*) fCEvNS->Get("hCEvNS");
  TH1D *hCEvNS_pEnu = (TH1D*) hCEvNS->ProjectionY();

  TFile *fNuAbs = TFile::Open("./cross_sections/NuE_absorb.root");
  hNuE_abs = (TH2D*) fNuAbs->Get("hNuE_abs");
  TH1D *hNuE_abs_pEnu = (TH1D*) hNuE_abs->ProjectionY();
  
  hCEvNS_pEnu->SetLineColor(30);
  hNuE_abs_pEnu->SetLineColor(38);


  hCEvNS_pEnu->Draw("histo");
  hNuE_abs_pEnu->Draw("histo, same");
  

  TLegend *leg3 = new TLegend(0.5 , 0.6 , 0.85, 0.85);
  leg3->SetBorderSize(0);
  leg3->SetFillColor(0);
  leg3->AddEntry(hCEvNS_pEnu,  "CEvNS in argon" ,      "l");
  leg3->AddEntry(hNuE_abs_pEnu, "ELectron Neutrino absorption" ,  "l");
  leg3->Draw();







}

void supernova_argon(bool make_libraries=true, bool make_plot = true){


	collect_fluxes(true,true);
  if(make_libraries) {
    //hCEvNS_G = buildCEvNS(max_erec,n_erec, hNuEFlux);
    buildCEvNS(max_erec,n_erec, h2_en_NOS_Bollig_flux);
    //build_NueAbsorption(hNuEFlux);
    build_NueAbsorption(h2_en_NOS_Bollig_flux);
  }

  cout <<hNuE_abs->Integral() << "-------" << endl;

  TH2D *hCEvNS_G = (TH2D*) hCEvNS->Clone("hCEvNS_G");
  TH2D *hCEvNS_B= (TH2D*) hCEvNS->Clone("hCEvNS_B");
  TH2D *habs_G = (TH2D*) hNuE_abs->Clone("habs_G");
  TH2D *habs_B = (TH2D*) hNuE_abs->Clone("habs_B");
 

  //differential rate of CEvNS from the SN 11 Msun Garching model 
  TH2D* hCEvNS_Garch_NuE  = (TH2D*) buildInteractions(hNuEFlux, hCEvNS_G, "hCevNS_Garch_NuE", false);
  TH2D* hCEvNS_Garch_NuEb = (TH2D*) buildInteractions(hNuEbFlux, hCEvNS_G, "hCevNS_Garch_NuEb", false);
  TH2D* hCEvNS_Garch_NuX  = (TH2D*) buildInteractions(hNuXFlux, hCEvNS_G, "hCevNS_Garch_NuX", false);


  //differential rate of CEvNS from the Bollig model 
  TH2D* hCEvNS_Boll_NuE_NOS  = (TH2D*) buildInteractions(h2_en_NOS_Bollig_flux, hCEvNS_B, "hCevNS_BollNOS_NuE", false);
  TH2D* hCEvNS_Boll_NuE_NO = (TH2D*) buildInteractions(h2_en_NO_Bollig_flux, hCEvNS_B, "hCevNS_BollNO_NuE", false);
  TH2D* hCEvNS_Boll_NuE_IO = (TH2D*) buildInteractions(h2_en_IO_Bollig_flux, hCEvNS_B, "hCevNS_BollIO_NuE", false);

  
  TH2D* hCEvNS_Boll_NuX_NOS  = (TH2D*) buildInteractions(h2_xn_NOS_Bollig_flux, hCEvNS_B, "hCevNS_BollNOS_NuX", false);
  TH2D* hCEvNS_Boll_NuX_NO = (TH2D*) buildInteractions(h2_xn_NO_Bollig_flux, hCEvNS_B, "hCevNS_BollNO_NuX", false);
  TH2D* hCEvNS_Boll_NuX_IO = (TH2D*) buildInteractions(h2_xn_IO_Bollig_flux, hCEvNS_B, "hCevNS_BollIO_NuX", false);


  TH2D* hCEvNS_Boll_NuEb_NOS  = (TH2D*) buildInteractions(h2_en_bar_NOS_Bollig_flux, hCEvNS_B, "hCevNS_BollNOS_NuEb", false);
  TH2D* hCEvNS_Boll_NuEb_NO = (TH2D*) buildInteractions(h2_en_bar_NO_Bollig_flux, hCEvNS_B, "hCevNS_BollNO_NuEb", false);
  TH2D* hCEvNS_Boll_NuEb_IO = (TH2D*) buildInteractions(h2_en_bar_IO_Bollig_flux, hCEvNS_B, "hCevNS_BollIO_NuEb", false);


  TH2D* hCEvNS_Boll_NuXb_NOS  = (TH2D*) buildInteractions(h2_xn_bar_NOS_Bollig_flux, hCEvNS_B, "hCevNS_BollNOS_NuXb", false);
  TH2D* hCEvNS_Boll_NuXb_NO = (TH2D*) buildInteractions(h2_xn_bar_NO_Bollig_flux, hCEvNS_B, "hCevNS_BollNO_NuXb", false);
  TH2D* hCEvNS_Boll_NuXb_IO = (TH2D*) buildInteractions(h2_xn_bar_IO_Bollig_flux, hCEvNS_B, "hCevNS_BollIO_NuXb", false);
  
  
  

  
  cout << "differential rate of Nue absorption from the Bollig model " << endl;
  TH2D* habs_Garch_NuE  = (TH2D*) buildInteractions(hNuEFlux, habs_G, "habs_Garch_NuE", false);

  //differential rate of Nue absorption from the Bollig model 
  TH2D* habs_Boll_NuE_NOS  = (TH2D*) buildInteractions(h2_en_NOS_Bollig_flux,  habs_B, "habs_BollNOS_NuE", false);
  TH2D* habs_Boll_NuE_NO = (TH2D*) buildInteractions(h2_en_NO_Bollig_flux,  habs_B, "habs_BollNO_NuE", false);
  TH2D* habs_Boll_NuE_IO = (TH2D*) buildInteractions(h2_en_IO_Bollig_flux,  habs_B, "habs_BollIO_NuE", false);
  
   

  char interactions_file[500];
  std::sprintf(interactions_file, "./diffrate/differential_rate_%ikpc.root", int(dist));
  cout  << interactions_file << endl;
  TFile *fout = new TFile(interactions_file,"RECREATE");

  
  // Write differential rate of CevNS for Garching 11 Msun

  hCEvNS_Garch_NuE->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Garch_NuE->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Garch_NuE->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Garch_NuE->Write("hCEvNS_Garch_NuE");

   
  hCEvNS_Garch_NuEb->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Garch_NuEb->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Garch_NuEb->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Garch_NuEb->Write("hCEvNS_Garch_NuEb");

  hCEvNS_Garch_NuX->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Garch_NuX->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Garch_NuX->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Garch_NuX->Write("hCEvNS_Garch_NuX");

  

  //Need to add oscillations to Garching before writing this
 //Write differential rate of Nue absorption from the Garching 11 Msun
  
  habs_Garch_NuE->GetXaxis()->SetTitle("Time [s] ");
  habs_Garch_NuE->GetYaxis()->SetTitle("Final states kinetic Energy [MeV]");
  habs_Garch_NuE->SetTitle("Nu_E events [counts/ MeV /s]");
  habs_Garch_NuE->Write("habs_Garch_NuE"); 
  
  

  

  //Write differential rate of CEvNS from the Bollig model 
  hCEvNS_Boll_NuE_NOS->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuE_NOS->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuE_NOS->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuE_NOS->Write("hCEvNS_Boll_NuE_NOS");


  hCEvNS_Boll_NuE_NO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuE_NO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuE_NO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuE_NO->Write("hCEvNS_Boll_NuE_NO");

  hCEvNS_Boll_NuE_IO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuE_IO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuE_IO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuE_IO->Write("hCEvNS_Boll_NuE_IO");


  hCEvNS_Boll_NuX_NOS->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuX_NOS->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuX_NOS->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuX_NOS->Write("hCEvNS_Boll_NuX_NOS");


  hCEvNS_Boll_NuX_NO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuX_NO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuX_NO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuX_NO->Write("hCEvNS_Boll_NuX_NO");

  hCEvNS_Boll_NuX_IO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuX_IO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuX_IO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuX_IO->Write("hCEvNS_Boll_NuX_IO");

  hCEvNS_Boll_NuEb_NOS->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuEb_NOS->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuEb_NOS->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuEb_NOS->Write("hCEvNS_Boll_NuEb_NOS");


  hCEvNS_Boll_NuEb_NO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuEb_NO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuEb_NO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuEb_NO->Write("hCEvNS_Boll_NuEb_NO");

  hCEvNS_Boll_NuEb_IO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuEb_IO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuEb_IO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuEb_IO->Write("hCEvNS_Boll_NuEb_IO");


  hCEvNS_Boll_NuXb_NOS->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuXb_NOS->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuXb_NOS->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuXb_NOS->Write("hCEvNS_Boll_NuXb_NOS");


  hCEvNS_Boll_NuXb_NO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuXb_NO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuXb_NO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuXb_NO->Write("hCEvNS_Boll_NuXb_NO");

  hCEvNS_Boll_NuXb_IO->GetXaxis()->SetTitle("Time [s] ");
  hCEvNS_Boll_NuXb_IO->GetYaxis()->SetTitle("Recoil Energy [MeV]");
  hCEvNS_Boll_NuXb_IO->SetTitle("Nu_E events [counts/ MeV /s]");
  hCEvNS_Boll_NuXb_IO->Write("hCEvNS_Boll_NuXb_IO");


  //Write differential rate of Nue absorption from the Bollig model 
  habs_Boll_NuE_NOS->GetXaxis()->SetTitle("Time [s] ");
  habs_Boll_NuE_NOS->GetYaxis()->SetTitle("Final states kinetic energy [MeV]");
  habs_Boll_NuE_NOS->SetTitle("Nu_E events [counts/ MeV /s]");
  habs_Boll_NuE_NOS->Write("habs_Boll_NuE_NOS");


  habs_Boll_NuE_NO->GetXaxis()->SetTitle("Time [s] ");
  habs_Boll_NuE_NO->GetYaxis()->SetTitle("Final states kinetic energy [MeV]");
  habs_Boll_NuE_NO->SetTitle("Nu_E events [counts/ MeV /s]");
  habs_Boll_NuE_NO->Write("habs_Boll_NuE_NO");


  habs_Boll_NuE_IO->GetXaxis()->SetTitle("Time [s] ");
  habs_Boll_NuE_IO->GetYaxis()->SetTitle("Final states kinetic energy [MeV]");
  habs_Boll_NuE_IO->SetTitle("Nu_E events [counts/ MeV /s]");
  habs_Boll_NuE_IO->Write("habs_Boll_NuE_IO");
  
  

  //plot_all(true, true);

 fout->Close();



}


