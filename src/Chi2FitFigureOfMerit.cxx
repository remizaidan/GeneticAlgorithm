#include "Chi2FitFigureOfMerit.h"

#include "ParametricModel.h"

#include <stdexcept>


Chi2FitFigureOfMerit::Chi2FitFigureOfMerit() :
  IFigureOfMerit()
{
  setAcceptThreshold(0.1);
}

Chi2FitFigureOfMerit::~Chi2FitFigureOfMerit()
{
}

/**
 * @param x \f$\vec{x}\f$ coordinate.
 * @param y \f$y\f$ coordinate.
 * @param ey \f$\sigma_y\f$ error on \f$y\f$ coordinate.
 */
void Chi2FitFigureOfMerit::addData(const std::vector<double> &x, double y, double ey)
{
  m_x.push_back(x);
  m_y.push_back(y);
  m_ey.push_back(ey);
}

void Chi2FitFigureOfMerit::clearData()
{
  m_x.clear();
  m_y.clear();
  m_ey.clear();
}

/**
 * @param imodel Model to be evaluated.
 * @return \f$\chi^2/ndf\f$ score.
 *
 * The \f$\chi^2/ndf\f$ score is then defined as:
 * \f[
 * \chi^2/ndf = \frac{1}{N}\sum_{i=0}^N \frac{(y_i - f(\vec{x_i}))^2}{\sigma_{y_i}^2}
 * \f]
 */
double Chi2FitFigureOfMerit::evaluate(IModel *imodel) const
{

  ParametricModel *model = dynamic_cast<ParametricModel*>(imodel);
  if(!model) {
    throw std::runtime_error("Given model is not a parametric model");
  }
  
  if(m_x.size() == 0) return 0;

  double chi2 = 0;
  int ndf = 0;
  for(unsigned int i=0; i<m_x.size(); i++) {
    double y = m_y[i];
    if(y == 0) continue;
    const std::vector<double> &x = m_x[i];
    double ey = m_ey[i];
    double fx = model->getFormula()->EvalPar(x.data());
    chi2 += (fx - y)*(fx - y)/(ey*ey);
    ndf++;
  }

  return chi2 / ndf;
}

/**
 * @param scoreToTest Score value to be tested.
 * @param referenceScore Score value to be compared to.
 * @return true if scoreToTest < referenceScore
 */
bool Chi2FitFigureOfMerit::isBetterThan(double scoreToTest, double referenceScore) const
{
  return scoreToTest < referenceScore;
}
