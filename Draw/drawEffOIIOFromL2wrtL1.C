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

void drawEffOIIOFromL2wrtL1(
  TString ver = "v6", TString SAMPLE = "Bs", TString tag = "Bs",
  TString L1tag = "L1DQ0", TString L1str = "L1 qual > 7, no p_{T}^{L1} cut",
  bool isLogy = false  // HERE
) {
  TStopwatch timer_total;
  timer_total.Start();

  gStyle->SetPalette(kRainBow);
  TH1::SetDefaultSumw2(kTRUE);

  TString efftag_str = "OI + IO(L2)";
  TString efftag = "OI_IOFromL2_L1_comp";
  TString Dir = "./plots_"+ver+"/"+tag+"/Eff_"+efftag+"/"+L1tag+"/";
  if (gSystem->mkdir(Dir,kTRUE) != -1)
    gSystem->mkdir(Dir,kTRUE);

  vector<TString> v_var = {"pt_zoom", "pt", "l1ptByQ", "l1ptByQ_zoom", "l2pt", "l2pt_zoom", "eta", "phi", "pu"};
  vector< vector<double> > range = {
    {1, 0, 200},  // pt
    {1, 0, 200},  // pt
    {1, 0, 200},  // L1 pt
    {1, 0, 200},  // L1 pt
    {1, 0, 200},  // L2 pt
    {1, 0, 200},  // L2 pt
    {1, -2.4, 2.4},  // eta
    {1, -TMath::Pi(), TMath::Pi()},
    {1, 30, 81}  // PU
  };
  if (tag == "JPsi" || tag == "Bs") {
    range.at(0) = {1, 0, 50};
    range.at(1) = {1, 0, 50};
    range.at(2) = {1, 0, 50};
    range.at(3) = {1, 0, 50};
    range.at(4) = {1, 0, 50};
    range.at(5) = {1, 0, 50};
  }
  if (tag == "MuGunPU") {
    range.at(0) = {1, 1., 1000};
    range.at(1) = {1, 1., 1000};
    range.at(2) = {1, 1., 1000};
    range.at(3) = {1, 1., 1000};
    range.at(4) = {1, 1., 1000};
    range.at(5) = {1, 1., 1000};
  }
  if (tag == "Zprime") {
    range.at(0) = {1, 30, 3000};
    range.at(1) = {1, 30, 3000};
    range.at(2) = {1, 30, 3000};
    range.at(3) = {1, 30, 3000};
    range.at(4) = {1, 30, 3000};
    range.at(5) = {1, 30, 3000};
  }

  int n_pt_bins = 52-1;
  double pt_bins[52] = {
    0., 0.5, 1., 1.5, 2., 2.5, 3., 3.5, 4., 4.5,
    5., 5.5, 6., 6.5, 7., 7.5, 8., 8.5, 9., 9.5,
    10, 11, 12, 13, 14,
    15, 16, 17, 18, 19,
    20, 21, 22, 23, 24,
    26, 30, 35, 40, 50,
    60, 80, 100, 120, 160,
    200, 300, 450, 700, 1000,
    2000, 3000
  };

  int n_eta_bins = 23-1;
  double eta_bins[23] = {
    -2.4, -2.1, -1.9, -1.7, -1.6, -1.5, -1.3, -1.2, -0.9,
    -0.3, -0.2,  0.0,  0.2,  0.3,
     0.9,  1.2, 1.3, 1.5, 1.6, 1.7, 1.9, 2.1,  2.4
  };
  vector<TString> etas_str = {"I", "B", "E"};
  vector<TString> etas_str_long = {"|#eta^{gen}| < 2.4", "|#eta^{gen}| < 1.2", "1.2 < |#eta^{gen}| < 2.4"};

  vector<Color_t> v_color = {
    kBlack,
    kBlue,
    kRed,
    kGreen+2,
    kMagenta,
    kCyan+2,
    kPink+4,
    //kGray+2,
    //kOrange,
  };
  vector<int> v_marker = {
    20,
    22,
    26,
    23,
    32,
    22,
    26,
    //23,
    //32,
  };
  vector<TString> files = {
    //"./Outputs_"+ver+"/hist-"+ver+"-default_V29-"+tag+"-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-default-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_2-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_2_ROIL2_3-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_2_ROIL2_5-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_3-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_4-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_5-"+tag+"-Eff.root",

    "./Outputs_"+ver+"/hist-"+ver+"-ROIL1_5_ROIL2_5-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROIL1_543_ROIL2_54-"+tag+"-Eff.root",
    //"./Outputs_"+ver+"/hist-"+ver+"-ROI_phi_5GeV_eta_smooth-"+tag+"-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-ROI_phi_5431p5_eta1-"+tag+"-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-ROI_phi_5431p5_eta1_wp02-"+tag+"-Eff.root",
    "./Outputs_"+ver+"/hist-"+ver+"-ROI_phi_5431p5_eta1_wp04-"+tag+"-Eff.root",
  };
  vector<TString> types = {
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/num_Eff_"+L1tag+"_hltL3FromL2Merged",
  };
  vector<TString> types_den = {
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
    "Eff/hltL3FromL2Merged/den_Eff_"+L1tag+"_hltL3FromL2Merged",
  };
  vector<TString> types_str = {
    //efftag_str+" : Default GRun V29",
    efftag_str+" : Default GRun V51 (1.5 #times 1.5)",
    //efftag_str+" : ROI(L1) #phi 2.0",
    //efftag_str+" : ROI(L1) #phi 2.0 & ROI(L2) #phi 3.0",
    //efftag_str+" : ROI(L1) #phi 2.0 & ROI(L2) #phi 5.0",
    //efftag_str+" : ROI(L1) #phi 3.0",
    //efftag_str+" : ROI(L1) #phi 4.0",
    //efftag_str+" : ROI(L1) #phi 5.0",

    efftag_str+" : ROI(L1,L2) 1.5 #times 5.0",
    //efftag_str+" : ROI(L1) #phi 5,4,3 ROI(L2) #phi 5,4",
    //efftag_str+" : ROI(L1) #phi 5GeV bins #eta smooth",
    efftag_str+" : ROI(L1,L2) fixed #eta, pT dep. #phi",
    efftag_str+" : ROI(L1,L2) fixed #eta, pT dep. #phi [WP 0.02]",
    efftag_str+" : ROI(L1,L2) fixed #eta, pT dep. #phi [WP 0.04]",

    //efftag_str+" : ROI(L1) pT dep. ROI 5.0 - 2.0 - 1.5",
    //efftag_str+" : ROI(L1) pT indep. ROI 5.0",
    //efftag_str+" : ROI(L1) pT indep. ROI 5.0 & ROI(L2) 5.0",
  };

  vector<TString> v_pts = {
    "genpt0",
    //"genpt10",
    //"genpt26"
  };

  vector<TString> v_pts_str = {
    "",
    //"p_{T}^{gen} > 10 GeV",
    //"p_{T}^{gen} > 26 GeV"
  };

  for(unsigned i_eta=0; i_eta<etas_str.size(); i_eta++){
    for(int ipt=0; ipt<(int)v_pts.size(); ++ipt) {
      for(int ivar=0; ivar<(int)v_var.size(); ++ivar) {

        double xmin = range[ivar][1];
        double xmax = range[ivar][2];
        double ymin = 0.0;
        double ymax = 1.6;

        if(v_var[ivar].Contains("zoom")){
          ymin = 0.85;
          ymax = 1.03;
        }else if (!v_var[ivar].Contains("pt")){
          ymin = 0.2;
          ymax = 1.35;
        }

        TString canvasName = TString::Format("Eff_%s_%s_%s_%s_%s_%s",
                                             efftag.Data(),
                                             tag.Data(),
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
        for(int i = 0; i<(int)files.size(); ++i) {
          TString fileName = files.at(i);

          TString the_type_num = types[i];
          TString the_type_den = types_den[i];
          TString the_type_str = types_str[i];

          TString hist_var = v_var[ivar];
          hist_var.ReplaceAll("_zoom", "");

          TString titleX = GetTitleX(hist_var+"_"+(!hist_var.Contains("l1")?"gen":"l1"));
          TString titleY = "L3/L1 efficiency";
          if(hist_var.Contains("l2")) titleY = "L3/L2 efficiency";

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
          g->Divide(num, den, "n e0");

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
        Latex_Simulation_14TeV( latex );
        latex.DrawLatexNDC( 0.45,0.96, "#scale[0.8]{#font[42]{"+SAMPLE+"}}");
        latex.DrawLatexNDC(0.17, 0.88, "#font[42]{#scale[0.8]{"+L1str+"}}");
        latex.DrawLatexNDC((i_eta==2?0.66:0.70), 0.89, "#font[42]{#scale[0.8]{"+etas_str_long.at(i_eta)+"}}");
        if(v_var[ivar] != "pt" ) latex.DrawLatexNDC(0.70, 0.84, "#font[42]{#scale[0.8]{"+v_pts_str.at(ipt)+"}}");

        TString logy_tag = isLogy ? "_log" : "";
        // CMS_lumi(c, 98, 11);
        c->Modified();  c->Update();  c->RedrawAxis();
        gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;");
        c->SaveAs(Dir+canvasName+logy_tag+".pdf","pdf");
        // c->SaveAs(Dir+canvasName+logy_tag+".C","C");
        // c->SaveAs(Dir+canvasName+logy_tag+".root","root");
        gROOT->ProcessLine( "gErrorIgnoreLevel = kPrint;");

        c->Close();
      }
    }
  }
  printRunTime(timer_total);
}
