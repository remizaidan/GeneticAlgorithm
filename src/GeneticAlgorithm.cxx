#include "GeneticAlgorithm.h"
#include "IModel.h"
#include "IFigureOfMerit.h"
#include "IPopulation.h"

GeneticAlgorithm::GeneticAlgorithm()
{
  m_generationsMax = 10000;
  m_populationSize = 100;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

/**
 * @param population Population of models to optimize.
 * @return Best fitted model after optimization.
 */
IModel *GeneticAlgorithm::optimize(IPopulation *population)
{

  initialize(population);

  while(nextGeneration());
  
  return population->getBestFitted();
}

/**
 * This function is provided so that the user have the option to control the optimization loop
 * and possibly execute some code before/after each iteration. This can be useful for example
 * to implement tests that need to monitor the progress of the optimization.
 *
 * @param population Population of models to optimize.
 */
void GeneticAlgorithm::initialize(IPopulation *population) {

  population->initialize(m_populationSize);
  population->score();
  
  m_currentGeneration = 0;
  m_population = population;
}

/**
 * This function is provided so that the user have the option to control the optimization loop
 * and possibly execute some code before/after each iteration. This can be useful for example
 * to implement tests that need to monitor the progress of the optimization.
 *
 * @return `true` if more generations are needed, `false` if optimal solution has been reached.
 */
bool GeneticAlgorithm::nextGeneration() {

  if(m_population->getFigureOfMerit()->accept(m_population->getBestFitted())) {
    return false;
  }
    
  if(m_currentGeneration > m_generationsMax) {
    return false;
  }

  m_currentGeneration++;

  m_population->crossOver();
  m_population->mutate();
  m_population->score();

  return true;
}

/**
 * @return Number of the current generation.
 */
int GeneticAlgorithm::getCurrentGeneration()
{
  return m_currentGeneration;
}

/**
 * @param Desired maximum number of iterations.
 */
void GeneticAlgorithm::setNGenerationsMax(int generationsMax)
{  
  m_generationsMax = generationsMax;
}


/**
 * @param Desired population size.
 */
void GeneticAlgorithm::setPopulationSize(int populationSize)
{
  m_populationSize = populationSize;
}
