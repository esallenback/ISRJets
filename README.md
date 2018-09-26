# Copied-CSV-Files
Copied CSV files from my virtual machine.
Both LPC and UCSB Ntuples were run over, and their separate values were printed into different files, hence files with
"LPC" or "UCSB" in their names. The .C files are various versions of the code that created the csv files. The ORIGINAL... .C
file printed every value in each Ntuple to their respective file (LPC or UCSB); the newest version prints only differences*.
All of the files with "Diff" in the name were created by the new code, so those files only have values that are unique to that group.
For ID variables, like vetoElectronID, for example, the proper ID cut was passsed (if(vetoElectronIDVec[i] == 1){...).

*If at event 1 UCSB has a vector: <1, 2, 3>, and LPC has a vector: <2, 3, 4, 5>, then values 2 and 3 would not be considered as
differences although those values are not in the same corresponding indexes. In the UCSB output file the following would be printed: 
"Event 1,1", and for LPC the following two lines would be printed: "Event 1,4" & "Event 1,5".
