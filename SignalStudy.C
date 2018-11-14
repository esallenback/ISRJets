#include "baselineDef.h"                                                                                                                    
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
#include "TMath.h"
#include "LHAPDF/LHAPDF.h"
#include "TH1.h"
#include "TH2.h"
#include "tdrstyle.h"
 int main(int argc, char* argv[])
{
  //----------------------------------------------------------------------------                                                                //  Getting input information                                                                                                                 //----------------------------------------------------------------------------                                                               
  if (argc < 2)
    {
      std::cerr <<"Please give 2 arguments " << "runList " << std::endl;
      std::cerr <<" Valid configurations are " << std::endl;
      std::cerr <<" ./SignalStudy aList.list" << std::endl;
      return -1;
    }
  //----------------------------------------------------------------------------                                                                //  Input and output files  
  //----------------------------------------------------------------------------                                                               
  const char *inputFileList = argv[1];
  const char *outFileName = argv[2];
   TChain *fChain = NULL;
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
  //NTupleReader trNoCuts(fChain);
  //trNoCuts.setReThrow(false);
   //use for non-fastsim signals  
  BaselineVessel blv(tr);
   //use for fastsim signals   
  
  //BaselineVessel blv(tr,"","fastsim");
   tr.registerFunction(blv);
  TFile OutFile(outFileName,"RECREATE");
  int totalEvents = tr.getNEntries();
  int entries = 0;
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initialize output file ~~~~~                                                            
   TH1* ISRJetPtBG = new TH1D("ISRJetPtBG","ISR Jet Pt with mtb < 175",30, 0,1000);
  TH1* ISRJetPhiBG = new TH1D("ISRJetPhiBG","ISR Jet Phi with mtb < 175",30,-4,4);
  TH1* ISRJetEtaBG = new TH1D("ISRJetEtaBG","ISR Jet Eta with mtb <175",30,-4,4);
  TH1* ISRJetnTops = new TH1D("ISRJetnTops", "Number of Top Jets for Events with ISR Jets Above 200 GeV", 20, 0, 5);
  TH1* ISRJetak8Jets = new TH1D("ISRJetak8Jets", "Number of ak8 Jets Above 100 GeV for Events with ISR Jets Above 200 GeV", 20, 0, 5);
   TH1* ISRJetDeltaPhiOneTop = new TH1D("ISRJetDeltaPhiOneTop","ISR Jet Delta Phi with One Top Jet, mtb < 175, and ak8 Jet > 100",30, -7,7);
  TH1* ISRJetDeltaROneTop = new TH1D("ISRJetDeltaROneTop","ISR Jet Delta R with One Top Jet, mtb < 175, and ak8 Jet > 100",20,0,10);
  TH1* ISRJetDeltaEtaOneTop = new TH1D("ISRJetDeltaEtaOneTop","ISR Jet Delta eta with One Top Jet, mtb < 175, and ak8 Jet > 100",30,-7,7);
  TH1* ISRJetak8JetDeltaR = new TH1D("ISRJetak8JetDeltaR","ISR Jet and ak8 Jet Delta R (ISR > 200 and ak8 > 100)",20,0,10);
  TH1* ISRJetDeltaPhiMET = new TH1D("ISRJetDeltaPhiOneMET","ISR Jet Delta Phi with MET, mtb < 175, and ak8 Jet > 100",30, -7,7);
  
  TH1* efficiencyBaseline = new TH1D("efficiencyBaseline","Efficiency",2,0,2);
  TH1* efficiencyBaseline_mtb175 = new TH1D("efficiencyBaseline_mtb175","Efficiency of baseline and mtb<175 cuts",2,0,2);
  TH1* efficiencyBaseline_mtb175_pt200 = new TH1D("efficiencyBaseline_mtb175_pt200","Efficiency of baseline and mtb<175 cuts and pt>200",2,0,2);
  TH1* efficiencyBaseline_mtb175_t1 = new TH1D("efficiencyBaseline_mtb175_t1","Efficiency of baseline, mtb<175 and nTops=1 Cuts",2,0,2);
  TH1* efficiencyBaseline_mtb175_100ak8 = new TH1D("efficiencyBaseline_mtb175_100","Efficiency of baseline, mtb<175 and ak8>100 Cuts",2,0,2);
  TH1* efficiencyBaseline_mtb175_ak8Count = new TH1D("efficiencyBaseline_mtb175_ak8Count","Efficiency of Baseline and mtb<175 Cuts on ak8 Count",2,0,2);
   TH1* efficiencyBaseline_mtb175_nTopsCount = new TH1D("efficiencyBaseline_mtb175_nTopsCount","Efficiency of Baseline and mtb<175 Cuts on nTop Count",2,0,2);
  
  while(tr.getNextEvent())
    {
      
      double wgt = tr.getVar<double>("evtWeight");
      const std::vector<TLorentzVector>& ISRJetVec = tr.getVec<TLorentzVector>("vISRJet");
      const std::vector<TLorentzVector>& TopsVec = tr.getVec<TLorentzVector>("vTops");
      const std::vector<TLorentzVector>& ak8JetsLVec = tr.getVec<TLorentzVector>("ak8JetsLVec");
       //Fill Efficiency with no cuts
      efficiencyBaseline_mtb175_pt200->Fill(0.0,wgt);
      efficiencyBaseline->Fill(0.0,wgt);
      efficiencyBaseline_mtb175->Fill(0.0,wgt);
      efficiencyBaseline_mtb175_t1->Fill(0.0,wgt);
      efficiencyBaseline_mtb175_ak8Count->Fill(0.0,wgt);
      efficiencyBaseline_mtb175_100ak8->Fill(0.0,wgt);
      efficiencyBaseline_mtb175_nTopsCount->Fill(0.0,wgt);
      //if(tr.getEvtNum() > 10000) break;
      bool passBaseline = tr.getVar<bool>("passBaseline");
      //Finding mtb
      std::vector<TLorentzVector> jlv = tr.getVec<TLorentzVector>("j\
etsLVec");
      int jlv_size = jlv.size();
      int jpt20cnt = 0;
      double mtb1 = -10;
      double mtb2 = -10;
      int bjetcnt = 0;
      double mtb = -10;
      double met = tr.getVar<double>("met");
      double metphi = tr.getVar<double>("metphi");
      std::vector<TLorentzVector> bjetsLVec;
      //std::vector<double> rjbt_vec = tr.getVec<double>("recoJetsBtag_0");
       for(int index=0; (index < jlv_size); index++)
        {
	  TLorentzVector tlv = jlv[index];
	  double jpt = tlv.Pt();
	  double jeta = tlv.Eta();
	  //double rjbt = rjbt_vec[index];
	  if (jpt >= 20.0 && fabs(jeta)<= 2.4)
	    {
	      jpt20cnt++;
	    }
 	  //veto non-bottom jets                                 
	  /*
	  if (rjbt < 0.8484)
	    {
	      continue;
	    }
	  */
	  bjetsLVec.push_back(tlv);
	  double bphi = tlv.Phi();
	  double bpt = tlv.Pt();
	  double dPhi = bphi - metphi;
	  double MT = sqrt(2*bpt*met*(1-cos(dPhi)));
	  bjetcnt++;
	  //leading b jet                                        
	  if (bjetcnt == 1){
	      mtb1 = MT;
	    }
	  if (bjetcnt == 2){
	      mtb2 = MT;
	    }
	}
      if (bjetsLVec.size() == 0){
	  mtb = 0;
      }
      if (bjetsLVec.size() == 1){
	  mtb = mtb1;
      }
      if (bjetsLVec.size() >= 2)
        {
	  if(mtb1 < mtb2){
	    mtb = mtb1;
	  }
	  else{
	    mtb = mtb2;
	  }
	}
      if(passBaseline==true){
	//Fill histogras after baseline cut
	efficiencyBaseline->Fill(1.0,wgt);
      }
      if(passBaseline==false || mtb>175){
	continue;
      }
      else{
	//All ISR jets
	if(ISRJetVec.size()!=0){
	  efficiencyBaseline_mtb175->Fill(1.0,wgt);
	  ISRJetPtBG -> Fill(ISRJetVec[0].Pt(),wgt);
	  ISRJetPhiBG -> Fill(ISRJetVec[0].Phi(),wgt);
	  ISRJetEtaBG -> Fill(ISRJetVec[0].Eta(),wgt);
	}
 	
	if(ISRJetVec.size()!=0 && ISRJetVec[0].Pt()>200){
	  efficiencyBaseline_mtb175_pt200->Fill(1.0,wgt);
	  ISRJetDeltaPhiMET->Fill(calcDPhi(ISRJetVec[0].Phi(),met);
	  
	  
	  //Number of Top Jets for each event with an ISR Pt above 200 (1.0)
	  for(int i=0;i<TopsVec.size();i++){
	    if(i==0) efficiencyBaseline_mtb175_nTopsCount->Fill(1.0,wgt);
	    ISRJetnTops -> Fill(TopsVec.size(),wgt);
	  }
	  entries++;
	  int count=0;
	  int ak8count=0;
	  for(int i=0; i<ak8JetsLVec.size(); i++){
	    if(i==0) efficiencyBaseline_mtb175_ak8Count->Fill(1.0,wgt);
	    if(ak8JetsLVec[i].Pt()>100){
	      //Delta R Value of ak8>100 and ISR>200 (2.5)
	      ISRJetak8JetDeltaR -> Fill(ISRJetVec[0].DeltaR(ak8JetsLVec[i]),wgt);
	      if(ak8count==0){
		efficiencyBaseline_mtb175_100ak8->Fill(1.0,wgt);
		ak8count++;
	      }
	      count++;
	    }
	  }
	  //Number of ak8 Jets corresponding to each ISR Jet with a Pt above 200 (1.0)
	  ISRJetak8Jets -> Fill(count,wgt);
	  
	  //Delta Phi, Pt, & Eta values of Top Jet and ISR Jet over 200 GeV when there 
	  //is only one Top Jet corresponding to the ISR Jet (2.0)
	  if(TopsVec.size()==1){
	    efficiencyBaseline_mtb175_t1->Fill(1.0,wgt);
	    ISRJetDeltaROneTop -> Fill(ISRJetVec[0].DeltaR(TopsVec[0]),wgt);
	    ISRJetDeltaPhiOneTop -> Fill(ISRJetVec[0].DeltaPhi(TopsVec[0]),wgt);
	    ISRJetDeltaEtaOneTop -> Fill(ISRJetVec[0].Eta()-TopsVec[0].Eta(),wgt);
	  }
	}
      }  
    }
   OutFile.cd();
  efficiencyBaseline->Write("", TObject::kOverwrite);
  efficiencyBaseline->GetYaxis()->SetTitle("Number of Events");
  efficiencyBaseline->GetXaxis()->SetTitle("Total Events = 0; Events Left After Baseline Cut = 1");
   OutFile.mkdir("ISRJet200_mtb175_ak8Count");
  OutFile.cd("ISRJet200_mtb175_ak8Count");
  efficiencyBaseline_mtb175_ak8Count->Write("", TObject::kOverwrite);
  efficiencyBaseline_mtb175_ak8Count->GetYaxis()->SetTitle("Number of Events");
  //efficiecnyBaseline_mtb175_ak8Count->GetXaxis()->SetTitle("Total Events = 0; Events Left After Cuts = 1");
  ISRJetak8Jets->GetYaxis()->SetTitle("Number of Events");
  ISRJetak8Jets->GetXaxis()->SetTitle("Number of ak8 Jets for Each Event with ISR > 200 GeV");
  ISRJetak8Jets->Write("", TObject::kOverwrite);
  
  OutFile.cd("../");
  OutFile.mkdir("ISRJets200_mtb175_nTopCount");
  OutFile.cd("ISRJets200_mtb175_nTopCount");
  efficiencyBaseline_mtb175_nTopsCount->Write("", TObject::kOverwrite);
  efficiencyBaseline_mtb175_nTopsCount->GetYaxis()->SetTitle("Number of Events");
  //efficiecnyBaseline_mtb175_nTopsCount->GetXaxis()->SetTitle("Total Events = 0;  Events Left After Cuts = 1");
  ISRJetnTops->GetYaxis()->SetTitle("Number of Events");
  ISRJetnTops->GetXaxis()->SetTitle("Number of nTops for Each Event with ISR > 200 GeV");
  ISRJetnTops->Write("", TObject::kOverwrite);
  
  OutFile.cd("../");
  OutFile.mkdir("ISRJets_mtb175");
  OutFile.cd("ISRJets_mtb175");
  efficiencyBaseline_mtb175->Write("", TObject::kOverwrite);
  efficiencyBaseline_mtb175->GetYaxis()->SetTitle("Number of Events");
  efficiencyBaseline_mtb175->GetXaxis()->SetTitle("Total Events = 0; Events Left After Cuts = 1");
  ISRJetPtBG->Write("", TObject::kOverwrite);
  ISRJetPtBG->GetYaxis()->SetTitle("Pt");
  ISRJetPtBG->GetXaxis()->SetTitle("Number of Events");
  ISRJetPhiBG->Write("", TObject::kOverwrite);
  ISRJetPhiBG->GetYaxis()->SetTitle("Phi");
  ISRJetPhiBG->GetXaxis()->SetTitle("Number of Events");
  ISRJetEtaBG->GetYaxis()->SetTitle("Number of Events");
  ISRJetEtaBG->GetXaxis()->SetTitle("Eta");
  ISRJetEtaBG->Write("", TObject::kOverwrite);
  
  OutFile.cd("../");
  OutFile.mkdir("ISRJets200_mtb175_nTops=1");
  OutFile.cd("ISRJets200_mtb175_nTops=1");
  efficiencyBaseline_mtb175_t1->Write("",TObject::kOverwrite);
  efficiencyBaseline_mtb175_t1->GetYaxis()->SetTitle("Number of Events");
  efficiencyBaseline_mtb175_t1->GetXaxis()->SetTitle("Total Events = 0; Events Left After Cuts = 1");
  ISRJetDeltaROneTop->Write("",TObject::kOverwrite);
  ISRJetDeltaROneTop->GetYaxis()->SetTitle("Delta R");
  ISRJetDeltaROneTop->GetXaxis()->SetTitle("Number of Events");
  ISRJetDeltaPhiOneTop->Write("",TObject::kOverwrite);
  ISRJetDeltaPhiOneTop->GetYaxis()->SetTitle("Delta Phi");
  ISRJetDeltaPhiOneTop->GetXaxis()->SetTitle("Number of Events");
  ISRJetDeltaEtaOneTop->Write("",TObject::kOverwrite);
  ISRJetDeltaEtaOneTop->GetYaxis()->SetTitle("Delta Eta");
  ISRJetDeltaEtaOneTop->GetXaxis()->SetTitle("Number of Events");
   OutFile.cd("../");
  OutFile.mkdir("ISRJets200_mtb175_ak8100");
  OutFile.cd("ISRJets200_mtb175_ak8100");
  ISRJetak8JetDeltaR->Write("",TObject::kOverwrite);
  efficiencyBaseline_mtb175_100ak8->Write("",TObject::kOverwrite);
  efficiencyBaseline_mtb175_100ak8->GetYaxis()->SetTitle("Number of Events");
  efficiencyBaseline_mtb175_100ak8->GetXaxis()->SetTitle("Total Events = 0; Events Left After Cuts = 1");
  
  ISRJetDeltaPhiMET->Write("",TObject::kOverwrite);
  efficiencyBaseline_mtb175_pt200->Write("",TObject::kOverwrite);
  
  OutFile.cd("../");
  OutFile.Close();
}
