#include "ParametricModelPopulation.h"

#include "ParametricModel.h"

ParametricModelPopulation::ParametricModelPopulation() :
  IPopulation()
{
  m_formula = 0;
  m_mutationSize = 0.1;
}

ParametricModelPopulation::~ParametricModelPopulation()
{
}

/**
 * @param formula Formula to be used for this population.
 */
void ParametricModelPopulation::setFormula(TF1 *formula)
{
  m_formula = formula;
}

/**
 * @param relativeSize The relative size (sigma) of the gaussian noise applied during mutation.
 */
void ParametricModelPopulation::setMutationSize(double relativeSize)
{
  m_mutationSize = relativeSize;
}

/**
 * Parameters for the individual models are randomly initialized following uniform 
 * distribution in the allowed range as defined in the population's formula.
 *
 * @param The desired size of the population.
 */
void ParametricModelPopulation::doInitialize(int n)
{
  clear();
  for(int i=0; i<n; i++) {
    ParametricModel *model = new ParametricModel();
    model->setFormula(m_formula);
    TF1 *formula = model->getFormula();
    for(int p=0; p<formula->GetNpar(); p++) {
      double pmin, pmax;
      formula->GetParLimits(p, pmin, pmax);
      if(pmin < pmax) {
	double par = m_random->Uniform(pmin, pmax);
	formula->SetParameter(p, par);
      }
    }
    m_individuals.push_back(model);
  }
}

/**
 * Cross-over is implemented such that each parameter is passed from either parents chosen at random. 
 *
 * @param parents List of parents to be crossed-over.
 */
void ParametricModelPopulation::doCrossOver(const std::vector<std::vector<IModel*> > &parents)
{
  
  std::vector<std::vector<double> > offspringGenes;
  for(int i=0; i<size(); i++) {
    std::vector<double> offspring;
    if(parents[i].size()==1) {
      ParametricModel *parent = dynamic_cast<ParametricModel*>(parents[i][0]);
      if(!parent) {
	throw std::runtime_error("Given models are not parametric models");
      }
      TF1 *formula = parent->getFormula();
      for(int p=0; p<formula->GetNpar(); p++) {
	offspring.push_back(formula->GetParameter(p));
      }
    }else if(parents[i].size() == 2) {
      ParametricModel *parent1 = dynamic_cast<ParametricModel*>(parents[i][0]);
      ParametricModel *parent2 = dynamic_cast<ParametricModel*>(parents[i][1]);
      if(!parent1 || !parent2) {
	throw std::runtime_error("Given models are not parametric models");
      }
      TF1 *formula1 = parent1->getFormula();
      TF1 *formula2 = parent2->getFormula();
      for(int p=0; p<formula1->GetNpar(); p++) {
	if(m_random->Integer(2)) {
	  offspring.push_back(formula1->GetParameter(p));
	}else{
	  offspring.push_back(formula2->GetParameter(p));
	}
      }
    }
    offspringGenes.push_back(offspring);
  }

  for(int i=0; i<size(); i++) {
    ParametricModel *model = (ParametricModel*)m_individuals[i];
    for(unsigned int p=0; p<offspringGenes[i].size(); p++) {
      model->getFormula()->SetParameter(p, offspringGenes[i][p]);
    }
  }
}

/**
 * Mutation is implemented such that a random parameter is chosen and then modified by adding a gaussian noise component.
 * The size of the gaussian noise is controlled via setMutationSize().
 *
 * @param imodel Model to be mutated.
 */
void ParametricModelPopulation::doMutate(IModel *imodel)
{
  
  ParametricModel *model = dynamic_cast<ParametricModel*>(imodel);
  if(!model) {
    throw std::runtime_error("Given models are not parametric models");
  }

  TF1 *formula = model->getFormula();
  int p = m_random->Integer(formula->GetNpar());
  double pmin, pmax;
  formula->GetParLimits(p, pmin, pmax);
  if(pmin < pmax) {
    double par = formula->GetParameter(p);
    par += m_random->Gaus(0, par==0?m_mutationSize:par*m_mutationSize);
    formula->SetParameter(p, par);
  }
}

