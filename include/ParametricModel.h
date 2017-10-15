#ifndef PARAMETRICMODEL_H
#define PARAMETRICMODEL_H

#include "IModel.h"

#include <TF1.h>

/**
 * @brief Class representing a model defined by a parametric function.
 *
 * The implementation of this parametric model is a simple wrapper around ROOT's powerful 
 * <a href="https://root.cern.ch/doc/v610/classTF1.html">TF1</a>
 * formula class which allows to describe any function in any number of dimensions 
 * and depending on any number of parameters.
 *
 * Another desired feature of <a href="https://root.cern.ch/doc/v610/classTF1.html">TF1</a>, 
 * is that it allows plotting 1D and 2D formulas with little extra code.
 */
class ParametricModel : public IModel {

public:

  /** Default Constructor */
  ParametricModel();

  /** Destructor */
  virtual ~ParametricModel();

  /** Sets the formula for this model. */
  void setFormula(TF1 *formula);

  /** Returns the formula for this model. */
  TF1 *getFormula();
  
  /** Returns the formula for this model. */
  const TF1 *getFormula() const;
  
protected:
  
  TF1 *m_formula; //!< Holds the formula for this model.
};

#endif
