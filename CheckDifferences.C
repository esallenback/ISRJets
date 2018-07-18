#include "NTupleReader.h"
#include "StopleAlias.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "TH1.h"
#include "TRandom3.h"
#include <getopt.h>

int main(int argc, char* argv[])
{               
    if (argc-1!= 2)
    {
      std::cerr <<"Please give 2 arguments: " << "filelist1 " << " " << "filelist2" << std::endl;
      std::cerr <<" Valid configurations are " << std::endl;
      std::cerr <<" ./CheckDifferences.C FileList/UCSBData.list FileList/LPCData.C" << std::endl;
      return -1;
    }
  const char *inputFileList1 = argv[1];
  const char *inputFileList2 = argv[2];

  if (strstr(inputFileList1, "LPC") != NULL)
    {
      std::cout<<"Please put UCSB as the first file and LPC as the second"<<std::endl;
      return -1;
    }
  TChain *UCSBChain = NULL;
  TChain *LPCChain = NULL;
  UCSBChain = new TChain("Events");
  LPCChain = new TChain("stopTreeMaker/AUX");
  //new stuff
  std::fstream inputUCSB(inputFileList1);
  std::fstream inputLPC(inputFileList2);
 
  if(!inputUCSB.is_open())
    {
      std::cerr << "** ERROR: Can't open UCSB Dataset for input" << std::endl;
      return false;
    }

  if(!inputLPC.is_open())
    {
      std::cerr << "** ERROR: Can't open LPC Dataset for input" << std::endl;
      return false;
    }

  for(std::string line; getline(inputUCSB, line);)
    {
      if (line[0] == '#') continue;
      std::cout << "Add File: " << line << std::endl;
      UCSBChain->Add(line.c_str());
    }
  for(std::string line; getline(inputLPC, line);)
    {
      if (line[0] == '#') continue;
      std::cout << "Add File: " << line << std::endl;
      LPCChain->Add(line.c_str());
    }

  //std::cout << "No. of Entries in this tree : " << fChain->GetEntries() << std::endl;
                                                       
  NTupleReader trUCSB(UCSBChain);
  NTupleReader trLPC(LPCChain);
  trUCSB.setReThrow(false);
  trLPC.setReThrow(false);
  StopleAlias alias;
  trUCSB.registerFunction(alias);
  std::cout << "total entries for UCSB: " << trUCSB.getNEntries() << std::endl;
  std::cout << "total entries for LPC: " << trLPC.getNEntries() << std::endl;
  if(trLPC.getNEntries()!=trUCSB.getNEntries())
    {
      std::cout << "This code was made to run over Ntuples that have the same number of entries." << std::endl;
      std::cout << "These two Ntuples do not have the same number of entries" << std::endl;
      return -1;
    }

  using namespace std;

  //create files
  ofstream outputFileLPCPt;
  outputFileLPCPt.open("LPCvetoElectronIDPt.csv");
  ofstream outputFileLPCEta;
  outputFileLPCEta.open("LPCvetoElectronIDEta.csv");
  ofstream outputFileLPCPhi;
  outputFileLPCPhi.open("LPCvetoElectronIDPhi.csv");
  ofstream outputFileUCSBPt;
  outputFileUCSBPt.open("UCSBvetoElectronIDPt.csv");
  ofstream  outputFileUCSBEta;
  outputFileUCSBEta.open("UCSBvetoElectronIDEta.csv");
  ofstream outputFileUCSBPhi;
  outputFileUCSBPhi.open("UCSBvetoElectronIDPhi.csv");
  int count = 0;
  outputFileUCSBPt << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<< endl;
  outputFileUCSBEta << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<< endl;
  outputFileUCSBPhi << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<< endl;
  outputFileLPCPt << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<< endl;
  outputFileLPCEta << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<<endl;
  outputFileLPCPhi << "Event,Column 1,Column 2,Column 3,Column 4,Column 5"<<endl;

  //loop through all events
  while(trLPC.getNextEvent())
    {
      //get LPC event number to find the same UCSB event number
      int eventNumber = trLPC.getVar<int>("event");
      bool sameEvent = false;

      //Find the UCSB event that corresponds to the LPC event
      //Print UCSB event information to respective files
      for(int i = 0; sameEvent!=true;i++){
	trUCSB.goToEvent(i);
	if(trLPC.getVar<int>("event")==trUCSB.getVar<int>("event"))
	  {
	    int commaCountUCSB=0;
	    const std::vector<int>& vetoElectronIDVecUCSB = trUCSB.getVec<int>("vetoElectronID");
	    const std::vector<TLorentzVector>& elesLVecUCSB  = trUCSB.getVec<TLorentzVector>("elesLVec");
	    outputFileUCSBPt << "Event "<< eventNumber;
	    outputFileUCSBEta << "Event "<< eventNumber;
	    outputFileUCSBPhi << "Event "<< eventNumber;
	    //loops over to find vetoElectronID Pt, Phi, and Eta
	    for(int i = 0; i < vetoElectronIDVecUCSB.size(); i++){
	      if(vetoElectronIDVecUCSB[i] == 1){
		outputFileUCSBPt << "," << elesLVecUCSB[i].Pt();
		outputFileUCSBEta << "," << elesLVecUCSB[i].Eta();
		outputFileUCSBPhi << "," << elesLVecUCSB[i].Phi();
		commaCountUCSB++;
	      }
	    }
	    while(commaCountUCSB!=5){
	      outputFileUCSBPt << " ,";
	      outputFileUCSBEta << " ,";
	      outputFileUCSBPhi << " ,";
	      commaCountUCSB++;
	    }
	    outputFileUCSBPt << endl;
	    outputFileUCSBEta << endl;
	    outputFileUCSBPhi << endl;
	    sameEvent = true;
	  }
      }
      const std::vector<int>& vetoElectronIDVecLPC = trLPC.getVec<int>("vetoElectronID");      
      const std::vector<TLorentzVector>& elesLVecLPC  = trLPC.getVec<TLorentzVector>("elesLVec");
      int commaCountLPC=0;
      outputFileLPCPt << "Event "<< eventNumber;
      outputFileLPCEta << "Event "<< eventNumber;
      outputFileLPCPhi << "Event "<< eventNumber;
      //loops over to find vetoElectronID Pt, Phi, and Eta                                                                               
      for(int i = 0; i < vetoElectronIDVecLPC.size(); i++){
	if(vetoElectronIDVecLPC[i] == 1){
	  outputFileLPCPt << "," << elesLVecLPC[i].Pt();
	  outputFileLPCEta << "," << elesLVecLPC[i].Eta();
	  outputFileLPCPhi << "," << elesLVecLPC[i].Phi();
	  commaCountLPC++;
	}
      }
      while(commaCountLPC!=5){
	outputFileLPCPt << ",";
	outputFileLPCEta << ",";
	outputFileLPCPhi << ",";
	commaCountLPC++;
      }
      outputFileLPCPt << endl;
      outputFileLPCEta << endl;
      outputFileLPCPhi << endl;

      //Keeps track of count and prints onto screen for every 100 events processed
      count++;
      if(count%100==0)
	std::cout<<count<<" done"<<std::endl;
    }
  //Closes all output files
  outputFileLPCPt.close();
  outputFileLPCEta.close();
  outputFileLPCPhi.close();
  outputFileUCSBPt.close();
  outputFileUCSBEta.close();
  outputFileUCSBPhi.close();
  return 0;
}
