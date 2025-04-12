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

