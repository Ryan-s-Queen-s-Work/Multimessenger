//Script to get fluxes from different models, for different neutrino flavours
//and mass hierarchies from SNEWPY and Garching Group. They are put into histograms and in a root file. 

//In Garching rgoup case
//In SNEWPY case, first run fluxes_from_snewpy.py editing code if you want to add more models,
//or just adding printing values to text file. The values from the .txt file will be used in this
//programme to create the histograms.

//To run:

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//============ Contstant values and files within the programmes =============


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_Bollig_fluxes(bool doBollig=true){

  cout << "------------ Starting to run, sit tight! ------------" << endl;

/*
    //==========================================================================
    //============================== Garching fluxes ===========================
    
    //--------------------Open files and get histos
    
    //Open files
    TFile *nu_fluxes; //Contains 3 (flavours) 2D differential flux histos generated from running supernova.C (see readme.txt)
    nu_fluxes = TFile::Open("/home/michela/multi-messenger/fluxes/nu_fluxes.root", "READ"); //fixthis path
    
    
    //Get histos from files
    TH2D *h2_en_NOS_Garching_flux; //Diff flux for electron neutrinos
    h2_en_NOS_Garching_flux = (TH2D*)nu_fluxes -> Get("hNuEFlux");
    TH2D *h2_aen_NOS_Garching_flux; //Diff flux for anti-electron neutrinos
    h2_aen_NOS_Garching_flux = (TH2D*)nu_fluxes -> Get("hNuEbFlux");
    TH2D *h2_xn_NOS_Garching_flux; //Diff flux for other-flavoured neutrinos (only one flavour, so for all other cases, x4)
    h2_xn_NOS_Garching_flux = (TH2D*)nu_fluxes -> Get("hNuXFlux");
    
    //Remove reference of histos to any directory
    h2_en_NOS_Garching_flux  -> SetDirectory(0);
    h2_aen_NOS_Garching_flux -> SetDirectory(0);
    h2_xn_NOS_Garching_flux  -> SetDirectory(0);
    
    nu_fluxes -> Close();
    
    
    //------------------------Projections
    
    //Flux projections
    TH1D *h_en_NOS_Garching_fluenceE  = (TH1D*) h2_en_NOS_Garching_flux  -> ProjectionY(); //En
    TH1D *h_en_NOS_Garching_fluenceT  = (TH1D*) h2_en_NOS_Garching_flux  -> ProjectionX(); //t
    
    TH1D *h_aen_NOS_Garching_fluenceE = (TH1D*) h2_aen_NOS_Garching_flux  -> ProjectionY(); //En
    TH1D *h_aen_NOS_Garching_fluenceT = (TH1D*) h2_aen_NOS_Garching_flux  -> ProjectionX(); //t
    
    TH1D *h_xn_NOS_Garching_fluenceE  = (TH1D*) h2_xn_NOS_Garching_flux  -> ProjectionY(); //En
    TH1D *h_xn_NOS_Garching_fluenceT  = (TH1D*) h2_xn_NOS_Garching_flux  -> ProjectionX(); //


    
    //----------------------Saving histos in root file
    TFile *outfile1 = TFile::Open("fluxes/Garching_fluxes.root","RECREATE"); //create out root file for saving histograms in
    
    h2_en_NOS_Garching_flux    -> Write("h2_en_NOS_Garching_flux");
    h2_aen_NOS_Garching_flux   -> Write("h2_aen_NOS_Garching_flux");
    h2_xn_NOS_Garching_flux    -> Write("h2_xn_NOS_Garching_flux");
    
    h_en_NOS_Garching_fluenceE  -> Write("h_en_NOS_Garching_fluenceE");
    h_en_NOS_Garching_fluenceT  -> Write("h_en_NOS_Garching_fluenceT"); //fix this, add title and axis names
    
    h_aen_NOS_Garching_fluenceE -> Write("h_aen_NOS_Garching_fluenceE");
    h_aen_NOS_Garching_fluenceT -> Write("h_aen_NOS_Garching_fluenceT");
    
    h_xn_NOS_Garching_fluenceE  -> Write("h_xn_NOS_Garching_fluenceE");
    h_xn_NOS_Garching_fluenceT  -> Write("h_xn_NOS_Garching_fluenceT");
    
      
    cout << "------ All Garching histograms have been saved to file" << endl;
    outfile1 -> Close();

    
  */
  //==========================================================================
  //============ SNEWPY fluxes =============================================

  //-------------------------------------------Values from running fluxes_from_snewpy.py for diff fluxes and axes and reading .txt files

  
  //============= Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for ELECTRON neutrinos
  if(doBollig){
    cout << "------ Differential fluxes from Bollig (2016) model [1/MeV m^2 s] for ELECTRON neutrinos ------" << endl;

    //=====================
    //====== No oscillations
    std::vector<double> fluxesE_Bollig_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesE_Bollig;
    double   fluxesE_Bollig_val; //[4200000]
    string   ena, dio, tria, tessera, pente, efta; //just some variables to get all columns
    char     buffer2[400];
    
    fluxesE_Bollig.open("./fluxes_data/Bollig/Bollig_27Msun_NOS_NuE",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesE_Bollig.getline (buffer2,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesE_Bollig >> ena >> dio >> tria >> tessera >> pente >> fluxesE_Bollig_val >> efta; //getting through file columns
      
      fluxesE_Bollig_array.push_back(fluxesE_Bollig_val); //storing values to array 
      
      if(fluxesE_Bollig.eof()) break;
      
    } //end while(1)
    fluxesE_Bollig.close();


    //=====================
    //====== Normal mass ordering assumption
    std::vector<double> fluxesE_Bollig_NO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesE_Bollig_NO;
    double   fluxesE_Bollig_NO_val; //[4200000]
    string   un, deux, trois, quatre, cinq, sept; //just some variables to get all columns
    char     buffer3[400];
    
    fluxesE_Bollig_NO.open("./fluxes_data/Bollig/Bollig_27Msun_NO_NuE",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesE_Bollig_NO.getline (buffer3,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesE_Bollig_NO >> un >> deux >> trois >> quatre >> cinq >> fluxesE_Bollig_NO_val >> sept; //getting through file columns
      
      fluxesE_Bollig_NO_array.push_back(fluxesE_Bollig_NO_val); //storing values to array 
      
      if(fluxesE_Bollig_NO.eof()) break;
      
    } //end while(1)
    fluxesE_Bollig_NO.close();


    //=====================
    //====== Inverted mass ordering assumption
    std::vector<double> fluxesE_Bollig_IO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesE_Bollig_IO;
    double   fluxesE_Bollig_IO_val; //[4200000]
    string   one, two, three, four, five, seven; //just some variables to get all columns
    char     buffer4[400];
    
    fluxesE_Bollig_IO.open("./fluxes_data/Bollig/Bollig_27Msun_IO_NuE",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesE_Bollig_IO.getline (buffer4,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesE_Bollig_IO >> one >> two >> three >> four >> five >> fluxesE_Bollig_IO_val >> seven; //getting through file columns
      
      fluxesE_Bollig_IO_array.push_back(fluxesE_Bollig_IO_val); //storing values to array 
      
      if(fluxesE_Bollig_IO.eof()) break;
      
    } //end while(1)
    fluxesE_Bollig_IO.close();
    

        cout << "------ Differential fluxes from Bollig (2016) model [1/MeV m^2 s] for Nu X (either mu or tau flavour) neutrinos ------" << endl;

    //=====================
    //====== No oscillations
    std::vector<double> fluxesX_Bollig_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesX_Bollig;
    double   fluxesX_Bollig_val; //[4200000]
    string   enax, diox, triax, tesserax, pentex, eftax; //just some variables to get all columns
    char     buffer2x[400];
    
    fluxesX_Bollig.open("./fluxes_data/Bollig/Bollig_27Msun_NOS_NuX",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesX_Bollig.getline (buffer2x,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesX_Bollig >> enax >> diox >> triax >> tesserax >> pentex >> fluxesX_Bollig_val >> eftax; //getting through file columns
      
      fluxesX_Bollig_array.push_back(fluxesX_Bollig_val); //storing values to array 
      
      if(fluxesX_Bollig.eof()) break;
      
    } //end while(1)
    fluxesX_Bollig.close();


    //=====================
    //====== Normal mass ordering assumption
    std::vector<double> fluxesX_Bollig_NO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesX_Bollig_NO;
    double   fluxesX_Bollig_NO_val; //[4200000]
    string   unx, deuxx, troisx, quatrex, cinqx, septx; //just some variables to get all columns
    char     buffer3x[400];
    
    fluxesX_Bollig_NO.open("./fluxes_data/Bollig/Bollig_27Msun_NO_NuX",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesX_Bollig_NO.getline (buffer3x,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesX_Bollig_NO >> unx >> deuxx >> troisx >> quatrex >> cinqx >> fluxesX_Bollig_NO_val >> septx; //getting through file columns
      
      fluxesX_Bollig_NO_array.push_back(fluxesX_Bollig_NO_val); //storing values to array 
      
      if(fluxesX_Bollig_NO.eof()) break;
      
    } //end while(1)
    fluxesX_Bollig_NO.close();


    //=====================
    //====== Inverted mass ordering assumption
    std::vector<double> fluxesX_Bollig_IO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesX_Bollig_IO;
    double   fluxesX_Bollig_IO_val; //[4200000]
    string   onex, twox, threex, fourx, fivex, sevenx; //just some variables to get all columns
    char     buffer4x[400];
    
    fluxesX_Bollig_IO.open("./fluxes_data/Bollig/Bollig_27Msun_IO_NuX",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesX_Bollig_IO.getline (buffer4x,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesX_Bollig_IO >> onex >> twox >> threex >> fourx >> fivex >> fluxesX_Bollig_IO_val >> sevenx; //getting through file columns
      
      fluxesX_Bollig_IO_array.push_back(fluxesX_Bollig_IO_val); //storing values to array 
      
      if(fluxesX_Bollig_IO.eof()) break;
      
    } //end while(1)
    fluxesX_Bollig_IO.close();


    cout << "------ Differential fluxes from Bollig (2016) model [1/MeV m^2 s] for electron  antineutrinos ------" << endl;

    //=====================
    //====== No oscillations
    std::vector<double> fluxesEb_Bollig_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesEb_Bollig;
    double   fluxesEb_Bollig_val; //[4200000]
    string   enab, diob, triab, tesserab, penteb, eftab; //just some variables to get all columns
    char     buffer2b[400];
    
    fluxesEb_Bollig.open("./fluxes_data/Bollig/Bollig_27Msun_NOS_NuEb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesEb_Bollig.getline (buffer2b,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesEb_Bollig >> enab >> diob >> triab >> tesserab >> penteb >> fluxesEb_Bollig_val >> eftab; //getting through file columns
      
      fluxesEb_Bollig_array.push_back(fluxesEb_Bollig_val); //storing values to array 
      
      if(fluxesEb_Bollig.eof()) break;
      
    } //end while(1)
    fluxesEb_Bollig.close();


    //=====================
    //====== Normal mass ordering assumption
    std::vector<double> fluxesEb_Bollig_NO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesEb_Bollig_NO;
    double   fluxesEb_Bollig_NO_val; //[4200000]
    string   unb, deuxb, troisb, quatreb, cinqb, septb; //just some variables to get all columns
    char     buffer3b[400];
    
    fluxesEb_Bollig_NO.open("./fluxes_data/Bollig/Bollig_27Msun_NO_NuEb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesEb_Bollig_NO.getline (buffer3b,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesEb_Bollig_NO >> unb >> deuxb >> troisb >> quatreb >> cinqb >> fluxesEb_Bollig_NO_val >> septb; //getting through file columns
      
      fluxesEb_Bollig_NO_array.push_back(fluxesEb_Bollig_NO_val); //storing values to array 
      
      if(fluxesEb_Bollig_NO.eof()) break;
      
    } //end while(1)
    fluxesEb_Bollig_NO.close();


    //=====================
    //====== Inverted mass ordering assumption
    std::vector<double> fluxesEb_Bollig_IO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesEb_Bollig_IO;
    double   fluxesEb_Bollig_IO_val; //[4200000]
    string   oneb, twob, threeb, fourb, fiveb, sevenb; //just some variables to get all columns
    char     buffer4b[400];
    
    fluxesEb_Bollig_IO.open("./fluxes_data/Bollig/Bollig_27Msun_IO_NuEb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesEb_Bollig_IO.getline (buffer4b,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesEb_Bollig_IO >> oneb >> twob >> threeb >> fourb >> fiveb >> fluxesEb_Bollig_IO_val >> sevenb; //getting through file columns
      
      fluxesEb_Bollig_IO_array.push_back(fluxesEb_Bollig_IO_val); //storing values to array 
      
      if(fluxesEb_Bollig_IO.eof()) break;
      
    } //end while(1)
    fluxesEb_Bollig_IO.close();


    cout << "------ Differential fluxes from Bollig (2016) model [1/MeV m^2 s] for Nux  antineutrinos ------" << endl;

    //=====================
    //====== No oscillations
    std::vector<double> fluxesXb_Bollig_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesXb_Bollig;
    double   fluxesXb_Bollig_val; //[4200000]
    string   enaxb, dioxb, triaxb, tesseraxb, pentexb, eftaxb; //just some variables to get all columns
    char     buffer2xb[400];
    
    fluxesXb_Bollig.open("./fluxes_data/Bollig/Bollig_27Msun_NOS_NuXb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesXb_Bollig.getline (buffer2xb,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesXb_Bollig >> enaxb >> dioxb >> triaxb >> tesseraxb >> pentexb >> fluxesXb_Bollig_val >> eftaxb; //getting through file columns
      
      fluxesXb_Bollig_array.push_back(fluxesXb_Bollig_val); //storing values to array 
      
      if(fluxesXb_Bollig.eof()) break;
      
    } //end while(1)
    fluxesXb_Bollig.close();


    //=====================
    //====== Normal mass ordering assumption
    std::vector<double> fluxesXb_Bollig_NO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesXb_Bollig_NO;
    double   fluxesXb_Bollig_NO_val; //[4200000]
    string   unxb, deuxxb, troisxb, quatrexb, cinqxb, septxb; //just some variables to get all columns
    char     buffer3xb[400];
    
    fluxesXb_Bollig_NO.open("./fluxes_data/Bollig/Bollig_27Msun_NO_NuXb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesXb_Bollig_NO.getline (buffer3xb,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesXb_Bollig_NO >> unxb >> deuxxb >> troisxb >> quatrexb >> cinqxb >> fluxesXb_Bollig_NO_val >> septxb; //getting through file columns
      
      fluxesXb_Bollig_NO_array.push_back(fluxesXb_Bollig_NO_val); //storing values to array 
      
      if(fluxesXb_Bollig_NO.eof()) break;
      
    } //end while(1)
    fluxesXb_Bollig_NO.close();


    //=====================
    //====== Inverted mass ordering assumption
    std::vector<double> fluxesXb_Bollig_IO_array; //to store values from .txt file
    
    //Get from file
    ifstream fluxesXb_Bollig_IO;
    double   fluxesXb_Bollig_IO_val; //[4200000]
    string   onexb, twoxb, threexb, fourxb, fivexb, sevenxb; //just some variables to get all columns
    char     buffer4xb[400];
    
    fluxesXb_Bollig_IO.open("./fluxes_data/Bollig/Bollig_27Msun_IO_NuXb",ios::in);
    
    for(int i=0;i<3;++i){
      fluxesXb_Bollig_IO.getline (buffer4xb,400);
    }
    
    while(1){ // loop that goes through lines of .txt file
      
      fluxesXb_Bollig_IO >> onexb >> twoxb >> threexb >> fourxb >> fivexb >> fluxesXb_Bollig_IO_val >> sevenxb; //getting through file columns
      
      fluxesXb_Bollig_IO_array.push_back(fluxesXb_Bollig_IO_val); //storing values to array 
      
      if(fluxesXb_Bollig_IO.eof()) break;
      
    } //end while(1)
    fluxesXb_Bollig_IO.close();

    
    //---------------------------------------------Creating histograms of fluxes and fluences-----------------

    double min_enu = 0;
    double max_enu = 200;
    int n_enu = 200;
    double min_t = 0;
    double  max_t = 15;
    int n_t = 12000;

    double enu_step = (max_enu- min_enu)/n_enu;
    double time_step = (max_t - min_t)/n_t;
    
    cout << " Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for ELECTRON neutrinos" << endl;
    //No oscillations
    TH2D *h2_en_NOS_Bollig_flux;
    h2_en_NOS_Bollig_flux = new TH2D("h2_en_NOS_Bollig_flux","Electron neutrino fluxes for a 10kpc supernova from the Bollig (2016) model", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    int k = 0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
	
	h2_en_NOS_Bollig_flux -> SetBinContent(i, j, fluxesE_Bollig_array[k] *enu_step * time_step);
	
	k++; //variable to go through long array of flux values, see .txt
	
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ No Osc histogram created" << endl;


    //Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for ELECTRON neutrinos
    //Normal mass ordering case
    TH2D *h2_en_NO_Bollig_flux;
    h2_en_NO_Bollig_flux = new TH2D("h2_en_NO_Bollig_flux","Electron neutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming NMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    int m = 0;
    for (int i=0; i<n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
	
	h2_en_NO_Bollig_flux -> SetBinContent(i, j, fluxesE_Bollig_NO_array[m]*enu_step * time_step);
	
	m++;
	
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Normal M.O. histogram created" << endl;


    //Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for ELECTRON neutrinos
    //Inverted mass ordering case
    TH2D *h2_en_IO_Bollig_flux;
    h2_en_IO_Bollig_flux = new TH2D("h2_en_IO_Bollig_flux","Electron neutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming IMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    int n = 0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
	
	h2_en_IO_Bollig_flux -> SetBinContent(i, j, fluxesE_Bollig_IO_array[n]*enu_step * time_step);
	
	n++;
	
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Inverted M.O. histogram created" << endl;
    
    

    
    cout << "Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for either tau or muon  neutrinos NuX" << endl;
    //No oscillations
    TH2D *h2_xn_NOS_Bollig_flux;
    h2_xn_NOS_Bollig_flux = new TH2D("h2_xn_NOS_Bollig_flux","Either Tau or muon neutrino fluxes for a 10kpc supernova from the Bollig (2016) model", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    k=0;
    for (int i=0; i<n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, xnergy = 700
  
  h2_xn_NOS_Bollig_flux -> SetBinContent(i, j, fluxesX_Bollig_array[k]*enu_step * time_step);
  
  k++; //variable to go through long array of flux values, see .txt
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ No Osc histogram created" << endl;


    //Normal mass ordering case
    TH2D *h2_xn_NO_Bollig_flux;
    h2_xn_NO_Bollig_flux = new TH2D("h2_xn_NO_Bollig_flux","Either Tau or muon neutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming NMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    m=0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
  
  h2_xn_NO_Bollig_flux -> SetBinContent(i, j, fluxesX_Bollig_NO_array[m]*enu_step * time_step);
  
  m++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Normal M.O. histogram created" << endl;


    //Inverted mass ordering case
    TH2D *h2_xn_IO_Bollig_flux;
    h2_xn_IO_Bollig_flux = new TH2D("h2_xn_IO_Bollig_flux","Either Tau or muon neutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming IMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    n=0;
    for (int i=0; i< n_t; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu; j++){ //y axis, energy = 700
  
  h2_xn_IO_Bollig_flux -> SetBinContent(i, j, fluxesX_Bollig_IO_array[n]*enu_step * time_step);
  
  n++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Inverted M.O. histogram created" << endl;
    



    cout << "Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for ELECTRON ANTIneutrinos" << endl;
    //No oscillations
    TH2D *h2_en_bar_NOS_Bollig_flux;
    h2_en_bar_NOS_Bollig_flux = new TH2D("h2_en_bar_NOS_Bollig_flux","Electron antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    k=0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
  
  h2_en_bar_NOS_Bollig_flux -> SetBinContent(i, j, fluxesEb_Bollig_array[k]*enu_step * time_step);
  
  k++; //variable to go through long array of flux values, see .txt
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ No Osc histogram created" << endl;


    //Normal mass ordering case
    TH2D *h2_en_bar_NO_Bollig_flux;
    h2_en_bar_NO_Bollig_flux = new TH2D("h2_en_bar_NO_Bollig_flux","Electron antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming NMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    m=0;
    for (int i=0; i<n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
  
  h2_en_bar_NO_Bollig_flux -> SetBinContent(i, j, fluxesEb_Bollig_NO_array[m]*enu_step * time_step);
  
  m++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Normal M.O. histogram created" << endl;


    //Inverted mass ordering case
    TH2D *h2_en_bar_IO_Bollig_flux;
    h2_en_bar_IO_Bollig_flux = new TH2D("h2_en_bar_IO_Bollig_flux","Electron antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming IMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    n=0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
  
  h2_en_bar_IO_Bollig_flux -> SetBinContent(i, j, fluxesEb_Bollig_IO_array[n]*enu_step * time_step);
  
  n++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Inverted M.O. histogram created" << endl;
    


    
    cout << "Differential fluxes for Bollig (2016) model [1/MeV m^2 s] for either tau or muon  antineutrinos NuX" << endl;
    //No oscillations
    TH2D *h2_xn_bar_NOS_Bollig_flux;
    h2_xn_bar_NOS_Bollig_flux = new TH2D("h2_xn_bar_NOS_Bollig_flux","Either Tau or muon antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    k=0;
    for (int i=0; i<n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, xnergy = 700
  
  h2_xn_bar_NOS_Bollig_flux -> SetBinContent(i, j, fluxesXb_Bollig_array[k]*enu_step * time_step);
  
  k++; //variable to go through long array of flux values, see .txt
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ No Osc histogram created" << endl;

    //Normal mass ordering case
    TH2D *h2_xn_bar_NO_Bollig_flux;
    h2_xn_bar_NO_Bollig_flux = new TH2D("h2_xn_bar_NO_Bollig_flux","Either Tau or muon antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming NMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    m=0;
    for (int i=0; i< n_t ; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu ; j++){ //y axis, energy = 700
  
  h2_xn_bar_NO_Bollig_flux -> SetBinContent(i, j, fluxesXb_Bollig_NO_array[m]*enu_step * time_step);
  
  m++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Normal M.O. histogram created" << endl;


    //Inverted mass ordering case
    TH2D *h2_xn_bar_IO_Bollig_flux;
    h2_xn_bar_IO_Bollig_flux = new TH2D("h2_xn_bar_IO_Bollig_flux","Either Tau or muon antineutrino fluxes for a 10kpc supernova from the Bollig (2016) model assuming IMO", n_t, min_t, max_t, n_enu, min_enu, max_enu);
    
    //Filling histogram
    n=0;
    for (int i=0; i< n_t; i++){ //x axis, time = 6000
      
      for (int j=0; j< n_enu; j++){ //y axis, energy = 700
  
  h2_xn_bar_IO_Bollig_flux -> SetBinContent(i, j, fluxesXb_Bollig_IO_array[n]*enu_step * time_step);
  
  n++;
  
      } // y axis, energy    
    } // x axis, time
    
    cout << "------ Inverted M.O. histogram created" << endl;
    














 
    //-----------------------------------Save histograms in .root files
    TFile *outfile = TFile::Open("./fluxes/Bollig_27Msun_10kpc_fluxes.root","RECREATE"); //create out root file for saving histograms in
    
    h2_en_NOS_Bollig_flux    -> Write("h2_en_NOS_Bollig_flux");
    h2_en_NO_Bollig_flux -> Write("h2_en_NO_Bollig_flux");
    h2_en_IO_Bollig_flux -> Write("h2_en_IO_Bollig_flux");

    h2_en_bar_NOS_Bollig_flux    -> Write("h2_en_bar_NOS_Bollig_flux");
    h2_en_bar_NO_Bollig_flux -> Write("h2_en_bar_NO_Bollig_flux");
    h2_en_bar_IO_Bollig_flux -> Write("h2_en_bar_IO_Bollig_flux");

    h2_xn_NOS_Bollig_flux    -> Write("h2_xn_NOS_Bollig_flux");
    h2_xn_NO_Bollig_flux -> Write("h2_xn_NO_Bollig_flux");
    h2_xn_IO_Bollig_flux -> Write("h2_xn_IO_Bollig_flux");

    h2_xn_bar_NOS_Bollig_flux    -> Write("h2_xn_bar_NOS_Bollig_flux");
    h2_xn_bar_NO_Bollig_flux -> Write("h2_xn_bar_NO_Bollig_flux");
    h2_xn_bar_IO_Bollig_flux -> Write("h2_xn_bar_IO_Bollig_flux");


    
    /*
    //Make and save projections in .root files
    TH1D *h_en_NOS_Bollig_fluenceE       = (TH1D*) h2_en_NOS_Bollig_flux    -> ProjectionY(); //En
    TH1D *h_en_NOS_Bollig_fluenceT       = (TH1D*) h2_en_NOS_Bollig_flux    -> ProjectionX(); //t
    
    TH1D *h_en_NO_Bollig_fluenceE    = (TH1D*) h2_en_NO_Bollig_flux -> ProjectionY(); //En
    TH1D *h_en_NO_Bollig_fluenceT    = (TH1D*) h2_en_NO_Bollig_flux -> ProjectionX(); //t

    TH1D *h_en_IO_Bollig_fluenceE    = (TH1D*) h2_en_IO_Bollig_flux -> ProjectionY(); //En
    TH1D *h_en_IO_Bollig_fluenceT    = (TH1D*) h2_en_IO_Bollig_flux -> ProjectionX(); //t
    
    h_en_NOS_Bollig_fluenceE    -> Write("h_en_NOS_Bollig_fluenceE");
    h_en_NOS_Bollig_fluenceT    -> Write("h_en_NOS_Bollig_fluenceT");
    h_en_NO_Bollig_fluenceE -> Write("h_en_NO_Bollig_fluenceE"); //fix this, add axis names to .root file
    h_en_NO_Bollig_fluenceT -> Write("h_en_NO_Bollig_fluenceT");
    h_en_IO_Bollig_fluenceE -> Write("h_en_IO_Bollig_fluenceE");
    h_en_IO_Bollig_fluenceT -> Write("h_en_IO_Bollig_fluenceT");*/


    
    cout << "------ All SNEWPY histograms have been saved to file" << endl;
    outfile -> Close();

  } // end if doBollig


  cout << "========================" << endl;
  cout << "=== Finished! Enjoy! ===" << endl;
  cout << "========================" << endl;
} // end void make_histos()


////////////////////////////////////////////////////////////
//To add another SNEWPY model, copy lines and repeat process
////////////////////////////////////////////////////////////
