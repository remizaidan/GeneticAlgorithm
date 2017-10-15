/**
 * @file
 */

#include <iostream>

#include <TRandom3.h>

#include "ParametricModel.h"
#include "ParametricModelPopulation.h"
#include "Chi2FitFigureOfMerit.h"
#include "GeneticAlgorithm.h"
#include "optparse.h"

#include <TH1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TString.h>
#include <TLegend.h>

void parseCommandLine(Config &config, int argc, char **argv);

/**
 * @defgroup runGA Demo Program
 *
 * @brief Demo Program.
 *
 * @b Objective: Use the Genetic Algorithm to fit a gaussian probability density function to a data distribution.
 *
 * @{
 */

/**
 * @brief Main function
 *
 * This program performs the following tasks:
 * - Parse the command line and defines configuration.
 * - Generates a dataset following a gaussian distribution.
 * - Fits the generated distribution using ROOT's implementation.
 * - Fits the generated distribution using our GA implementation.
 *   - Perform some tests about the behavior of our GA algorithm.
 * - Plot the results of the main algorithm and the tests.
 *
 * Full documentation of the algorithms is available in @ref index.
 * 
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return 0 upon successfull exit
 */
int main(int argc, char **argv) {

  //
  // Initialize program settings
  //
  Config config;
  parseCommandLine(config, argc, argv);

  std::cout << "Algorithm Configuration:" << std::endl
	    << "  ==> nmc = " << (int)config.get("nmc") << std::endl
	    << "  ==> acceptThreshold = " << (double)config.get("acceptThreshold") << std::endl
	    << "  ==> mutateRate = " << (double)config.get("mutateRate") << std::endl
	    << "  ==> mutateSize = " << (double)config.get("mutateSize") << std::endl
	    << "  ==> maxGenerations = " << (int)config.get("maxGenerations") << std::endl
	    << "  ==> populationSize = " << (int)config.get("populationSize") << std::endl;
  
  //
  // Generates a dataset following a gaussian distribution.
  //
  int nmc = config.get("nmc");
  double mean = config.get("mean");
  double sigma = config.get("sigma");  
  TRandom3 rnd(1234);
  double xmin = mean-5*sigma;
  double xmax = mean+5*sigma;
  int nbins = 100;
  TH1 *hData = new TH1F("hData", "", nbins, xmin, xmax);
  double dx = (xmax-xmin)/nbins;
  hData->Sumw2();
  hData->GetXaxis()->SetTitle("x");
  hData->GetYaxis()->SetTitle("Probability density");
  for(int mc=0; mc<nmc; mc++) {
    hData->Fill(rnd.Gaus(mean, sigma), 1./(nmc*dx));
  }

  //
  // Perform a likelihood fit using ROOT's implementation.
  //
  TF1 *likelihoodFit = new TF1("likelihoodFit", "gaus", xmin, xmax);
  hData->Fit(likelihoodFit, "NOQ");

  //
  // Initialize the figure of merit and pass the data.
  //
  Chi2FitFigureOfMerit fom;
  fom.setAcceptThreshold(config.get("acceptThreshold"));
  std::vector<double> x(1);
  for(int bin=1; bin<=hData->GetNbinsX(); bin++) {
    x[0] = hData->GetBinCenter(bin);
    fom.addData(x, hData->GetBinContent(bin), hData->GetBinError(bin));
  }

  //
  // Configure the population to be optimized
  //
  ParametricModelPopulation population;
  population.setMutateRate(config.get("mutateRate"));
  population.setMutationSize(config.get("mutateSize"));
  population.setFigureOfMerit(&fom);
  TF1 *f = new TF1("f", "gaus", xmin, xmax);
  f->SetParameter(0, 1./(sigma*sqrt(2*TMath::Pi())));
  f->SetParameter(1, mean);
  f->SetParameter(2, sigma);
  f->SetParLimits(0, 0.001, 1);
  f->SetParLimits(1, -10, 10);
  f->SetParLimits(2, 0.001, 10);
  f->SetParName(0, "Constant");
  f->SetParName(1, "Mean");
  f->SetParName(2, "Sigma");
  population.setFormula(f);

  std::cout << "Input parameters:" << std::endl;
  for(int i=0; i<f->GetNpar(); i++) {
    double pmin, pmax;
    f->GetParLimits(i, pmin, pmax);
    std::cout << "  ==> " << f->GetParName(i) << " : " << f->GetParameter(i)
	      << "  - range: [" << pmin << ", " << pmax << "]"
	      << std::endl;
  }

  //
  // Configure the Genetic Algorithm
  //
  GeneticAlgorithm alg;
  alg.setNGenerationsMax(config.get("maxGenerations"));
  alg.setPopulationSize(config.get("populationSize"));

  //
  // Prepare for making plots
  //
  gSystem->Exec("rm -rf figures");
  gSystem->Exec("mkdir -p figures");
  gStyle->SetOptStat(0); // disables stat box on plots
  TGraph *gScore = new TGraph();
  TGraph *gRMS = new TGraph();
  TCanvas *C_anim = new TCanvas("C_anim", "C_anim");
  hData->SetMarkerStyle(20);
  likelihoodFit->SetLineColor(4);
  likelihoodFit->SetLineStyle(7);
  f->SetLineColor(2);
  TLegend *L_anim = new TLegend(0.7, 0.7, 0.9, 0.9);
  L_anim->AddEntry(hData, "Data", "lp");
  L_anim->AddEntry(likelihoodFit, "Likelihood fit", "l");
  L_anim->AddEntry(f, "GA fit", "l");
  TLatex *lt_anim = new TLatex();
  
  //
  // Run the GA and the tests if requested
  //
  alg.initialize(&population);
  do {
    if(config.get("runTests")) {
      ParametricModel *bestModel = (ParametricModel*)population.getBestFitted();
      gScore->SetPoint(gScore->GetN(), alg.getCurrentGeneration(), bestModel->getScore());
      gRMS->SetPoint(gRMS->GetN(), alg.getCurrentGeneration(), population.getScoreRMS()/bestModel->getScore());
      C_anim->cd();
      hData->Draw();
      likelihoodFit->Draw("same");
      bestModel->getFormula()->SetLineColor(2);
      bestModel->getFormula()->Draw("same");
      L_anim->Draw();
      lt_anim->SetText(xmin + (xmax-xmin)/10., hData->GetMaximum()*0.9, TString::Format("Generation: %d", alg.getCurrentGeneration()));
      lt_anim->Draw();
      C_anim->Modified();
      C_anim->Update();
      C_anim->SaveAs("figures/C_anim.gif+25");
      std::cout << "\rGeneration: " << alg.getCurrentGeneration();
      std::flush(std::cout);
    }
  } while(alg.nextGeneration());
  if(config.get("runTests")) {
    std::cout << std::endl;
  }

  //
  // Display results and make plots
  //
  ParametricModel *bestModel = (ParametricModel*)population.getBestFitted();
  TF1 *bestFormula = bestModel->getFormula();

  std::cout << "Done after " << alg.getCurrentGeneration() << " generations." << std::endl
	    << "  ==> Best score is: " << bestModel->getScore() << std::endl;
  
  std::cout << "After GA fit: " << std::endl;
  for(int i=0; i<bestFormula->GetNpar(); i++) {
    std::cout << "  ==> " << f->GetParName(i) << " : " << bestFormula->GetParameter(i) << std::endl;
  }

  std::cout << "After Likelihood fit: " << std::endl;
  for(int i=0; i<likelihoodFit->GetNpar(); i++) {
    std::cout << "  ==> " << f->GetParName(i) << " : " << likelihoodFit->GetParameter(i) << std::endl;
  }

  TCanvas *C_fit = new TCanvas("C_fit", "C_fit");
  C_fit->cd();
  hData->Draw("ep");
  likelihoodFit->Draw("same");
  bestFormula->SetLineColor(2);
  bestFormula->Draw("same");
  L_anim->Draw();
  lt_anim->SetText(xmin + (xmax-xmin)/10., hData->GetMaximum()*0.9, TString::Format("Generation: %d", alg.getCurrentGeneration()));
  lt_anim->Draw();
  C_fit->SaveAs("figures/C_fit.png");
  C_fit->SaveAs("figures/C_fit.root");

  if(config.get("runTests")) {
    C_fit->SaveAs("figures/C_anim.gif++300");
  
    TCanvas *C_Score = new TCanvas("C_Score", "C_Score");
    C_Score->cd()->SetLogx();
    C_Score->cd()->SetLogy();
    gScore->SetLineColor(4);
    gScore->SetLineWidth(2);
    gScore->Draw("AL");
    C_Score->SaveAs("figures/C_Score.png");
    C_Score->SaveAs("figures/C_Score.root");
    
    TCanvas *C_RMS = new TCanvas("C_RMS", "C_RMS");
    C_RMS->cd()->SetLogx();
    C_RMS->cd()->SetLogy();
    gRMS->SetLineColor(4);
    gRMS->SetLineWidth(2);
    gRMS->Draw("AL");
    C_RMS->SaveAs("figures/C_RMS.png");
    C_RMS->SaveAs("figures/C_RMS.root");
  }
  
  return 0;
}


/** 
 * @brief Prase command line arguments.
 *
 * @param config Configuration to parse into.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 *
 * #### Configuration details: 
 */
void parseCommandLine(Config &config, int argc, char **argv)
{

  optparse::OptionParser parser = optparse::OptionParser().description("Point Cloud Analysis");

  /** - @b -n, <b> \-\-nmc </b> Number of toy MC experiments used to build the dataset.*/
  parser.add_option("-n", "--nmc").action("store").dest("nmc").set_default(10000)
    .help("Number of toy MC experiments used to build the dataset.");

  /** - @b -m, <b> \-\-mean </b> Mean of the gaussian distribution used to generate the dataset. */
  parser.add_option("-m", "--mean").action("store").dest("mean").set_default(1.5)
    .help("Mean of the gaussian distribution used to generate the dataset.");

  /** - @b -s, <b> \-\-sigma </b> Width (sigma) of the gaussian distribution used to generate the dataset. */
  parser.add_option("-s", "--sigma").action("store").dest("sigma").set_default(2.3)
    .help("Width (sigma) of the gaussian distribution used to generate the dataset.");

  /** - @b -a, <b> \-\-acceptThreshold </b> Score threshold to accept a model as a final answer. */
  parser.add_option("-a", "--acceptThreshold").action("store").dest("acceptThreshold").set_default(0.85)
    .help("Score threshold to accept a model as a final answer.");

  /** - @b -R, <b> \-\-mutateRate </b> Rate at which models are subjected to mutation. */
  parser.add_option("-R", "--mutateRate").action("store").dest("mutateRate").set_default(0.01)
    .help("Rate at which models are subjected to mutation.");

  /** - @b -S, <b> \-\-mutateSize </b> Relative size of the mutation whenever applied. */
  parser.add_option("-S", "--mutateSize").action("store").dest("mutateSize").set_default(0.1)
    .help("Relative size of the mutation whenever applied.");

  /** - @b -G, <b> \-\-maxGenerations </b> Maximum number of generations before aborting the optimization loop. */
  parser.add_option("-G", "--maxGenerations").action("store").dest("maxGenerations").set_default(10000)
    .help("Maximum number of generations before aborting the optimization loop.");

  /** - @b -N, <b> \-\-populationSize </b> Size of the population to be evolved. */
  parser.add_option("-G", "--populationSize").action("store").dest("populationSize").set_default(500)
    .help("Size of the population to be evolved.");

  /** - @b -t, <b> \-\-runTests </b> Run tests alongside the main algorithm. */
  parser.add_option("-t", "--runTests").action("store_true").dest("runTests").set_default(false)
    .help("Run tests alongside the main algorithm.");

  config = parser.parse_args(argc, argv);
}
