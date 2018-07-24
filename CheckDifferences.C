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
  outputFileLPCPt.open("LPCmetPt.csv");
  //ofstream outputFileLPCEta;
  //outputFileLPCEta.open("LPCDiffmediumPhotonIDEta.csv");
  ofstream outputFileLPCPhi;
  outputFileLPCPhi.open("LPCmetPhi.csv");
  ofstream outputFileUCSBPt;
  outputFileUCSBPt.open("UCSBmetPt.csv");
  //ofstream  outputFileUCSBEta;
  //outputFileUCSBEta.open("UCSBDiffmediumPhotonIDEta.csv");
  ofstream outputFileUCSBPhi;
  outputFileUCSBPhi.open("UCSBmetPhi.csv");
  int count = 0;
  
  outputFileLPCPt<<"Event #,Value only in LPC"<<endl;
  //outputFileLPCEta<<"Event #,Value only in LPC"<<endl;
  outputFileLPCPhi<<"Event #,Value only in LPC"<<endl;
  outputFileUCSBPt<<"Event #,Value only in UCSB"<<endl;
  //outputFileUCSBEta<<"Event #,Value only in UCSB"<<endl;
  outputFileUCSBPhi<<"Event #,Value only in UCSB"<<endl;
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
	    if(trUCSB.getVar<double>("met")!=trLPC.getVar<double>("met")){
	      outputFileLPCPt << "Event "<< eventNumber<<","<< trLPC.getVar<double>("met") <<endl;
	      outputFileUCSBPt << "Event "<< eventNumber<<","<< trUCSB.getVar<double>("met") <<endl;
	      
	    }
	    if(trUCSB.getVar<double>("metphi")!=trLPC.getVar<double>("metphi")){
	      outputFileLPCPhi << "Event "<< eventNumber<<","<< trLPC.getVar<double>("metphi") <<endl;
	      outputFileUCSBPhi << "Event "<< eventNumber<<","<< trUCSB.getVar<double>("metphi") <<endl;
            }

	    /*
	    const std::vector<int>& mediumPhotonIDVecUCSB = trUCSB.getVec<int>("mediumPhotonID");
	    const std::vector<TLorentzVector>& gammaLVecUCSB  = trUCSB.getVec<TLorentzVector>("gammaLVec");
	    const std::vector<int>& mediumPhotonIDVecLPC = trLPC.getVec<int>("mediumPhotonID");      
	    const std::vector<TLorentzVector>& gammaLVecLPC  = trLPC.getVec<TLorentzVector>("gammaLVec");
	    for(int y = 0; y < mediumPhotonIDVecUCSB.size(); y++){
	      if(mediumPhotonIDVecUCSB[y] == 1){
		bool sameExistsPt = false;
		bool sameExistsEta = false;
		bool sameExistsPhi = false;
		for(int z = 0; z < mediumPhotonIDVecLPC.size(); z++){
		  if(mediumPhotonIDVecLPC[z]==1){
		    if(gammaLVecLPC[z].Pt()==gammaLVecUCSB[y].Pt()){
		      sameExistsPt = true;
		    }
		    if(gammaLVecLPC[z].Eta()==gammaLVecUCSB[y].Eta()){
		      sameExistsEta = true;
		    }
		    if(gammaLVecLPC[z].Phi()==gammaLVecUCSB[y].Phi()){
		      sameExistsPhi = true;
		    }
		  }
		}
		if(sameExistsPt==false){
		  outputFileUCSBPt << "Event "<< eventNumber<<","<< gammaLVecUCSB[y].Pt()<<endl;
		}
		if(sameExistsEta==false){
		  outputFileUCSBEta << "Event "<< eventNumber<<","<< gammaLVecUCSB[y].Eta()<<endl;
		}
		if(sameExistsPhi==false){
		  outputFileUCSBPhi << "Event "<< eventNumber<<","<< gammaLVecUCSB[y].Phi()<<endl;
		}
	      }
	    }
	    */
	    /*
	    for(int y = 0; y < mediumPhotonIDVecLPC.size(); y++){
	      if(mediumPhotonIDVecLPC[y] == 1){
		bool sameExistsPt = false;
		bool sameExistsEta = false;
		bool sameExistsPhi = false;
		for(int z = 0; z <mediumPhotonIDVecUCSB.size(); z++){
		  if(mediumPhotonIDVecUCSB[z]==1){
		    if(gammaLVecUCSB[z].Pt()==gammaLVecLPC[y].Pt()){
		      sameExistsPt = true;
		    }
		    if(gammaLVecUCSB[z].Eta()==gammaLVecLPC[y].Eta()){
		      sameExistsEta = true;
		    }
		    if(gammaLVecUCSB[z].Phi()==gammaLVecLPC[y].Phi()){
		      sameExistsPhi = true;
		    }
		  }
		}
		if(sameExistsPt==false){
		  outputFileLPCPt << "Event "<< eventNumber<<","<< gammaLVecLPC[y].Pt()<<endl;
		}
		if(sameExistsEta==false){
		  outputFileLPCEta << "Event "<< eventNumber<<","<< gammaLVecLPC[y].Eta()<<endl;
		}
		if(sameExistsPhi==false){
		  outputFileLPCPhi << "Event "<< eventNumber<<","<< gammaLVecLPC[y].Phi()<<endl;
		}
	      }
	    }
	    sameEvent = true;
	  }
	    */
	    sameEvent = true;
	  }
      //Keeps track of count and prints onto screen for every 100 events processed
      }
      count++;
      if(count%1000==0){
	std::cout<<count<<" done"<<std::endl;
      }
    }
  //Closes all output files
  outputFileLPCPt.close();
  //outputFileLPCEta.close();
  outputFileLPCPhi.close();
  outputFileUCSBPt.close();
  //outputFileUCSBEta.close();
  outputFileUCSBPhi.close();
  return 0;
}
