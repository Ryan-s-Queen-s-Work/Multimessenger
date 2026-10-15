#include <TFile.h>
#include <TTree.h>
#include <TArrayS.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TH2D.h>


void num_nue_cevns_events(const char* filename = "Bollig_27Msun_10kpc_fluxes.root", const char* subdir = "/home/rcurtis/Multimessenger/fluxes") {
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
    TH2D *h2 = (TH2D*)f->Get("h2_xn_bar_NO_Bollig_flux");

    if (!h2) {
        printf("Error: TH2D 'my_2d_hist_name' not found!\n");
        return;
    }

    h2->GetXaxis()->SetTitle("Time Since Bounce (s)");
    h2->GetYaxis()->SetTitle("Energy (MeV)");
    h2->Draw("COLZ");

    
    TH1D *h_energy_spec = h2->ProjectionY("h_energy_spec");
    //scale for different fluxes
    h_energy_spec->Scale(std::pow(10, 2) / std::pow(0.38, 2));

    h_energy_spec->SetTitle("Integrated Neutrino Energy Spectrum");
    h_energy_spec->GetXaxis()->SetTitle("Neutrino Energy [MeV]");
    h_energy_spec->GetYaxis()->SetTitle("Integrated Flux [MeV^{-1}]");

    //h_energy_spec->Draw("COLZ");

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
    double recoil_high = 50.0;  

    int bin_low  = hCEvNS_recoil->FindBin(recoil_low);
    int bin_high = hCEvNS_recoil->FindBin(recoil_high); 
    
    double total_events_in_window = hCEvNS_recoil->Integral(bin_low, bin_high);

    printf("Total events expected between %.1f and %.1f: %f\n", 
        recoil_low, recoil_high, total_events_in_window);

    //Now calcualting the number of photo electrons produced in the CEvNS events

    double LY = 6.1; // PE/keV

    double L_eff_values[] = {0.250, 0.262, 0.274, 0.286, 0.297, 0.306, 0.313};
    double NR_energy_values[] = {16.8, 25.4, 36.1, 49.9, 70.6, 96.9, 129.4}; //keV_nr

    TGraph *L_eff_graph = new TGraph();
    L_eff_graph->SetPoint(0, 16.8, 0.250);
    L_eff_graph->SetPoint(1, 25.4, 0.262);
    L_eff_graph->SetPoint(2, 36.1, 0.274);
    L_eff_graph->SetPoint(3, 49.9, 0.286);
    L_eff_graph->SetPoint(4, 70.6, 0.297);
    L_eff_graph->SetPoint(5, 96.9, 0.305);
    L_eff_graph->SetPoint(6, 129.4, 0.313);

    TArrayD L_eff_array(7, L_eff_values);
    TArrayD NR_energy_array(7, NR_energy_values);

    TCanvas *c4 = new TCanvas("c4", "PE Rate via Recoil Energy", 800, 600);
    TH1D *hCEvNSPE = new TH1D("CEvNS PE", "PE Rate via Recoil Energy", recoil_axis->GetNbins(), recoil_axis->GetXmin(), recoil_axis->GetXmax());
    hCEvNSPE->GetXaxis()->SetTitle("Recoil Energy (keV)");
    hCEvNSPE->GetYaxis()->SetTitle("PE/N_{CEvNS}");

    double PE_threshold = 0;
    double Total_PE = 0; //sum of total PE deposited in the detector
    double N_cevns_above90 = 0; //Sum of CEvNS events that produce 90 or more PE
    for (int n = 1; n <= hCEvNS_recoil->GetNbinsX(); n++) {
        double N = hCEvNS_recoil->GetBinContent(n);
        double E_rec = hCEvNS_recoil->GetBinCenter(n);

        double L_eff = 0;
        for (int i = 0; i < NR_energy_array.GetSize() - 1; i++) {
            L_eff = L_eff_graph->Eval(E_rec);
            //if (E_rec >= NR_energy_array[i] && E_rec < NR_energy_array[i+1]) {
            //    double Emax = NR_energy_array[i+1];
            //    double Emin = NR_energy_array[i];
            //    double Lmax = L_eff_array[i+1];
            //    double Lmin = L_eff_array[i];
            //    double m = (Lmax - Lmin)/(Emax - Emin);
            //    double b = Lmin - m * Emin;
            //    L_eff = E_rec * m + b;
            //    continue;
            //}
        }
        double PE = L_eff * E_rec * LY;
        //printf("PE for Bin %f\n: %f\n", E_rec, PE);
        printf("PE: %f\n", PE);
        if (PE <= 90) {
            Total_PE += 0;
            if (n + 1 > PE_threshold){
                PE_threshold = n + 1;
            }
        }
        else{
            hCEvNSPE->SetBinContent(n, PE);
            Total_PE += PE * N;
            //N_cevns_above90 += N;
        }

    }
    N_cevns_above90 = hCEvNS_recoil->Integral(PE_threshold, hCEvNS_recoil->GetNbinsX());
    hCEvNSPE->Draw("HISTO");
    printf("Total PE from Adsorption: %f\n", Total_PE);
    printf("Total CEvNS events above 90PE: %f\n", N_cevns_above90);

}