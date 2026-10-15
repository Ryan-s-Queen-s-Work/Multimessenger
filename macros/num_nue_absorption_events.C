#include <TFile.h>
#include <TTree.h>
#include <TArrayS.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TH2D.h>


void num_nue_absorption_events(const char* filename = "Bollig_27Msun_10kpc_fluxes.root", const char* subdir = "/home/rcurtis/Multimessenger/fluxes") {
    // Mass calculations for target mass
    double mass_tonnes = 3.6; // Example: 10 tonnes of Liquid Argon
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

    if (!h2) {
        printf("Error: TH2D 'my_2d_hist_name' not found!\n");
        return;
    }

    h2->GetXaxis()->SetTitle("Time Since Bounce (s)");
    h2->GetYaxis()->SetTitle("Energy (MeV)");
    h2->Draw("COLZ");

    
    TH1D *h_energy_spec = h2->ProjectionY("h_energy_spec");
    // Scale for different fluxes
    h_energy_spec->Scale(std::pow(10, 2) / std::pow(0.38, 2));

    h_energy_spec->SetTitle("Integrated Neutrino Energy Spectrum");
    h_energy_spec->GetXaxis()->SetTitle("Neutrino Energy [MeV]");
    h_energy_spec->GetYaxis()->SetTitle("Integrated Flux [MeV^{-1}]");

    h_energy_spec->Draw("COLZ");

    // Returns the sum of all bin contents
    TCanvas *c2 = new TCanvas("c2", "Absorption x section", 800, 600);
    double total_flux = h_energy_spec->Integral();

    printf("Total integrated flux: %e\n", total_flux);

    // Get the differential cross sections

    const char* absorb_diff_xsec_path = "/home/rcurtis/Multimessenger/cross_sections/NuE_absorb.root";
    TFile *fAbsorb = TFile::Open(absorb_diff_xsec_path, "READ");
    if (!fAbsorb || fAbsorb->IsZombie()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    // Retrieve the 2D histogram by its exact name in the file
    TH2D *hAbsorb = (TH2D*)fAbsorb->Get("hNuE_abs");

    hAbsorb->Draw("COLZ");

    TCanvas *c3 = new TCanvas("c3", "Interaction Rate via Recoil Energy", 800, 600);

    TAxis *recoil_axis = hAbsorb->GetYaxis();
    // Get the pdf of the recoil energy of an event 
    TH1D *hAbsorb_recoil = new TH1D("Absorb_recoil", "Absorb Event Rate as a function of Neutrino Energy", recoil_axis->GetNbins(), recoil_axis->GetXmin(), recoil_axis->GetXmax());

    //Loop through all possible interactions
    for (int n = 1; n <= hAbsorb_recoil->GetNbinsX(); ++n) {
        double sum = 0;
        for (int j = 1; j <= h_energy_spec->GetNbinsX(); j++){
            double energy_nu = h_energy_spec->GetBinCenter(j);
            double d_energy_nu = h_energy_spec->GetBinWidth(j);

            double flux = h_energy_spec->GetBinContent(j);
            double xsec = hAbsorb->GetBinContent(n,hAbsorb->GetYaxis()->FindBin(energy_nu));
            sum += flux * xsec * d_energy_nu;
        }
        hAbsorb_recoil->SetBinContent(n, sum * N_target);
    
    }

    hAbsorb_recoil->Draw("COLZ");

    double recoil_low  = 10.0; 
    //double recoil_high = 50.0;  

    int bin_low  = hAbsorb_recoil->FindBin(recoil_low);
    //int bin_high = hAbsorb_recoil->FindBin(recoil_high); 
    
    //double total_events_in_window = hAbsorb_recoil->Integral(bin_low, bin_high);
    double total_events_in_window = hAbsorb_recoil->Integral(bin_low, hAbsorb_recoil->GetNbinsX());

    //printf("Total events expected between %.1f and %.1f: %f\n", 
    //    recoil_low, recoil_high, total_events_in_window);

    printf("Total events expected: %f\n", 
         total_events_in_window);

    // Now let's incorporate light yield
    double LY = 6.1; // PE/keV
    double LY_MeV = std::pow(LY, 3);

    // Calculate total PE from this event now
    double Total_PE = 0;

    TCanvas *c4 = new TCanvas("c4", "PE via neutrino energy", 800, 600);
    TH1D *hPEspectrum = new TH1D("PE Spectrum", "Total PE per neutrino energy in Detector", recoil_axis->GetNbins(), recoil_axis->GetXmin(), recoil_axis->GetXmax());

    for (int n = 1; n <= hAbsorb_recoil->GetNbinsX(); n++) {
        double E = hAbsorb_recoil->GetBinCenter(n);

        double N = hAbsorb_recoil->GetBinContent(n);

        double PE = E * LY_MeV; 

        Total_PE += PE * N;
        hPEspectrum->SetBinContent(n, PE);
    }
    hPEspectrum->Draw("COLZ");

    printf("Total PE from Adsorption: %f\n", Total_PE);

}