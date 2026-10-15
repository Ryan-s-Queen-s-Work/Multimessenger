#include <TFile.h>
#include <TTree.h>
#include <TArrayS.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TH2D.h>

TGraph *CEvNS_nu_e = new TGraph();
TGraph *CEvNS_nu_e_bar = new TGraph();
TGraph *CEvNS_nu_x = new TGraph();
TGraph *CEvNS_nu_x_bar = new TGraph();
TGraph *Abs_nu_e = new TGraph();

CEvNS_nu_e->SetPoint(0, 0.08, 217478);
CEvNS_nu_e->SetPoint(1, 0.168, 49314);
CEvNS_nu_e->SetPoint(2, 0.17, 48161);
CEvNS_nu_e->SetPoint(3, 0.26, 20589);
CEvNS_nu_e->SetPoint(3, 0.38, 9638);