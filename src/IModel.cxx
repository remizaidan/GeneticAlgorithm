#include "IModel.h"

IModel::IModel()
{
  
  m_score = 0;
}

IModel::~IModel()
{
}

/**
 * @return The score for this model.
 */
double IModel::getScore() const {

  return m_score;
}

/**
 * @param score The score for this model.
 */
void IModel::setScore(double score)
{
  
  m_score = score;
}
  
