/**
 * @file
 * @defgroup testParentSelection Macro testParentSelection
 *
 * @brief Test Parent rank-based selection.
 *
 * @b Objective: test the algorithm to select parents based on the ranking.
 * The probability to be selected should show a linear dependence on the rank.
 *
 * To run this macro using ROOT:
 * > root -l testParentSelection.C+
 *
 * <b>Output example:</b>
 *
 * <table class="image" align="center">
 * <caption align="bottom" id="Fig1"> Parent selection probability as function of parent's rank. </caption>
 * <tr><td><img src="../../figures/C_ParentProb.png" alt="Test Parent Selection."/></td></tr>
 * </table>
 *
 * @{
 */

#include <iostream>

#include <TRandom3.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TLegend.h>

/**
 * @brief Implements the parent selection criteria.
 */
void selectParents(TRandom3 *random, int &p1, int &p2, int N)
{

  int f1 = 0;
  int f2 = 0;
  do {
    f1 = random->Integer(N);
    p1 = random->Integer(N);
  }while(p1 > f1);
  do{
    f2 = random->Integer(N);
    p2 = random->Integer(N);    
  }
  while(p1 == p2 || p2 > f2);
} 

/**
 * @brief Tests the parent selection criteria.
 *
 * Run a toy simulation and measure the selection probability as function of the rank.
 */
void testParentSelection() {

  std::cout << "Hello" << std::endl;

  // Our population size
  int populationSize = 500;

  // Initialize a pseudo-random number generator
  TRandom3 *rnd = new TRandom3(1234);

  // Initialize a histogram to store the probability density function
  int nbins = 100;
  TH1 *hProb = new TH1F("hProb", "", nbins, 0, populationSize);

  // Number of experiments
  int nmc = 100000;

  // Run toy simulation
  for(int mc=0; mc<nmc; mc++) {
    int p1, p2;
    selectParents(rnd, p1, p2, populationSize);
    hProb->Fill(p1);
    hProb->Fill(p2);
  }

  // Scale the histogram to convert it into a PDF
  hProb->Scale(1./(2.*nmc*populationSize/nbins));

  // Plot it.
  gStyle->SetOptStat(0); // Don't show stat box.
  TString cname = "C_ParentProb";
  TCanvas *C = new TCanvas(cname, cname);
  C->cd()->SetGrid();
  hProb->SetLineColor(1);
  hProb->SetMarkerStyle(20);
  hProb->SetMarkerSize(0.8);
  hProb->SetLineWidth(2);
  hProb->GetXaxis()->SetTitle("Rank");
  hProb->GetYaxis()->SetTitle("Probability density");
  hProb->GetXaxis()->SetTitleSize(0.05);
  hProb->GetYaxis()->SetTitleSize(0.05);
  hProb->GetXaxis()->SetTitleOffset(0.9);
  hProb->GetYaxis()->SetTitleOffset(0.9);
  hProb->Draw();

  // Fit it with a linear function
  TF1 *fLinear = new TF1("fLinear", "pol1", 0, populationSize);
  hProb->Fit(fLinear, "NOQ");
  fLinear->SetLineColor(2);
  fLinear->Draw("same");

  // Add a legend
  TLegend *L = new TLegend(0.6,0.7,0.89,0.89);
  L->AddEntry(hProb, "Measured PDF", "lp");
  L->AddEntry(fLinear, "Linear fit:", "l");
  L->AddEntry("", TString::Format("y = %.2g x + %.2g", fLinear->GetParameter(1), fLinear->GetParameter(0)), "");
  L->Draw();
  
  // Save the plot
  C->SaveAs(cname+".png");
  C->SaveAs(cname+".root");
}
