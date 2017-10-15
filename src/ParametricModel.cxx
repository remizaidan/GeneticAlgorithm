#include "ParametricModel.h"

#include <stdexcept>
#include <sstream>

ParametricModel::ParametricModel() :
  IModel()
{
  m_formula=0;
}

ParametricModel::~ParametricModel()
{
  // The formula is a clone that we own.
  if(m_formula) delete m_formula;
}

/**
 * @param formula A <a href="https://root.cern.ch/doc/v610/classTF1.html">TF1</a> formula.
 *
 * This function keeps a clone of the formula in order to make sure that modifying it later
 * does not affect other instances that might share the same original formula. 
 */
void ParametricModel::setFormula(TF1 *formula)
{

  if(m_formula) delete m_formula;
  m_formula = (TF1*)formula->Clone();
}

/**
 * @return a pointer to the <a href="https://root.cern.ch/doc/v610/classTF1.html">TF1</a> 
 * formula for this model.
 */
TF1 *ParametricModel::getFormula()
{
  return m_formula;
}


/**
 * @return a const pointer to the <a href="https://root.cern.ch/doc/v610/classTF1.html">TF1</a> 
 * formula for this model.
 */
const TF1 *ParametricModel::getFormula() const
{
  return m_formula;
}


			     
