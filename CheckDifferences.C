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
  outputFileLPCPt.open("LPCjetPt.csv");
  ofstream outputFileLPCEta;
  outputFileLPCEta.open("LPCjetEta.csv");
  ofstream outputFileLPCPhi;
  outputFileLPCPhi.open("LPCjetPhi.csv");
  ofstream outputFileUCSBPt;
  outputFileUCSBPt.open("UCSBjetPt.csv");
  ofstream  outputFileUCSBEta;
  outputFileUCSBEta.open("UCSBjetEta.csv");
  ofstream outputFileUCSBPhi;
  outputFileUCSBPhi.open("UCSBjetPhi.csv");
  int count = 0;

  //loop through all events
  while(trLPC.getNextEvent())
    {
      //get LPC event number to find the same UCSB event number
      int eventNumber = trLPC.getVar<int>("event");
      bool sameEvent = false;
      bool different = true;

      //Find the UCSB event that corresponds to the LPC event
      //Print UCSB event information to respective files
      for(int i = 0; sameEvent!=true and different==true;i++){
	trUCSB.goToEvent(i);
	if(trLPC.getVar<int>("event")==trUCSB.getVar<int>("event"))
	  {	
	    const std::vector<TLorentzVector>& jetsLVecUCSB  = trUCSB.getVec<TLorentzVector>("jetsLVec");     
	    const std::vector<TLorentzVector>& jetsLVecLPC  = trLPC.getVec<TLorentzVector>("jetsLVec");
	    for(int y = 0; y < jetsLVecUCSB.size(); y++){
	      bool sameExistsPt = false;
	      bool sameExistsEta = false;
	      bool sameExistsPhi = false;
	      for(int z = 0; z < jetsLVecLPC.size(); z++){
		if(jetsLVecLPC[z].Pt()==jetsLVecUCSB[y].Pt()){
		  sameExistsPt = true;
		}
		if(jetsLVecLPC[z].Eta()==jetsLVecUCSB[y].Eta()){
		  sameExistsEta = true;
		}
		if(jetsLVecLPC[z].Phi()==jetsLVecUCSB[y].Phi()){
		  sameExistsPhi = true;
		}
	      }
	      if(sameExistsPt==false){
		outputFileUCSBPt << "Event "<< eventNumber<<","<< jetsLVecUCSB[y].Pt()<<endl;
	      }
	      if(sameExistsEta==false){
		outputFileUCSBEta << "Event "<< eventNumber<<","<< jetsLVecUCSB[y].Eta()<<endl;
	      }
	      if(sameExistsPhi==false){
		outputFileUCSBPhi << "Event "<< eventNumber<<","<< jetsLVecUCSB[y].Phi()<<endl;
	      }
	    }
	    for(int y = 0; y < jetsLVecLPC.size(); y++){
	      bool sameExistsPt = false;
	      bool sameExistsEta = false;
	      bool sameExistsPhi = false;
	      for(int z = 0; z < jetsLVecUCSB.size(); z++){
		if(jetsLVecUCSB[z].Pt()==jetsLVecLPC[y].Pt()){
		  sameExistsPt = true;
		}
		if(jetsLVecUCSB[z].Eta()==jetsLVecLPC[y].Eta()){
		  sameExistsEta = true;
		}
		if(jetsLVecUCSB[z].Phi()==jetsLVecLPC[y].Phi()){
		  sameExistsPhi = true;
		}
	      }
	      if(sameExistsPt==false){
		outputFileLPCPt << "Event "<< eventNumber<<","<< jetsLVecLPC[y].Pt()<<endl;
	      }
	      if(sameExistsEta==false){
		outputFileLPCEta << "Event "<< eventNumber<<","<< jetsLVecLPC[y].Eta()<<endl;
	      }
	      if(sameExistsPhi==false){
		outputFileLPCPhi << "Event "<< eventNumber<<","<< jetsLVecLPC[y].Phi()<<endl;
	      }
	    }
	    sameEvent = true;
	  }
      }
      //Keeps track of count and prints onto screen for every 100 events processed
      count++;
      if(count%100==0){
	std::cout<<count<<" done"<<std::endl;
      }
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
