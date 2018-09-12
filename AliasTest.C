//#include "baselineDef.h"
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
  
  //BaselineVessel blv(tr);
  //tr.registerFunction(blv);
  std::cout << "total entries" << tr.getNEntries() << std::endl;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initialize output file ~~~~~
  std::shared_ptr<TFile> OutFile(new TFile(outFileName, "RECREATE"));
    TH1* totalevents = new TH1D("TotalEvents", "Total Events", 2, 0, 10);
    TH1* loosePhotonIDPt = new TH1D("loosePhotonIDPt", "LoosePhotonIDPt", 30, 0, 500);
    TH1* loosePhotonIDPhi = new TH1D("loosePhotonIDPhi", "LoosePhotonIDPhi", 30, -3, 3);
    TH1* loosePhotonIDEta = new TH1D("loosePhotonIDEta", "LoosePhotonIDEta", 30, -3, 3);
    TH1* mediumPhotonIDPt = new TH1D("mediumPhotonIDPt", "MediumPhotonIDPt", 30, 0, 500);
    TH1* mediumPhotonIDPhi = new TH1D("mediumPhotonIDPhi", "MediumPhotonIDPhi", 30, -3, 3);
    TH1* mediumPhotonIDEta = new TH1D("mediumPhotonIDEta", "MediumPhotonIDEta", 30, -3, 3);
    TH1* tightPhotonIDPt = new TH1D("tightPhotonIDPt", "TightPhotonIDPt", 30, 0, 500);
    TH1* tightPhotonIDPhi = new TH1D("tightPhotonIDPhi", "TightPhotonIDPhi", 30, -3, 3);
    TH1* tightPhotonIDEta = new TH1D("tightPhotonIDEta", "TightPhotonIDEta", 30, -3, 3);
    TH1* muonsCharge = new TH1D("muonsCharge","MuonCharge", 30, -50, 50);
    TH1* muonsMiniIso = new TH1D("muonsMiniIso", "MuonMiniIso", 30, 0, 500);
    //TH1* muonsFlagLoosePt = new TH1D("muonsFlagLoosePt","MuonFlagLoosePt", 30, 0, 500);
    //TH1* muonsFlagLooseEta = new TH1D("muonsFlagLooseEta","MuonFlagLooseEta", 30, -3, 3);
    //TH1* muonsFlagLoosePhi = new TH1D("muonsFlagLoosePhi","MuonFlagLoosePhi", 30, -TMath::Pi(), TMath::Pi());
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
    //TH1* elesFlagVetoPt = new TH1D("elesFlagVetoPt","ElectronFlagVetoPt", 30, 0, 500);
    //TH1* elesFlagVetoEta = new TH1D("elesFlagVetoEta","ElectronFlagVetoEta", 30, -2.5, 2.5);
    //TH1* elesFlagVetoPhi = new TH1D("elesFlagVetoPhi","ElectronFlagVetoPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* vetoElectronIDPt = new TH1D("vetoElectronIDPt", "vetoElectronIDPt", 30, 0, 500);
    TH1* vetoElectronIDEta = new TH1D("vetoElectronIDEta","vetoElectronIDEta", 30, -2.5, 2.5);
    TH1* vetoElectronIDPhi = new TH1D("vetoElectronIDPhi","vetoElectronIDPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* elesFlagMediumPt = new TH1D("elesFlagMediumPt", "ElectronFlagMediumPt", 30, 0, 500);
    TH1* elesFlagMediumEta = new TH1D("elesFlagMediumEta", "ElectronFlagMediumEta", 30, -3, 3);
    TH1* elesFlagMediumPhi = new TH1D("elesFlagMediumPhi", "EletronFlagMediumPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* elesPt = new TH1D("elesPt", "ElectronPt", 30, 0, 1000);
    TH1* elesEta = new TH1D("elesEta", "ElectronEta", 30, -3, 3);
    TH1* elesPhi = new TH1D("elesPhi", "ElectronPhi", 30, -TMath::Pi(), TMath::Pi());
    TH1* jetPt = new TH1D("jetPt","Jet pt",30, 0,1000);
    TH1* jetPhi = new TH1D("jetPhi","Jet phi",30,-5,5);
    TH1* jetEta = new TH1D("jetEta","Jet eta",30,-3,3);

    TH1* miniIsoDiff = new TH1D("miniIsoDiff","Mini Iso Diff",30,-5,5);
    //TH1* ak8JetPt = new TH1D("ak8JetPt","ak8 Jet pt",30, 0,1000);
    //TH1* ak8JetPhi = new TH1D("ak8JetPhi","ak8 Jet phi",30,-5,5);
    //TH1* ak8JetEta = new TH1D("ak8JetEta","ak8 Jet eta",30,-3,3);

    TH1* recoJetsBtag_0Pt = new TH1D("recoJetsBtag_0Pt","B-tagged Jet Pt",30, 0,1000);
    TH1* recoJetsBtag_0Eta = new TH1D("recoJetsBtag_0Eta","B-Tagged Jet Eta",30,-3,3);
    TH1* recoJetsBtag_0Phi = new TH1D("recoJetsBtag_0Phi","B-tagged Jet Phi",30,-5,5);
    TH1* leadingJetPt = new TH1D("leadingJetPt","Leading Jet pt",30,0,1000);
    TH1* leadingJetPhi = new TH1D("leadingJetPhi","Leading Jet phi",30,-5,5);
    TH1* leadingJetEta = new TH1D("leadingJetEta","Leading Jet eta",30,-3,3);
    TH1* secondLeadingJetPt = new TH1D("secondLeadingJetPt","Second Leading Jet pt",30,0,1000);
    TH1* secondLeadingJetPhi = new TH1D("secondLeadingJetPhi","Second Leading Jet phi",30,-5,5);
    TH1* secondLeadingJetEta = new TH1D("secondLeadingJetEta","Second Leading Jet eta",30,-3,3);
    TH1* thirdLeadingJetPhi = new TH1D("thirdLeadingJetPhi","Third Leading Jet phi",30,-5,5);
    TH1* thirdLeadingJetPt = new TH1D("thirdLeadingJetPt","Third Leading Jet pt",30,0,1000);
    TH1* thirdLeadingJetEta = new TH1D("thirdLeadingJetEta","Third Leading Jet eta",30,-3,3);
    TH1* fourthLeadingJetPhi = new TH1D("fourthLeadingJetPhi","Fourth Leading Jet phi",30,-5,5);
    TH1* fourthLeadingJetPt = new TH1D("fourthLeadingJetPt","Fourth Leading Jet pt",30,0,1000);
    TH1* fourthLeadingJetEta = new TH1D("fourthLeadingJetEta","Fourth Leading Jet eta",30,-3,3);
    TH1* metPt = new TH1D("metPt","MET pt",30,0,1000);
    TH1* metPhi = new TH1D("metPhi","MET phi",30,-5,5);
    TH1* isotracksPt = new TH1D("isotracksPt","Isotracks Pt", 30, 0, 500);
    TH1* isotracksEta = new TH1D("isotracksEta","Isotracks Eta", 30, -3, 3);
    TH1* isotracksPhi = new TH1D("isotracksPhi","Isotracks Phi", 30, -TMath::Pi(), TMath::Pi());
    int elesCount = 0;
    
    int leadingJets = 0;
    int secondLeadingJets = 0;
    int thirdLeadingJets = 0;
    int fourthLeadingJets =0;
    int totalJets =0;

    int countPt5 = 0;
    int countPt10 = 0;
    int countPt15 = 0;
    int countPt20 = 0;
    int countPt25 = 0;
    int countPt30 = 0;
    int countPt35 = 0;
    int count =0;
    int different=0;
    int countIso=0;
    int countM=0;
    std::cout<<"debug"<<std::endl;
  while(tr.getNextEvent())
  {
    countIso=0;
    //if(tr.getEvtNum() % 10000 == 0) std::cout << "Event #: " << tr.getEvtNum() << std::endl;
    //if(tr.getEvtNum() > 10000) break;
    const std::vector<TLorentzVector>& muonsLVec  = tr.getVec<TLorentzVector>("muonsLVec");
    const std::vector<double>& muonsChargeVec = tr.getVec<double>("muonsCharge");
    const std::vector<double>& muonsMiniIsoVec = tr.getVec<double>("muonsMiniIso");
    const std::vector<int>& muonsFlagTightVec = tr.getVec<int>("muonsFlagTight");
    //const std::vector<int>& muonsFlagLooseVec = tr.getVec<int>("muonsFlagLoose");
    //std::cout<<"debug"<<std::endl;
    const std::vector<int>& muonsFlagMediumVec = tr.getVec<int>("muonsFlagMedium");
    const std::vector<TLorentzVector>& elesLVec  = tr.getVec<TLorentzVector>("elesLVec");

    const std::vector<double>& elesChargeVec = tr.getVec<double>("elesCharge");
    const std::vector<double>& elesMiniIsoVec = tr.getVec<double>("elesMiniIso");
    //const std::vector<int>& elesFlagVetoVec = tr.getVec<int>("elesFlagVeto");
    const std::vector<int>& vetoElectronIDVec = tr.getVec<int>("vetoElectronID");
    const std::vector<double>& recoJetsBtag_0Vec = tr.getVec<double>("recoJetsBtag_0");
    const std::vector<double>& elesFlagMediumVec = tr.getVec<double>("elesFlagMedium");
    const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");
    const std::vector<TLorentzVector>& gammaLVec = tr.getVec<TLorentzVector>("gammaLVec");
    const std::vector<int>& loosePhotonIDVec = tr.getVec<int>("loosePhotonID");
    const std::vector<int>& mediumPhotonIDVec = tr.getVec<int>("mediumPhotonID");
    const std::vector<int>& tightPhotonIDVec = tr.getVec<int>("tightPhotonID");
    //const std::vector<TLorentzVector>& ak8JetsLVec = tr.getVec<TLorentzVector>("ak8JetsLVec");
    const std::vector<TLorentzVector>& isotracksLVec = tr. getVec<TLorentzVector>("loose_isoTrksLVec");
    
    if(ucsb_tuples){
	totalevents -> Fill(7);
    }
    else{
	totalevents -> Fill(2);
    }
    
    leadingJetPt -> Fill(jetsLVec[0].Pt());
    leadingJetPhi -> Fill(jetsLVec[0].Phi());
    leadingJetEta -> Fill(jetsLVec[0].Eta());
    leadingJets++;
    if(1<jetsLVec.size()){
      secondLeadingJetPt -> Fill(jetsLVec[1].Pt());
      secondLeadingJetPhi -> Fill(jetsLVec[1].Phi());
      secondLeadingJetEta -> Fill(jetsLVec[1].Eta());
      secondLeadingJets++;
    }

    if(2<jetsLVec.size()){
      thirdLeadingJetPt -> Fill(jetsLVec[2].Pt());
      thirdLeadingJetPhi -> Fill(jetsLVec[2].Phi());
      thirdLeadingJetEta -> Fill(jetsLVec[2].Eta());
      thirdLeadingJets++;
    }

    if(3<jetsLVec.size()){
      fourthLeadingJetPt -> Fill(jetsLVec[3].Pt());
      fourthLeadingJetPhi -> Fill(jetsLVec[3].Phi());
      fourthLeadingJetEta -> Fill(jetsLVec[3].Eta());
      fourthLeadingJets++;
    }


    for(int i = 0; i < jetsLVec.size(); i++){
	jetPt -> Fill(jetsLVec[i].Pt());
	jetPhi -> Fill(jetsLVec[i].Phi());
	jetEta -> Fill(jetsLVec[i].Eta());
	if(jetsLVec[i].Pt()>5){
	  countPt5++;
	}
	if(jetsLVec[i].Pt()>10){
          countPt10++;
        }
	if(jetsLVec[i].Pt()>15){
          countPt15++;
        }
	if(jetsLVec[i].Pt()>20){
          countPt20++;
        }
	if(jetsLVec[i].Pt()>25){
          countPt25++;
        }
	if(jetsLVec[i].Pt()>30){
          countPt30++;
        }
	if(jetsLVec[i].Pt()>35){
          countPt35++;
        }
	totalJets++;
    }
    //for(int i = 0; i < ak8JetsLVec.size(); i++){
    //  ak8JetPt -> Fill(ak8JetsLVec[i].Pt());
    //  ak8JetPhi -> Fill(ak8JetsLVec[i].Phi());
    //  ak8JetEta -> Fill(ak8JetsLVec[i].Eta());
    //}
    metPt -> Fill(tr.getVar<double>("met"));
    metPhi -> Fill(tr.getVar<double>("metphi"));
    for(int i = 0; i < muonsLVec.size(); i++){
      if(muonsMiniIsoVec.size()!=muonsLVec.size()){
        countM++;
      }
      if(muonsMiniIsoVec[i]<.2){
    	muonsPt -> Fill(muonsLVec[i].Pt());
        muonsEta -> Fill(muonsLVec[i].Eta());
        muonsPhi -> Fill(muonsLVec[i].Phi());
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
    
    //for(int i = 0; i < muonsFlagLooseVec.size(); i++){
    //  if(muonsFlagLooseVec[i] == 1){
    //	muonsFlagLoosePt -> Fill(muonsLVec[i].Pt());
    //	muonsFlagLooseEta -> Fill(muonsLVec[i].Eta());
    //	muonsFlagLoosePhi -> Fill(muonsLVec[i].Phi());
    //}
    //}

    for(int i = 0; i < loosePhotonIDVec.size(); i++){
      if(loosePhotonIDVec[i] == 1){
	if(gammaLVec[i].Pt()>100){
	  loosePhotonIDPt -> Fill(gammaLVec[i].Pt());
	  loosePhotonIDEta -> Fill(gammaLVec[i].Eta());
	  loosePhotonIDPhi -> Fill(gammaLVec[i].Phi());
	}
      }
    }
    for(int i = 0; i < mediumPhotonIDVec.size(); i++){
      if(mediumPhotonIDVec[i] == 1){
	if(gammaLVec[i].Pt()>100){
	  mediumPhotonIDPt -> Fill(gammaLVec[i].Pt());
	  mediumPhotonIDEta -> Fill(gammaLVec[i].Eta());
	  mediumPhotonIDPhi -> Fill(gammaLVec[i].Phi());
	}
      }
    }
    for(int i = 0; i < tightPhotonIDVec.size(); i++){
      if(tightPhotonIDVec[i] == 1){
	if(gammaLVec[i].Pt()>100){
        tightPhotonIDPt -> Fill(gammaLVec[i].Pt());
        tightPhotonIDEta -> Fill(gammaLVec[i].Eta());
        tightPhotonIDPhi -> Fill(gammaLVec[i].Phi());
	}
      }
    }

    for(int i = 0; i < elesLVec.size(); i++){
      //if(elesLVec[i].Pt()>20){
       elesPt -> Fill(elesLVec[i].Pt());
       elesEta -> Fill(elesLVec[i].Eta());
       elesPhi -> Fill(elesLVec[i].Phi());
	 //if(elesLVec[i].Pt() > 5 && -2.4 < elesLVec[i].Eta() < 2.4){
	   //elesCount++;
		// }
       //}
       if(elesMiniIsoVec[i]<0.1){
	 countIso++;
	 }
    } 
    if(countIso!=vetoElectronIDVec.size()){
      miniIsoDiff -> Fill(vetoElectronIDVec.size()-countIso);
    }
  
    for(int i = 0; i < isotracksLVec.size(); i++){
      isotracksPt -> Fill(isotracksLVec[i].Pt());
      isotracksEta -> Fill(isotracksLVec[i].Eta());
      isotracksPhi -> Fill(isotracksLVec[i].Phi());
    }
   
    for(int i = 0; i < elesChargeVec.size(); i++){
      //if(elesLVec[i].Pt()>20){   
	elesCharge -> Fill(elesChargeVec[i]);
	//}
      }
    for(int i = 0; i < elesMiniIsoVec.size(); i++){
      //if(elesLVec[i].Pt()>20){   
	elesMiniIso -> Fill(elesMiniIsoVec[i]);
	//}
      }
   
    //for(int i = 0; i < elesFlagVetoVec.size(); i++){
    if(ucsb_tuples){
      for(int i = 0; i < vetoElectronIDVec.size(); i++){
	if(vetoElectronIDVec[i] == 1){
	  vetoElectronIDPt -> Fill(elesLVec[i].Pt());
	  vetoElectronIDEta -> Fill(elesLVec[i].Eta());
	  vetoElectronIDPhi -> Fill(elesLVec[i].Phi());
	}                                                           
      }
    }
    else{
      for(int i = 0; i < elesLVec.size(); i++){
	if(elesMiniIsoVec[i]<0.1){
	  vetoElectronIDPt -> Fill(elesLVec[i].Pt());
          vetoElectronIDEta -> Fill(elesLVec[i].Eta());
          vetoElectronIDPhi -> Fill(elesLVec[i].Phi());
	}
      }
    }
    for(int i = 0; i < recoJetsBtag_0Vec.size(); i++){
      //if(recoJetBtagVec[i] == 1){
      const float cutCSVS = 0.8484;
	if(recoJetsBtag_0Vec[i] >= cutCSVS){
	  recoJetsBtag_0Pt -> Fill(jetsLVec[i].Pt());
	  recoJetsBtag_0Eta -> Fill(jetsLVec[i].Eta());
	  recoJetsBtag_0Phi -> Fill(jetsLVec[i].Phi());
	  count++;
	}
	//}
    }


    for(int i = 0; i < elesFlagMediumVec.size(); i++){
      //if(elesLVec[i].Pt()>20){   
	if(elesFlagMediumVec[i] == 1){
	  elesFlagMediumPt -> Fill(elesLVec[i].Pt());
	  elesFlagMediumEta -> Fill(elesLVec[i].Eta());
	  elesFlagMediumPhi -> Fill(elesLVec[i].Phi());
         } 
	//}
    }
  }
  std::cout << countM<<std::endl;
  //std::cout << leadingJets << std::endl;
  //std::cout << secondLeadingJets << std::endl;
  //std::cout << thirdLeadingJets << std::endl;
  //std::cout << fourthLeadingJets << std::endl;
  //std::cout << totalJets << std::endl;
  std::cout << "Jets above 5 GeV" << countPt5 << std::endl;
  std::cout << "Jets above 10 GeV" << countPt10 << std::endl;
  std::cout << "Jets above 15 GeV" << countPt15 << std::endl;
  std::cout << "Jets above 20 GeV" << countPt20 << std::endl;
  std::cout << "Jets above 25 GeV" << countPt25 << std::endl;
  std::cout << "Jets above 30 GeV" << countPt30 << std::endl;
  std::cout << "Jets above 35 GeV" << countPt35 << std::endl;
  std::cout << count << std::endl;
  std::cout << "vetoElectronID and MiniIso different this many times: "<< different << std::endl;
  OutFile->cd();
  muonsPt -> Write("", TObject::kOverwrite);
  muonsEta -> Write("", TObject::kOverwrite);
  muonsPhi -> Write("", TObject::kOverwrite);
  muonsCharge->Write("", TObject::kOverwrite);
  muonsMiniIso->Write("", TObject::kOverwrite);
  muonsFlagTightPt ->Write("", TObject::kOverwrite);
  muonsFlagTightEta ->Write("", TObject::kOverwrite);
  muonsFlagTightPhi ->Write("", TObject::kOverwrite);
  //muonsFlagLoosePt ->Write("", TObject::kOverwrite);
  //muonsFlagLooseEta ->Write("", TObject::kOverwrite);
  //muonsFlagLoosePhi ->Write("", TObject::kOverwrite);
  muonsFlagMediumPt ->Write("", TObject::kOverwrite);
  muonsFlagMediumEta ->Write("", TObject::kOverwrite);
  muonsFlagMediumPhi ->Write("", TObject::kOverwrite);
  elesPt -> Write("", TObject::kOverwrite);
  elesEta -> Write("", TObject::kOverwrite);
  elesPhi -> Write("", TObject::kOverwrite);
  elesCharge->Write("", TObject::kOverwrite);
  elesMiniIso->Write("", TObject::kOverwrite);
  //elesFlagVetoPt ->Write("", TObject::kOverwrite);
  //elesFlagVetoEta ->Write("", TObject::kOverwrite);
  //elesFlagVetoPhi ->Write("", TObject::kOverwrite);
  vetoElectronIDPt ->Write("", TObject::kOverwrite);
  vetoElectronIDEta ->Write("", TObject::kOverwrite);
  vetoElectronIDPhi ->Write("", TObject::kOverwrite);
  elesFlagMediumPt ->Write("", TObject::kOverwrite);
  elesFlagMediumEta ->Write("", TObject::kOverwrite);
  elesFlagMediumPhi ->Write("", TObject::kOverwrite);
  leadingJetPt->Write("", TObject::kOverwrite);
  leadingJetPhi->Write("", TObject::kOverwrite);
  leadingJetEta->Write("", TObject::kOverwrite);
  secondLeadingJetPt->Write("", TObject::kOverwrite);
  secondLeadingJetPhi->Write("", TObject::kOverwrite);
  secondLeadingJetEta->Write("", TObject::kOverwrite);
  thirdLeadingJetPt->Write("", TObject::kOverwrite);
  thirdLeadingJetPhi->Write("", TObject::kOverwrite);
  thirdLeadingJetEta->Write("", TObject::kOverwrite);
  fourthLeadingJetPt->Write("", TObject::kOverwrite);
  fourthLeadingJetPhi->Write("", TObject::kOverwrite);
  fourthLeadingJetEta->Write("", TObject::kOverwrite);
  jetPt->Write("", TObject::kOverwrite);
  jetPhi->Write("", TObject::kOverwrite);
  jetEta->Write("", TObject::kOverwrite);
  //ak8JetPt->Write("", TObject::kOverwrite);
  //ak8JetPhi->Write("", TObject::kOverwrite);
  //ak8JetEta->Write("", TObject::kOverwrite);
  recoJetsBtag_0Pt->Write("", TObject::kOverwrite);
  recoJetsBtag_0Phi->Write("", TObject::kOverwrite);
  recoJetsBtag_0Eta->Write("", TObject::kOverwrite);
  metPt->Write("", TObject::kOverwrite);
  metPhi->Write("", TObject::kOverwrite);
  isotracksPt -> Write("", TObject::kOverwrite);
  isotracksEta -> Write("", TObject::kOverwrite);
  isotracksPhi -> Write("", TObject::kOverwrite);
  loosePhotonIDPt -> Write("", TObject::kOverwrite);
  loosePhotonIDPhi -> Write("", TObject::kOverwrite);
  loosePhotonIDEta -> Write("", TObject::kOverwrite);
  mediumPhotonIDPt -> Write("", TObject::kOverwrite);
  mediumPhotonIDPhi -> Write("", TObject::kOverwrite);
  mediumPhotonIDEta -> Write("", TObject::kOverwrite);
  tightPhotonIDPt -> Write("", TObject::kOverwrite);
  tightPhotonIDPhi -> Write("", TObject::kOverwrite);
  tightPhotonIDEta -> Write("", TObject::kOverwrite);

  totalevents -> Write("", TObject::kOverwrite);

  miniIsoDiff -> Write("",TObject::kOverwrite);
  OutFile->Close();
}


