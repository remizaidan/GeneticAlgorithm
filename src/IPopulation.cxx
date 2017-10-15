#include "IPopulation.h"

#include "IModel.h"
#include "IFigureOfMerit.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

IPopulation::IPopulation()
{
  m_sorted = false;
  m_fom = 0;
  m_random = new TRandom3(1234);
  m_mutateRate = 0.01;
  m_scoreMean = 0;
  m_scoreRMS = 0;
}

IPopulation::~IPopulation()
{
  delete m_random;
}

/**
 * @param n Desired size of the population.
 */
void IPopulation::initialize(int n)
{
  doInitialize(n);
  m_sorted = false;
}

void IPopulation::crossOver()
{

  sort();

  m_parents.resize(size());
  for(int i=0; i<size(); i++) {
    if(i==0) {
      m_parents[i].resize(1);
      m_parents[i][0] = m_individuals[0];
    }else{
      int p1, p2;
      selectParents(p1, p2);
      m_parents[i].resize(2);
      m_parents[i][0] = m_individuals[p1];
      m_parents[i][1] = m_individuals[p2];
    }
  }
  doCrossOver(m_parents);
  m_sorted = false;
}

void IPopulation::mutate()
{
  for(int i=0; i<size(); i++) {
    double f = m_random->Uniform(0,1);
    if(f < m_mutateRate) {
      doMutate(m_individuals[i]);
    }
  }
  m_sorted = false;
}

/**
 * @param seed Random number generator seed.
 */
void IPopulation::setRandomSeed(int seed)
{
  
  m_random->SetSeed(seed);
}

/**
 * @param rate Desired mutation rate.
 */
void IPopulation::setMutateRate(double rate)
{

  if(rate<0 || rate > 1) {
    std::ostringstream ostr;
    ostr << "Specified rate (" << rate << ") is out of range [0,1]";
    throw std::runtime_error(ostr.str().c_str());
  }
  m_mutateRate = rate;
}

/**
 * This function also calculates the mean and RMS for the scores of this population.
 */
void IPopulation::score() {

  checkFigureOfMerit();
  
  if(!size()) return;
  
  m_scoreMean = 0;
  m_scoreRMS = 0;

  for(int i=0; i<size(); i++) {
    double score = m_fom->evaluate(m_individuals[i]);
    m_individuals[i]->setScore(score);
    m_scoreMean += score;
    m_scoreRMS += score*score;
  }
  m_scoreMean /= size();
  m_scoreRMS /= size();
  m_scoreRMS = m_scoreRMS - m_scoreMean*m_scoreMean;
  if(m_scoreRMS<0) m_scoreRMS = 0;
  m_scoreRMS = sqrt(m_scoreRMS);

  sort();
}

/**
 * @param fom Pointer to a figure of merit object to be used to calculate scores and perform the ranking.
 */
void IPopulation::setFigureOfMerit(IFigureOfMerit *fom)
{
  m_fom = fom;
}

/**
 * @return Pointer to the figure of merit object used to calculate scores and perform the ranking.
 */
IFigureOfMerit *IPopulation::getFigureOfMerit()
{
  return m_fom;
}

/**
 * @param rank Rank.
 * @return Model at the given rank.
 */
IModel *IPopulation::getBestFitted(int rank)
{
  
  if(rank < 0 || rank >= size()) {
    std::ostringstream ostr;
    ostr << "Rank (" << rank << ") is out of range [" << 0 << ", " << size() << "[";
    throw std::runtime_error(ostr.str().c_str());
  }
  
  sort();

  return m_individuals[rank];
}

/**
 * @return Size of this population.
 */
int IPopulation::size()
{  
  return m_individuals.size();
}

/**
 * @return Mean score for this population.
 */
double IPopulation::getScoreMean()
{
  return m_scoreMean;
}

/**
 * @return RMS of the scores for this population.
 */
double IPopulation::getScoreRMS()
{
  return m_scoreRMS;
}

void IPopulation::clear()
{  
  for(int i=0; i<size(); i++) {
    delete m_individuals[i];
  }
  m_individuals.clear();
}

void IPopulation::sort()
{
  if(m_sorted) return;

  checkFigureOfMerit();

  m_sorted = true;
  
  if(size() <= 1) return;
  
  int n = size();
  while (n > 0) {
    int newn = 0;
    for (int i = 1; i<=n-1; i++) {
      if ( m_fom->isBetterThan(m_individuals[i], m_individuals[i-1]) ) {
	IModel *temp = m_individuals[i];
	m_individuals[i] = m_individuals[i-1];
	m_individuals[i-1] = temp;
	newn = i;
      }
    }
    n = newn;
  }
}


void IPopulation::checkFigureOfMerit()
{
  
  if(!m_fom) {
    throw std::runtime_error("Figure of merit not assigned for this population.");
  }
}

/**
 * @param p1 Returns the index of the first parent.
 * @param p2 Returns the index of the second parent.
 */
void IPopulation::selectParents(int &p1, int &p2)
{

  int f1 = 0;
  int f2 = 0;
  do {
    f1 = m_random->Integer(size());
    p1 = m_random->Integer(size());
  }while(p1 > f1);
  do{
    f2 = m_random->Integer(size());
    p2 = m_random->Integer(size());    
  }
  while(p1 == p2 || p2 > f2);
} 
