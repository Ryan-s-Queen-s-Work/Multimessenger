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


using namespace TMath ;
using namespace std;

TRandom3 *ran = new TRandom3;


double MassTPC = 50; //tonnes LAr TPC
double Mass = 50 + 72 +650; //tonnes LAr


void create_SNcsv(){
	double dist = 10; //kpc
	double ratio = pow(10./dist,2);
  	char interactions_file[500];
  	std::sprintf(interactions_file, "./diffrate/differential_rate_%ikpc.root", int(dist));
  	//cout  << interactions_file << endl;
  	TFile *diffrate = TFile::Open(interactions_file);

  	TH2D* hCEvNS_Boll_NuE_NOS  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuE_NOS");
  	TH2D* hCEvNS_Boll_NuEb_NOS  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuEb_NOS");
  	TH2D* hCEvNS_Boll_NuX_NOS  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuX_NOS");
  	TH2D* hCEvNS_Boll_NuXb_NOS  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuXb_NOS");

  	TH2D* hCEvNS_Boll_NuE_NO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuE_NO");
  	TH2D* hCEvNS_Boll_NuEb_NO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuEb_NO");
  	TH2D* hCEvNS_Boll_NuX_NO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuX_NO");
  	TH2D* hCEvNS_Boll_NuXb_NO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuXb_NO");

  	TH2D* hCEvNS_Boll_NuE_IO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuE_IO");
  	TH2D* hCEvNS_Boll_NuEb_IO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuEb_IO");
  	TH2D* hCEvNS_Boll_NuX_IO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuX_IO");
  	TH2D* hCEvNS_Boll_NuXb_IO  = (TH2D*) diffrate->Get("hCEvNS_Boll_NuXb_IO");

/////////No oscillation

    std::ofstream csvFile("CEvNS_Boll_NOS.csv");

  	double NuE_NOS = ran->Poisson(MassTPC*hCEvNS_Boll_NuE_NOS->Integral()*ratio);
  	double NuEb_NOS = ran->Poisson(MassTPC*hCEvNS_Boll_NuEb_NOS->Integral()*ratio);
  	double NuX_NOS = ran->Poisson(MassTPC*hCEvNS_Boll_NuX_NOS->Integral()*ratio);
  	double NuXb_NOS = ran->Poisson(MassTPC*hCEvNS_Boll_NuXb_NOS->Integral()*ratio);


  	for(int i=0;i<NuE_NOS;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuE_NOS->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFile << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuEb_NOS;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuEb_NOS->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFile << _t << " " << _Er << std::endl;
  	}
 
  	for(int i=0;i<NuX_NOS;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuX_NOS->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFile << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuXb_NOS;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuXb_NOS->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFile << _t << " " << _Er << std::endl;
  	}

//////////////Normal Mass Ordering

  	std::ofstream csvFileNO("CEvNS_Boll_NO.csv");

  	double NuE_NO = ran->Poisson(MassTPC*hCEvNS_Boll_NuE_NO->Integral()*ratio);
  	double NuEb_NO = ran->Poisson(MassTPC*hCEvNS_Boll_NuEb_NO->Integral()*ratio);
  	double NuX_NO = ran->Poisson(MassTPC*hCEvNS_Boll_NuX_NO->Integral()*ratio);
  	double NuXb_NO = ran->Poisson(MassTPC*hCEvNS_Boll_NuXb_NO->Integral()*ratio);


  	for(int i=0;i<NuE_NO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuE_NO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileNO << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuEb_NO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuEb_NO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileNO << _t << " " << _Er << std::endl;
  	}
 
  	for(int i=0;i<NuX_NO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuX_NO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileNO << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuXb_NO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuXb_NO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileNO << _t << " " << _Er << std::endl;
  	}

////////Inverted Ordering  ////////////////////////


  	std::ofstream csvFileIO("CEvNS_Boll_IO.csv");

  	double NuE_IO = ran->Poisson(MassTPC*hCEvNS_Boll_NuE_IO->Integral()*ratio);
  	double NuEb_IO = ran->Poisson(MassTPC*hCEvNS_Boll_NuEb_IO->Integral()*ratio);
  	double NuX_IO = ran->Poisson(MassTPC*hCEvNS_Boll_NuX_IO->Integral()*ratio);
  	double NuXb_IO = ran->Poisson(MassTPC*hCEvNS_Boll_NuXb_IO->Integral()*ratio);


  	for(int i=0;i<NuE_IO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuE_IO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileIO << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuEb_IO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuEb_IO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileIO << _t << " " << _Er << std::endl;
  	}
 
  	for(int i=0;i<NuX_IO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuX_IO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileIO << _t << " " << _Er << std::endl;
  	}

  	for(int i=0;i<NuXb_IO;++i) {
    	double _t, _Er;
    	hCEvNS_Boll_NuXb_IO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
        //cout <<  _t <<" " << _Er << endl;
        csvFileIO << _t << " " << _Er << std::endl;
  	}


//////////////////////////////////////////////////////////////////
  	//////////////////////////////////////////////////////////////
  	/////Now, toyMC for the Electron neutrino absorption in argon 

  	TH2D* habs_Boll_NuE_NOS  = (TH2D*) diffrate->Get("habs_Boll_NuE_NOS");
  	TH2D* habs_Boll_NuE_NO  = (TH2D*) diffrate->Get("habs_Boll_NuE_NO");
  	TH2D* habs_Boll_NuE_IO  = (TH2D*) diffrate->Get("habs_Boll_NuE_IO");

 	std::ofstream csvFileabsNOS("NuEAbs_Boll_NOS.csv");
 	std::ofstream csvFileabsNO("NuEAbs_Boll_NO.csv");
 	std::ofstream csvFileabsIO("NuEAbs_Boll_IO.csv");


  	double NuE_abs_NOS = ran->Poisson(Mass*habs_Boll_NuE_NOS->Integral()*ratio);
  	double NuE_abs_NO = ran->Poisson(Mass*habs_Boll_NuE_NO->Integral()*ratio);
  	double NuE_abs_IO = ran->Poisson(Mass*habs_Boll_NuE_IO->Integral()*ratio);


  	for(int i=0;i<NuE_abs_NOS;++i) {
    	double _t, _Er;
    	habs_Boll_NuE_NOS->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns

   		double _p = ran->Uniform(0,1.0);
   		if(_p < 0.65){  //65 % probability of having a delayed gamma at 1.644 MeV 
   			double _tdel = _t + (ran->Poisson(480))*1e9; // lifetime of 480 ns
   			double _Edel = 1.644*1e3 ;// energy of the delayed gamma
   			double _tpr = _t + (ran->Poisson(5))*1e9;
   			double _Epr = _Er - _Edel; //assuming all the prompt e- and gammas are emitted at once
   			csvFileabsNOS << _tpr << " " << _Epr << std::endl;
   			csvFileabsNOS << _tdel << " " << _Edel << std::endl;
   		} 
   		else {		
   			csvFileabsNOS << _t << " " << _Er << std::endl;

   		}
        cout <<  _t <<" " << _Er << endl;      
  	} //NOS done


  	for(int i=0;i<NuE_abs_NO;++i) {
    	double _t, _Er;
    	habs_Boll_NuE_NO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
   		double _p = ran->Uniform(0,1.0);
   		if(_p < 0.65){  //65 % probability of having a delayed gamma at 1.644 MeV 
   			double _tdel = _t + (ran->Poisson(480))*1e9; // lifetime of 480 ns
   			double _Edel = 1.644*1e3 ;// energy of the delayed gamma
   			double _tpr = _t + (ran->Poisson(5))*1e9;
   			double _Epr = _Er - _Edel; //assuming all the prompt e- and gammas are emitted at once
   			csvFileabsNO << _tpr << " " << _Epr << std::endl;
   			csvFileabsNO << _tdel << " " << _Edel << std::endl;
   		} 
   		else {		
   			csvFileabsNO << _t << " " << _Er << std::endl;

   		}
        //cout <<  _t <<" " << _Er << endl;      
  	} //NO done


  	for(int i=0;i<NuE_abs_IO;++i) {
    	double _t, _Er;
    	habs_Boll_NuE_IO->GetRandom2(_t,_Er);
   		_Er *= 1e3 ; //MeV to keV
   		_t *=1e9 ; //s to ns
   		double _p = ran->Uniform(0,1.0);
   		if(_p < 0.65){  //65 % probability of having a delayed gamma at 1.644 MeV 
   			double _tdel = _t + (ran->Poisson(480))*1e9; // lifetime of 480 ns
   			double _Edel = 1.644*1e3 ;// energy of the delayed gamma
   			double _tpr = _t + (ran->Poisson(5))*1e9;
   			double _Epr = _Er - _Edel; //assuming all the prompt e- and gammas are emitted at once
   			csvFileabsIO << _tpr << " " << _Epr << std::endl;
   			csvFileabsIO << _tdel << " " << _Edel << std::endl;
   		} 
   		else {		
   			csvFileabsIO << _t << " " << _Er << std::endl;

   		}
        //cout <<  _t <<" " << _Er << endl;      
  	} //IO done



}