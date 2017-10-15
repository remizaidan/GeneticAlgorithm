#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

class IModel;
class IFigureOfMerit;
class IPopulation;

/**
 * @brief Class imlementing the Genetic Algorithm.
 *
 * There is no need to derive from this class: the implementation is built on top of the 
 * IModel, IFigureOfMerit and IPopulation interfaces.
 *
 * The algorithm flow is as follows:
 * - Creates an initial population.
 * - Rank the population.
 * - Repeat the following until a solution is found or a maximum numeber of generations is reached:
 *   - Select parents among the fittest individuals.
 *   - Cross them over to form a new population.
 *   - Mutate some individuals.
 *   - Rank the new population.
 */
class GeneticAlgorithm {

public:

  /** Default Constructor */
  GeneticAlgorithm();

  /** Destructor */
  ~GeneticAlgorithm();

  /** Finds the best solution given a population of models. */
  IModel *optimize(IPopulation *population);

  /** Initialize the algorithm before the optimization loop starts. */
  void initialize(IPopulation *population);

  /** Perform one iteration of the optimization loop: creates next generation of models. */
  bool nextGeneration();

  /** Returns the current generation number. */
  int getCurrentGeneration();

  /** Sets the maximum number of generations before giving up. */
  void setNGenerationsMax(int generationsMax);

  /** Sets the population size */
  void setPopulationSize(int populationSize);

private:

  int m_generationsMax; //!< Stores the maximum number of generations.
  int m_populationSize; //!< Stores the desired population size.
  int m_currentGeneration; //!< Stores the number of the current generation.
  IPopulation *m_population; //!< Stores a pointer to the population being optimized.
};

#endif
