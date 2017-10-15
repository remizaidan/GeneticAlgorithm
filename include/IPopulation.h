#ifndef IPOPULATION_H
#define IPOPULATION_H

#include <vector>
#include "TRandom3.h"

class IModel;
class IFigureOfMerit;

/**
 * @brief Abstract class describing a population of models.
 *
 * This class should implement the following functionalities:
 * - Initialize the population: usually this use a random number generator. 
 * The actual implementation is left to the derived classes.
 * - Selection: decide which members of the population should be cross-overed.
 * The default behavior is to select parents with a probability that is linear with the rank.
 * - Cross-over: implements the logic based on which a child is constructed from its parents.
 * - Mutation: implements the logic based on which a child is altered through random mutations.
 *
 * Derive from this class by implementing at least these three methods:
 * - doInitialize(): performs the initialization of the population
 * - doCrossOver(): performs the cross-over. 
 * - doMutate(): performs the mutation of a model.
 * Additionally, one might override the selectParents() method to change its default behavior.
 */
class IPopulation {

public:

  /** Default Constructor */
  IPopulation();

  /** Destructor */
  virtual ~IPopulation();

  /** Initialize a population of a given size. */
  void initialize(int n);

  /** Performs population cross-over. */
  void crossOver();

  /** Performs individual mutations. */
  void mutate();

  /** Compute the scores for the members of the population. */
  void score();

  /** Sets the random seed for the random number generator. */
  void setRandomSeed(int seed);

  /** Sets the mutation rate. */
  void setMutateRate(double rate);

  /** Sets the figure of merit to be used to calculate scores and perform the ranking. */
  void setFigureOfMerit(IFigureOfMerit *fom);

  /** Returns the figure of merit that is used to calculate scores and perform the ranking. */
  IFigureOfMerit *getFigureOfMerit();

  /** Returns the method at a given rank. */
  IModel *getBestFitted(int rank=0);

  /** Returns the size of the population. */
  int size();

  /** Returns the mean score for the population. */
  double getScoreMean();

  /** Returns the RMS of the scores for the population. */
  double getScoreRMS();

  /** Resets the population. */
  void clear();
  
protected:

  /** Should implement the actual initialization. */
  virtual void doInitialize(int n)=0;

  /** Should implement the cross-over. */
  virtual void doCrossOver(const std::vector<std::vector<IModel*> > &parents)=0;

  /** Should implement the mutation of a model. */
  virtual void doMutate(IModel *model)=0;

  /** Selects parents to be crossed-over */
  virtual void selectParents(int &p1, int &p2);

  /** Performs the ranking from the best to the least fitted. */
  void sort();

  /** Makes sure an IFigureOfMerit object is assigned to this population. */
  void checkFigureOfMerit();

  double m_mutateRate; //!< Stores the mutate rate.
  bool m_sorted; //!< Stores whether the ranking is valid or needs to be redone.
  std::vector<IModel*> m_individuals; //!< Stores the individuals of this population.
  IFigureOfMerit *m_fom; //!< Stores the figure of merit to be used to calculate scores and perform the ranking.
  TRandom3 *m_random; //!< Stores a random number generator.
  double m_scoreMean; //!< Stores the mean score for the population.
  double m_scoreRMS; //!< Stores the score RMS for the population.
  std::vector<std::vector<IModel*> > m_parents; //!< Stores the list of parents about to be crossed-over.
};

#endif
