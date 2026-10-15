#include <TFile.h>
#include <TTree.h>
#include <TArrayS.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TH2D.h>


void num_nu_events(const char* filename = "Bollig_27Msun_10kpc_fluxes.root", const char* subdir = "/home/rcurtis/Multimessenger/fluxes") {
    // Mass calculations for target mass
    double mass_tonnes = 1.0; // Example: 10 tonnes of Liquid Argon
    double mass_grams = mass_tonnes * 1e6;
    double N_target = (mass_grams / 39.948) * 6.02214e23;
    
    TCanvas *c1 = new TCanvas("c1", "Neutrino Fluence", 800, 600);

    // Open the file
    TString fullPath = TString::Format("%s/%s", subdir, filename);
    TFile *f = TFile::Open(fullPath, "READ");
    if (!f || f->IsZombie()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }


    // Retrieve the 2D histogram by its exact name in the file
    TH2D *h2 = (TH2D*)f->Get("h2_en_NO_Bollig_flux");
    TH2D *h3 = (TH2D*)f->Get("h2_xn_NO_Bollig_flux");
    TH2D *h4 = (TH2D*)f->Get("h2_en_bar_NO_Bollig_flux");
    TH2D *h5 = (TH2D*)f->Get("h2_xn_bar_NO_Bollig_flux");

    if (!h2) {
        printf("Error: TH2D 'my_2d_hist_name' not found!\n");
        return;
    }

    h2->GetXaxis()->SetTitle("Time Since Bounce (s)");
    h2->GetYaxis()->SetTitle("Energy (MeV)");
    h2->Draw("COLZ");

    
    TH1D *h_energy_spec = h2->ProjectionY("h_energy_spec");
    TH1D *h_energy_spec_xn = h3->ProjectionY("h_energy_spec");
    TH1D *h_energy_spec_en_bar = h4->ProjectionY("h_energy_spec");
    TH1D *h_energy_spec_xn_bar = h5->ProjectionY("h_energy_spec");

    h_energy_spec->Add(h_energy_spec_xn);
    h_energy_spec->Add(h_energy_spec_en_bar);
    h_energy_spec->Add(h_energy_spec_xn_bar);

    h_energy_spec->SetTitle("Integrated Neutrino Energy Spectrum");
    h_energy_spec->GetXaxis()->SetTitle("Neutrino Energy [MeV]");
    h_energy_spec->GetYaxis()->SetTitle("Integrated Flux [MeV^{-1}]");

    h_energy_spec->Draw("COLZ");

    // Returns the sum of all bin contents
    TCanvas *c2 = new TCanvas("c2", "CEvNS x section", 800, 600);
    double total_flux = h_energy_spec->Integral();

    printf("Total integrated flux: %e\n", total_flux);

    // Get the differential cross sections

    const char* CEvNS_diff_xsec_path = "/home/rcurtis/Multimessenger/cross_sections/CEvNS.root";
    TFile *fCEvNS = TFile::Open(CEvNS_diff_xsec_path, "READ");
    if (!fCEvNS || fCEvNS->IsZombie()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    // Retrieve the 2D histogram by its exact name in the file
    TH2D *hCEvNS = (TH2D*)fCEvNS->Get("hCEvNS");

    hCEvNS->Draw("COLZ");

    TCanvas *c3 = new TCanvas("c3", "Interaction Rate via Recoil Energy", 800, 600);

    TAxis *recoil_axis = hCEvNS->GetYaxis();
    // Get the pdf of the recoil energy of an event 
    TH1D *hCEvNS_recoil = new TH1D("CEvNS_recoil", "CEvNS Event Rate as a function of Recoil Energy", recoil_axis->GetNbins(), recoil_axis->GetXmin(), recoil_axis->GetXmax());

    //Loop through all possible interactions
    for (int n = 1; n <= hCEvNS_recoil->GetNbinsX(); ++n) {
        double sum = 0;
        for (int j = 1; j <= h_energy_spec->GetNbinsX(); j++){
            double energy_nu = h_energy_spec->GetBinCenter(j);
            double d_energy_nu = h_energy_spec->GetBinWidth(j);

            double flux = h_energy_spec->GetBinContent(j);
            double xsec = hCEvNS->GetBinContent(n,hCEvNS->GetYaxis()->FindBin(energy_nu));
            sum += flux * xsec * d_energy_nu;
        }
        hCEvNS_recoil->SetBinContent(n, sum * N_target);
    
    }

    hCEvNS_recoil->Draw("COLZ");

    double recoil_low  = 10.0; 
    //double recoil_high = 50.0;  

    int bin_low  = hCEvNS_recoil->FindBin(recoil_low);
    //int bin_high = hCEvNS_recoil->FindBin(recoil_high); 
    
    double total_events_in_window = hCEvNS_recoil->Integral(bin_low, hCEvNS_recoil->GetNbinsX());

    //printf("Total events expected between %.1f and %.1f: %f\n", 
    //    recoil_low, recoil_high, total_events_in_window);
    printf("Total events expected abopve %.1f: %f\n", 
        recoil_low, total_events_in_window);
}