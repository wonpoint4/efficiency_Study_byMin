#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>

#include <TH1.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TF1.h>
#include <TTree.h>
#include <TTreeCache.h>
#include <TChain.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TString.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TSystem.h>

#define DEBUG (0)
#include "PlotTools.h"

using namespace std;

void printRunTime(TStopwatch timer_)
{
  Double_t cpuTime = timer_.CpuTime();
  Double_t realTime = timer_.RealTime();

  cout << endl;
  cout << "************************************************" << endl;
  cout << "Total real time: " << realTime << " (seconds)" << endl;
  cout << "Total CPU time:  " << cpuTime << " (seconds)" << endl;
  cout << "  CPU time / real time = " << cpuTime / realTime << endl;
  cout << "************************************************" << endl;
}

void drawtnpCompEffL3filter(
  TString efftag = "myL1sSingleMu22", TString ver = "vRun3_04", TString SAMPLE = "Run2022", TString tag = "Muon",
  TString L1tag = "", TString L1str = "",
  bool isLogy = false  // HERE
) {
  TStopwatch timer_total;
  timer_total.Start();

  gStyle->SetPalette(kRainBow);
  TH1::SetDefaultSumw2(kTRUE);

  TString Dir = "./plots_"+ver+"/"+tag+"/Eff_"+efftag+"/"+L1tag+"/";
  if (gSystem->mkdir(Dir,kTRUE) != -1)
    gSystem->mkdir(Dir,kTRUE);

  vector<TString> v_var = {"pt_zoom", "pt", "eta", "phi", "pu"};
  vector< vector<double> > range = {
    {1, 0, 200},  // pt
    {1, 0, 200},  // pt
    {1, -2.4, 2.4},  // eta
    {1, -TMath::Pi(), TMath::Pi()},
    {1, 10, 75}  // PU
  };
  if (tag == "JPsi" || tag == "Bs") {
    range.at(0) = {1, 0, 40};
    range.at(1) = {1, 0, 40};
  }

  int n_pt_bins = 29-1;
  double pt_bins[29] = {
    0, 1, 2, 3, 4,
    5, 6, 7, 8, 9,
    10, 12, 15, 18, 20,
    21, 22, 23, 26, 30,
    40, 60, 90, 130, 200,
    300, 450, 700, 1000
  };

  int n_eta_bins = 23-1;
  double eta_bins[23] = {
    -2.4, -2.1, -1.9, -1.7, -1.6, -1.5, -1.3, -1.2, -0.9,
    -0.3, -0.2,  0.0,  0.2,  0.3,
     0.9,  1.2, 1.3, 1.5, 1.6, 1.7, 1.9, 2.1,  2.4
  };
  vector<TString> etas_str = {"I"};//, "B", "E"};
  vector<TString> etas_str_long = {"|#eta^{reco}| < 2.4"};//, "|#eta^{reco}| < 1.2", "1.2 < |#eta^{reco}| < 2.4"};

  vector<Color_t> v_color = {
    kBlack,
    kBlue,
    kRed,
    kOrange,
    kGreen+2,
    //kCyan+2,
    //kPink+4,
    //kGray+2,
    //kMagenta,
  };
  vector<int> v_marker = {
    20,
    22,
    26,
    32,
    23,
    //22,
    //26,
    //23,
    //32,
  };
  vector<TString> files = {
    "./Outputs_"+ver+"/hist-"+ver+"-Single"+tag+"_RunUL2018D-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-DYToLL_M50_120X-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-"+tag+"_Run2022BCD_hadd-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-"+tag+"_Run2022EF_hadd-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-"+tag+"_Run2022FG_hadd-Eff.root",
  };
  vector<TString> types = {
    //"Eff/myMu24/num_Eff_L1SQ22_myMu24_RunAll",

    //"Eff/myL1sSingleMu22/num_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/num_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/num_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/num_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/num_Eff_L1SQ22_myL1sSingleMu22_RunAll",

    "Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
  };
  vector<TString> types_den = {
    //"Eff/hltIterL3Muon/num_Eff_L1SQ22_hltIterL3Muon_RunAll",

    //"Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",
    //"Eff/myL1sSingleMu22/den_Eff_L1SQ22_myL1sSingleMu22_RunAll",

    "Eff/myL1sSingleMu22/num_Eff_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/num_Eff_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/num_Eff_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/num_Eff_myL1sSingleMu22_RunAll",
    "Eff/myL1sSingleMu22/num_Eff_myL1sSingleMu22_RunAll",
  };
  vector<TString> types_str = {
    efftag+" : Run2 UL2018 D 5fb^{-1}",
    efftag+" : Run3 DY M-50 Summer21",
    efftag+" : Run3 2022 BCD",
    efftag+" : Run3 2022 EF before v1.5",
    efftag+" : Run3 2022 FG after v1.5",
  };

  vector<TString> v_pts = {
    "genpt0",
    //"genpt10",
    "genpt26"
    //"genpt53",
  };

  vector<TString> v_pts_str = {
    "",
    //"p_{T}^{reco} > 10 GeV",
    "p_{T}^{reco} > 26 GeV"
    //"p_{T}^{reco} > 53 GeV"
  };

  for(unsigned i_eta=0; i_eta<etas_str.size(); i_eta++){
    for(int ipt=0; ipt<(int)v_pts.size(); ++ipt) {
      for(int ivar=0; ivar<(int)v_var.size(); ++ivar) {

        double xmin = range[ivar][1];
        double xmax = range[ivar][2];
        double ymin = 0.0;
        double ymax = 1.6;

	if(!v_var[ivar].Contains("pt") || v_var[ivar] == "pt_zoom") {
	  ymin = 0.6;//0.6;//0.85;
	  ymax = 1.2;//1.2;//1.1;
	}

        TString canvasName = TString::Format("L3filterEff_%s_%s_%s_%s_%s",
                                             efftag.Data(),
                                             L1tag.Data(),
                                             etas_str.at(i_eta).Data(),
                                             v_pts[ipt].Data(),
                                             v_var[ivar].Data());
        canvasName.ReplaceAll(".","p").ReplaceAll("-","_");
        TCanvas *c;
        SetCanvas_Square( c, canvasName, kFALSE, kFALSE, 900, 900 );
        c->cd();
        if(isLogy) c->SetLogy();
        if(tag == "Zprime" && v_var[ivar].Contains("pt")) c->SetLogx();

        TLegend *legend;
        SetLegend( legend, 0.14, 0.71, 0.94, 0.84, -1);

        bool isFirst = true;
        for(int i = 0; i<(int)types.size(); ++i) {
          TString fileName = files.at(i);

          TString the_type_num = types[i];
          TString the_type_den = types_den[i];
          TString the_type_str = types_str[i];

          TString hist_var = v_var[ivar];
          hist_var.ReplaceAll("_zoom", "");

          TString titleX = GetTitleX(hist_var+"_reco");
          TString titleY = "Mu24/L3+ID efficiency";

          TString den_name = TString::Format("%s_%s_%s_%s", the_type_den.Data(), etas_str.at(i_eta).Data(), v_pts[ipt].Data(), hist_var.Data());
          TString num_name = TString::Format("%s_%s_%s_%s", the_type_num.Data(), etas_str.at(i_eta).Data(), v_pts[ipt].Data(), hist_var.Data());

          TH1F* den = Get_Hist( fileName, den_name );
          TH1F* num = Get_Hist( fileName, num_name );

          if(v_var[ivar].Contains("pt")) {
            den = (TH1F*)den->Rebin(n_pt_bins, den_name+"_rb", pt_bins);
            num = (TH1F*)num->Rebin(n_pt_bins, num_name+"_rb", pt_bins);
          }
          else if(v_var[ivar] == "eta") {
            den = (TH1F*)den->Rebin(n_eta_bins, den_name+"_rb", eta_bins);
            num = (TH1F*)num->Rebin(n_eta_bins, num_name+"_rb", eta_bins);
          }
          else{
            den = (TH1F*)den->Rebin(range[ivar][0]);
            num = (TH1F*)num->Rebin(range[ivar][0]);
          }

          int nbins = den->GetNbinsX();

          c->cd();

          TGraphAsymmErrors* g = new TGraphAsymmErrors(nbins);
          //g->Divide(num, den, "n e0");
          g->Divide(num, den, "pois");

          for(int ip=0; ip<nbins; ++ip) {
            if(g->GetPointY(ip) == 0.)  g->SetPointEYhigh(ip, 0.0);
          }

          g->SetTitle("");
          g->SetMarkerSize(1.5);
          g->SetMarkerStyle(v_marker[i]);
          g->SetMarkerColor(v_color[i]);
          g->SetLineColor(  v_color[i]);
          g->SetLineWidth(1);

          g->GetXaxis()->SetLimits( xmin, xmax );
          g->GetXaxis()->SetRangeUser( xmin, xmax );
          g->GetYaxis()->SetRangeUser( ymin, ymax );

          SetAxis_SinglePad( g->GetXaxis(), g->GetYaxis(), titleX, titleY );

          if(isFirst) {
            g->Draw("APE");
            isFirst = false;
          }
          else {
            g->Draw("PE same");
          }

          legend->AddEntry( g, TString::Format("%s", the_type_str.Data()), "lep" );
        }

        legend->Draw();

        TLatex latex;
	Latex_Preliminary_13p6TeV( latex );
        latex.DrawLatexNDC( 0.45,0.96, "#scale[0.8]{#font[42]{"+SAMPLE+"}}");
        latex.DrawLatexNDC(0.20, 0.87, "#font[42]{#scale[0.8]{"+L1str+"}}");
        latex.DrawLatexNDC((i_eta==2?0.66:0.70), 0.89, "#font[42]{#scale[0.8]{"+etas_str_long.at(i_eta)+"}}");
        if(v_var[ivar] != "pt" ) latex.DrawLatexNDC(0.70, 0.84, "#font[42]{#scale[0.8]{"+v_pts_str.at(ipt)+"}}");

        TString logy_tag = isLogy ? "_log" : "";
        // CMS_lumi(c, 98, 11);
        c->Modified();  c->Update();  c->RedrawAxis();
        gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
        c->SaveAs(Dir+canvasName+logy_tag+".pdf","pdf");
        gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

        c->Close();
      }
    }
  }
  printRunTime(timer_total);
}
