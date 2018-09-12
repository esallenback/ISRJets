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
  //----------------------------------------------------------------------------
  //  Getting input information
  //----------------------------------------------------------------------------
  if (argc < 2)
  {
    std::cerr <<"Please give 2 arguments " << "runList " << " " << "outputFileName" << std::endl;
    std::cerr <<" Valid configurations are " << std::endl;
    std::cerr <<" ./LostLepton_MuCS_TTbar runlist_ttjets.txt isoplots.root" << std::endl;
    return -1;
  }

  //----------------------------------------------------------------------------
  //  Input and output files
  //----------------------------------------------------------------------------
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];

  TChain *fChain = NULL;
  if (strstr(inputFileList, "UCSB") != NULL)
    fChain = new TChain("Events");
  else
    fChain = new TChain("stopTreeMaker/AUX");

  // :WARNING:06/16/2015 11:41:12:benwu: Can't work with passing TChain
  // through a function. Why?
  std::fstream input(inputFileList);
  if(!input.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  for(std::string line; getline(input, line);)
  {
    if (line[0] == '#') continue;
    std::cout << "Add File: " << line << std::endl;
    fChain->Add(line.c_str());
  }
  //std::cout << "No. of Entries in this tree : " << fChain->GetEntries() << std::endl;
  NTupleReader tr(fChain);
  tr.setReThrow(false);
  StopleAlias alias;
  bool ucsb_tuples = false;
  if(!tr.checkBranch("jetsLVec"))
  { 
    tr.registerFunction(alias);
    ucsb_tuples = true;
  }
  std::cout << tr.getNEntries() << std::endl;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initialize output file ~~~~~
  std::shared_ptr<TFile> OutFile(new TFile(outFileName, "RECREATE"));
    TH1* totalevents = new TH1D("TotalEvents", "Total Events", 2, 0, 10);
    TH1* muonsCharge = new TH1D("muonsCharge","MuonCharge", 30, -50, 50);
    TH1* muonsMiniIso = new TH1D("muonsMiniIso", "MuonMiniIso", 30, 0, 500);
    TH1* muonsFlagTightPt = new TH1D("muonsFlagTightPt","MuonFlagTightPt", 30, 0, 500);
    TH1* muonsFlagTightEta = new TH1D("muonsFlagTightEta","MuonFlagTightEta", 30, -3, 3);
    TH1* muonsFlagTightPhi = new TH1D("muonsFlagTightPhi","MuonFlagTightPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* muonsFlagMediumPt = new TH1D("muonsFlagMediumPt", "MuonFlagMediumPt", 30, 0, 500);
    TH1* muonsFlagMediumEta = new TH1D("muonsFlagMediumEta", "MuonFlagMediumEta", 30, -3, 3);
    TH1* muonsFlagMediumPhi = new TH1D("muonsFlagMediumPhi", "EletronFlagMediumPhi", 30, -TMath::Pi(),TMath::Pi());
    TH1* muonsPt = new TH1D("muonsPt", "MuonPt", 30, 0, 500);
    TH1* muonsEta = new TH1D("muonsEta", "MuonEta", 30, -3, 3);
    TH1* muonsPhi = new TH1D("muonsPhi", "MuonPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* elesCharge = new TH1D("elesCharge","ElectronCharge", 30, -50, 50);
    TH1* elesMiniIso = new TH1D("elesMiniIso", "ElectronMiniIso", 30, 0, 500);
    TH1* elesFlagVetoPt = new TH1D("elesFlagVetoPt","ElectronFlagVetoPt", 30, 0, 500);
    TH1* elesFlagVetoEta = new TH1D("elesFlagVetoEta","ElectronFlagVetoEta", 30, -2.5, 2.5);
    TH1* elesFlagVetoPhi = new TH1D("elesFlagVetoPhi","ElectronFlagVetoPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* elesFlagMediumPt = new TH1D("elesFlagMediumPt", "ElectronFlagMediumPt", 30, 0, 500);
    TH1* elesFlagMediumEta = new TH1D("elesFlagMediumEta", "ElectronFlagMediumEta", 30, -3, 3);
    TH1* elesFlagMediumPhi = new TH1D("elesFlagMediumPhi", "EletronFlagMediumPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* elesPt = new TH1D("elesPt", "ElectronPt", 30, 0, 500);
    TH1* elesEta = new TH1D("elesEta", "ElectronEta", 30, -3, 3);
    TH1* elesPhi = new TH1D("elesPhi", "ElectronPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* leading_jet_pt = new TH1D("leading_jet_pt","Leading Jet pt",30,0,1000);
    TH1* leading_jet_phi = new TH1D("leading_jet_phi","Leading Jet phi",30,-5,5);
    TH1* leading_jet_eta = new TH1D("leading_jet_eta","Leading Jet eta",30,-3,3);
    TH1* met_pt = new TH1D("met_pt","MET pt",30,0,1000);
    TH1* met_phi = new TH1D("met_phi","MET phi",30,-5,5);
    TH1* isotracksPt = new TH1D("isotracksPt","Isotracks Pt", 30, 0, 500);
    TH1* isotracksEta = new TH1D("isotracksEta","Isotracks Eta", 30, -3, 3);
    TH1* isotracksPhi = new TH1D("isotrakcsPhi","Isotracks Phi", 30, -TMath::Pi(), TMath::Pi());
    int muonsCount = 0;
    int elesCount = 0;

  while(tr.getNextEvent())
  {
    //if(tr.getEvtNum() % 10000 == 0) std::cout << "Event #: " << tr.getEvtNum() << std::endl;
    //if(tr.getEvtNum() > 10000) break;
    const std::vector<TLorentzVector>& muonsLVec  = tr.getVec<TLorentzVector>("muonsLVec");
    const std::vector<double>& muonsChargeVec = tr.getVec<double>("muonsCharge");
    const std::vector<double>& muonsMiniIsoVec = tr.getVec<double>("muonsMiniIso");
    const std::vector<int>& muonsFlagTightVec = tr.getVec<int>("muonsFlagTight");
    const std::vector<int>& muonsFlagMediumVec = tr.getVec<int>("muonsFlagMedium");
    const std::vector<TLorentzVector>& elesLVec  = tr.getVec<TLorentzVector>("elesLVec");
    const std::vector<double>& elesChargeVec = tr.getVec<double>("elesCharge");
    const std::vector<double>& elesMiniIsoVec = tr.getVec<double>("elesMiniIso");
    const std::vector<int>& elesFlagVetoVec = tr.getVec<int>("elesFlagVeto");
    const std::vector<int>& elesFlagMediumVec = tr.getVec<int>("elesFlagMedium");
    const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");    
    const std::vector<TLorentzVector>& isotracksLVec = tr. getVec<TLorentzVector>("loose_isoTrksLVec");
    if(ucsb_tuples){
	totalevents -> Fill(7);
    }
    else{
	totalevents -> Fill(2);
    }
    for(int i = 0; i < jetsLVec.size(); i++){
	leading_jet_pt -> Fill(jetsLVec[i].Pt());
	leading_jet_phi -> Fill(jetsLVec[i].Phi());
	leading_jet_eta -> Fill(jetsLVec[i].Eta());
    }
    met_pt -> Fill(tr.getVar<double>("met"));
    met_phi -> Fill(tr.getVar<double>("metphi"));
    for(int i = 0; i < muonsLVec.size(); i++){
    	muonsPt -> Fill(muonsLVec[i].Pt());
        muonsEta -> Fill(muonsLVec[i].Eta());
        muonsPhi -> Fill(muonsLVec[i].Phi());
	if(muonsLVec[i].Pt() > 5 && -2.4 < muonsLVec[i].Eta() < 2.4){
		muonsCount++;
	}
    }
    for(int i = 0; i < muonsChargeVec.size(); i++){
        muonsCharge -> Fill(muonsChargeVec[i]);
    }
    for(int i = 0; i < muonsMiniIsoVec.size(); i++){
        muonsMiniIso -> Fill(muonsMiniIsoVec[i]);
    }
    for(int i = 0; i < muonsFlagTightVec.size(); i++){
	if(muonsFlagTightVec[i] == 1){
        	muonsFlagTightPt -> Fill(muonsLVec[i].Pt());
                muonsFlagTightEta -> Fill(muonsLVec[i].Eta());
                muonsFlagTightPhi -> Fill(muonsLVec[i].Phi());
	}
    }
    for(int i = 0; i < muonsFlagMediumVec.size(); i++){
        if(muonsFlagMediumVec[i] == 1){
                muonsFlagMediumPt -> Fill(muonsLVec[i].Pt());
		muonsFlagMediumEta -> Fill(muonsLVec[i].Eta());
                muonsFlagMediumPhi -> Fill(muonsLVec[i].Phi());
         }
    }
    for(int i = 0; i < elesLVec.size(); i++){
         elesPt -> Fill(elesLVec[i].Pt());
         elesEta -> Fill(elesLVec[i].Eta());
         elesPhi -> Fill(elesLVec[i].Phi());
	 if(elesLVec[i].Pt() > 5 && -2.4 < elesLVec[i].Eta() < 2.4){
	 	elesCount++;
	 }
    } 
    for(int i = 0; i < isotracksLVec.size(); i++){
	 isotracksPt -> Fill(isotracksLVec[i].Pt());
	 isotracksEta -> Fill(isotracksLVec[i].Eta());
	 isotracksPhi -> Fill(isotracksLVec[i].Phi());
    }
    for(int i = 0; i < elesChargeVec.size(); i++){
         elesCharge -> Fill(elesChargeVec[i]);
    }
    for(int i = 0; i < elesMiniIsoVec.size(); i++){
         elesMiniIso -> Fill(elesMiniIsoVec[i]);
    }
    for(int i = 0; i < elesFlagVetoVec.size(); i++){
         if(elesFlagVetoVec[i] == 1){
		    elesFlagVetoPt -> Fill(elesLVec[i].Pt());
		    elesFlagVetoEta -> Fill(elesLVec[i].Eta());
                    elesFlagVetoPhi -> Fill(elesLVec[i].Phi());
	 }
    }
    for(int i = 0; i < elesFlagMediumVec.size(); i++){
         if(elesFlagMediumVec[i] == 1){
                elesFlagMediumPt -> Fill(elesLVec[i].Pt());
                elesFlagMediumEta -> Fill(elesLVec[i].Eta());
                elesFlagMediumPhi -> Fill(elesLVec[i].Phi());
         } 
    }
  }
  OutFile->cd();
  muonsPt -> Write("", TObject::kOverwrite);
  muonsEta -> Write("", TObject::kOverwrite);
  muonsPhi -> Write("", TObject::kOverwrite);
  muonsCharge->Write("", TObject::kOverwrite);
  muonsMiniIso->Write("", TObject::kOverwrite);
  muonsFlagTightPt ->Write("", TObject::kOverwrite);
  muonsFlagTightEta ->Write("", TObject::kOverwrite);
  muonsFlagTightPhi ->Write("", TObject::kOverwrite);
  muonsFlagMediumPt ->Write("", TObject::kOverwrite);
  muonsFlagMediumEta ->Write("", TObject::kOverwrite);
  muonsFlagMediumPhi ->Write("", TObject::kOverwrite);
  elesPt -> Write("", TObject::kOverwrite);
  elesEta -> Write("", TObject::kOverwrite);
  elesPhi -> Write("", TObject::kOverwrite);
  elesCharge->Write("", TObject::kOverwrite);
  elesMiniIso->Write("", TObject::kOverwrite);
  elesFlagVetoPt ->Write("", TObject::kOverwrite);
  elesFlagVetoEta ->Write("", TObject::kOverwrite);
  elesFlagVetoPhi ->Write("", TObject::kOverwrite);
  elesFlagMediumPt ->Write("", TObject::kOverwrite);
  elesFlagMediumEta ->Write("", TObject::kOverwrite);
  elesFlagMediumPhi ->Write("", TObject::kOverwrite);
  leading_jet_pt->Write("", TObject::kOverwrite);
  leading_jet_phi->Write("", TObject::kOverwrite);
  leading_jet_eta->Write("", TObject::kOverwrite);
  met_pt->Write("", TObject::kOverwrite);
  met_phi->Write("", TObject::kOverwrite);
  isotracksPt -> Write("", TObject::kOverwrite);
  isotracksEta -> Write("", TObject::kOverwrite);
  isotracksPhi -> Write("", TObject::kOverwrite);
  totalevents -> Write("", TObject::kOverwrite);

  OutFile->Close();
}


