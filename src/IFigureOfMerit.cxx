#include "IFigureOfMerit.h"

#include "IModel.h"

IFigureOfMerit::IFigureOfMerit()
{
  m_acceptThreshold = 0.;
}

IFigureOfMerit::~IFigureOfMerit()
{
}

/**
 * The default behavior is to apply a threshold on the score.
 *
 * Derived classes can override this method if the default behaviour is not adequate.
 *
 * @param score The score of the model to be tested.
 * @return true if the score can be accepted.
 */
bool IFigureOfMerit::accept(double score) const
{  
  return isBetterThan(score, m_acceptThreshold);
}

/**
 * This is an overloaded function.
 * The default behavior is to apply a threshold on the score.
 *
 * Derived classes can override this method if the decision is not based solely on the score.
 *
 * @param model The model to be tested.
 * @return true if the model can be accepted.
 */
bool IFigureOfMerit::accept(IModel *model) const
{
  return accept(model->getScore());
}

/**
 * The default behavior is: higher score is better.
 *
 * Derived classes can override this method if the default behaviour is not adequate.
 *
 * @param scoreToTest The score of the model to be tested.
 * @param referenceScore The score of the model to compare to.
 * @return true if scoreToTest is better than referenceScore, false otherwise.
 */
bool IFigureOfMerit::isBetterThan(double scoreToTest, double referenceScore) const
{
  return scoreToTest > referenceScore;
}

/**
 * This is an overloaded function.
 * The default behavior is: higher score is better.
 *
 * Derived classes can override this method if the comparison is not based solely on the score.
 *
 * @param modelToTest The model to be tested.
 * @param referenceModel The model to compare to.
 * @return true if modelToTest is better than referenceModel, false otherwise.
 */
bool IFigureOfMerit::isBetterThan(IModel *modelToTest, IModel *referenceModel) const
{
  return isBetterThan(modelToTest->getScore(), referenceModel->getScore());
}

/**
 * @param threshold score threshold to accept a model as a final answer.
 */
void IFigureOfMerit::setAcceptThreshold(double threshold)
{
  m_acceptThreshold = threshold;
}

/**
 * @return the score threshold to accept a model as a final answer.
 */
double IFigureOfMerit::getAcceptThreshold() const
{
  return m_acceptThreshold;
}
