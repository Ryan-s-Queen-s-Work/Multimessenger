#include <TFile.h>
#include <TTree.h>
#include <TArrayS.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TH2D.h>
#include "TLatex.h"

void CCSN_distance(bool CCSN=true){
    //Belatrix Data
    double belatrix_d = 0.08; //kpc
    //CEvNS measureable for DEAP-3600
    double belatrix_cevns_nue = 15658;
    double belatrix_cevns_nue_bar = 15576;
    double belatrix_cevns_nux = 12306;
    double belatrix_cevns_nux_bar = 15761;
    double belatrix_abs_nue = 5282;
    //Betelgeuse Data
    double betel_d = 0.168;
    double betel_cevns_nue = 3550;
    double betel_cevns_nue_bar = 3532;
    double betel_cevns_nux = 2791;
    double betel_cevns_nux_bar = 3574;
    double betel_abs_nue = 1198;
    //Antares data
    double antares_d = 0.170;
    double antares_cevns_nue = 3468;
    double antares_cevns_nue_bar = 3449;
    double antares_cevns_nux = 2725;
    double antares_cevns_nux_bar = 3490;
    double antares_abs_nue = 1170;
    //Rigel data
    double rigel_d = 0.26;
    double rigel_cevns_nue = 1482;
    double rigel_cevns_nue_bar = 1475;
    double rigel_cevns_nux = 1165;
    double rigel_cevns_nux_bar = 1492;
    double rigel_abs_nue = 500;
    //Alnilam data
    double aln_d = 0.38;
    double aln_cevns_nue = 694;
    double aln_cevns_nue_bar = 690;
    double aln_cevns_nux = 545;
    double aln_cevns_nux_bar = 698;
    double aln_abs_nue = 234;
    //10kpc 27M Data
    double ten_d = 10;
    double ten_cevns_nue = 1;
    double ten_cevns_nue_bar = 1;
    double ten_cevns_nux = 0.78;
    double ten_cevns_nux_bar = 1;
    double ten_abs_nue = 0.33;

    //add up interactions data
    double bela_total = belatrix_cevns_nue + belatrix_cevns_nue_bar + belatrix_cevns_nux + belatrix_cevns_nux_bar + belatrix_abs_nue;
    double betel_total = betel_cevns_nue + betel_cevns_nue_bar + betel_cevns_nux + betel_cevns_nux_bar + betel_abs_nue;
    double ant_total = antares_cevns_nue + antares_cevns_nue_bar + antares_cevns_nux + antares_cevns_nux_bar + antares_abs_nue;
    double rigel_total = rigel_cevns_nue + rigel_cevns_nue_bar + rigel_cevns_nux + rigel_cevns_nux_bar + rigel_abs_nue;
    double aln_total = aln_cevns_nue + aln_cevns_nue_bar + aln_cevns_nux + aln_cevns_nux_bar + aln_abs_nue;
    double ten_total = ten_cevns_nue + ten_cevns_nue_bar + ten_cevns_nux + ten_cevns_nux_bar + ten_abs_nue;
    
    
    TCanvas *c1 = new TCanvas("c1", "interactiopns v distance", 800, 600);
    TH1D *nu_dist = new TH1D("DEAP CCSN Neutrino Events", "Number of detectable neutrino interactions for CCSN Candidates", 50, 0.01, 0.5);
    nu_dist->SetBinContent(nu_dist->FindBin(belatrix_d), bela_total);
    nu_dist->SetBinContent(nu_dist->FindBin(betel_d), betel_total);
    nu_dist->SetBinContent(nu_dist->FindBin(antares_d), ant_total);
    nu_dist->SetBinContent(nu_dist->FindBin(rigel_d), rigel_total);
    nu_dist->SetBinContent(nu_dist->FindBin(aln_d), aln_total);

    const int n = 6; 
    double x[n] = {belatrix_d, betel_d, antares_d, rigel_d, aln_d, ten_d};
    double y[n] = {bela_total, betel_total, ant_total, rigel_total, aln_total, ten_total};
    const char* names[]  = {"Bellatrix", "Betelgeuse", "Antares", "Rigel", "Alnilam", "27M"};
    
    auto graph = new TGraph(n, x, y);
    graph->SetTitle("Number of detectable neutrino interactions for CCSN Candidates");
    graph->SetMarkerStyle(21);
    graph->Draw("AP");

    graph->GetXaxis()->SetTitle("Distance [kpc]");
    graph->GetYaxis()->SetTitle("Number of Interactions");

    TLatex *text = new TLatex();
    for (int i = 0; i < 5; i++) {
        text->SetTextSize(0.035);      // Size of the font
        text->SetTextFont(42);         // Standard clean Helvetica font
        text->SetTextAlign(12);         // Align text: left-aligned, vertically centered
        
        // Calculate a small vertical offset so text sits neatly right of/above the point
        Double_t x_pos = x[i] + 0.005; 
        Double_t y_pos = y[i] + 1000 * (12-(2*i));
        
        text->DrawLatex(x_pos, y_pos, names[i]);
    }

    text->DrawLatex(x[5] - 2, y[5] + 5, names[5]);
    c1->SetLogx(1);
    c1->SetLogy(1);

    c1->Update();


    //nu_dist->Draw();

    

}