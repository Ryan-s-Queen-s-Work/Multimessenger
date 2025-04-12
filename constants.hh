double  cspeed = 2.9979246e8; // in m/s

double  fm     = 1e-15; //m
double  htc    = 197.327*fm; // MeV m
double  kg     = 1e-3; // ton
double  cm     = 1e-2;
double  parsec = 3.0857e18*cm; // da pc a m
double  keV    = 1e3;
double  MeV    = 1e6;
double  GeV    = 1e9;
double  amu = 931.49 ; //MeV/c^2
double  hbar   = 6.58212e-22; //in Mev s
double  GF     = 1.166378e-11 ; //MeV-2   
double mN = 39.962383123*amu;
double Z = 18;
double A = 40 ;
double ton = 6.022e23 / A * 1e6; // numero di target in una tonnellata
int n_ene  = 200; //700
double min_e = 0;
double max_e = 100; //MeV /700
double enestep = (max_e - min_e)/n_ene;

int n_erec =500; //200
double min_erec= 0. ;
double max_erec = 0.100; //MeV //50
double erecstep = (max_erec - min_erec)/n_erec ;

int n_time = 6000;
//int n_time = 12379;
double min_t = -4.8313946701e-4 ;
double max_t = 7.600478503686;
double timestep = (max_t - min_t)/n_time;
double max_t_27 = 15.439433579436;
double min_t_27 = -0.349455361517 ;

int n_time_27 = 12000;
double timestep_27 = (max_t_27 - min_t_27)/n_time_27 ;