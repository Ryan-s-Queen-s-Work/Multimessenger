#This folder builts on the DS20k supernova paper (2021) to build the sensitivity study including now 
#the veto system. 
#Authors: Michela Lai, Lucy Kotsioupolou

Instructions:
-Make sure you are in an environment where ROOT is installed
-git clone this repository locally
-within the same folder, run bash setup.sh. This will setup the fluxes taken from SNEWPY, which includes the MSW oscillations within the supernova, as well as creating the needed folders for the following step
-run root -l -q supernova_argon.C. This will produce (takes a while)
+MSW oscillated (Boolig) and not oscillated  (Garching, for now 11 Msun) fluxes
+CeVNS and electron neutrino absorption cross-sections
+differential rate in argon for both of these interactions, for both the models

-----------------------------------
G4DS simulation 
g-4-ds-11-ov is Git cloned g4ds11 from repository https://gitlab.in2p3.fr/peckalec/g-4-ds-11-ov, with the updated
geometry of the OV. 
-Enter the folder at CNAF and run 
conda activate ds
Then 
source configDarSide.sh 
The g-4-ds-11-ov/src folder has a new Generator,DSGeneratorAr40.cc, essentially a G4Gun shooting particles at energies
picked from a csv file in input. The name refers to the ar40 energy recoils, in principle works with
every particle, which is specified in the .mac file. So far, the generator reads the csv file as time in ns 
and energy in keV. An example of usage is given in g-4-ds-11-ov/macros/outervetoar40.mac gives an example of usage.

Finally, the folder input_g4ds contains inputs for this generator, specifically sampled from the differential rates 
created in the diffrate folder after running supernova_argon.C, using the macro create_SNcsv.C, and then time ordered 
with ordered_merged_SN.py, where also the number of events is added to the file, as this must match with the number in 
runbeamOn in the .mac file. 


