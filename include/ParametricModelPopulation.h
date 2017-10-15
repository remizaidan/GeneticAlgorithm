#ifndef PARAMETRICMODELPOPULATION_H
#define PARAMETRICMODELPOPULATION_H

#include "IPopulation.h"

#include <TF1.h>

/**
 * @brief Implements a population of parametric models.
 *
 * The following behavior is implemented:
 * - Initialization: parameters are randomly initialized following uniform distribution in the allowed range.
 * - Cross-over: each parameter is passed from either parents chosen at random.
 * - Mutation: a random parameter is chosen and then modified by adding a gaussian noise component.
 */
class ParametricModelPopulation : public IPopulation
{

public:

  /** Default Constructor. */
  ParametricModelPopulation();

  /** Destructor. */
  ~ParametricModelPopulation();

  /** Sets the formula for this population. */
  void setFormula(TF1 *formula);

  /** Sets the relative size (sigma) of the gaussian noise applied during mutation. */
  void setMutationSize(double relativeSize);
  
protected:

  /** Implements initialization. */
  virtual void doInitialize(int n);

  /** Implements cross-over. */
  virtual void doCrossOver(const std::vector<std::vector<IModel*> > &parents);

  /** Implements mutation. */
  virtual void doMutate(IModel *model);
  
  TF1 *m_formula; //!< Stores the formula for this population.
  double m_mutationSize; //!< Stores the relative size (sigma) of the gaussian noise applied during mutation.
};

#endif
