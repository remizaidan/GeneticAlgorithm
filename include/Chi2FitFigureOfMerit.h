#ifndef CHI2FITFIGUREOFMERIT_H
#define CHI2FITFIGUREOFMERIT_H

#include "IFigureOfMerit.h"

#include <vector>

class IModel;

/**
 * @brief Class implementing a \f$\chi^2/ndf\f$ figure of merit.
 *
 * The \f$\chi^2/ndf\f$ is computed by comparing a model to a dataset:
 * - The data consists of \f$(\vec{x_i}, y_i)\f$ pairs with associated errors \f$\sigma_{y_i}\f$ on \f$y_i\f$.
 * - The model is a function of the form \f$y = f(\vec{x})\f$.
 * - The figure of merit is then defined as:
 * \f[
 * \chi^2/ndf = \frac{1}{N}\sum_{i=0}^N \frac{(y_i - f(\vec{x_i}))^2}{\sigma_{y_i}^2}
 * \f]
 */
class Chi2FitFigureOfMerit : public IFigureOfMerit {

public:

  /** Default Constructor */
  Chi2FitFigureOfMerit();

  /** Destructor */
  ~Chi2FitFigureOfMerit();

  /** Adds a data point */
  void addData(const std::vector<double> &x, double y, double ey);

  /** Clear all data */
  void clearData();

  /** Compute the score (\f$\chi^2/ndf\f$) for a given model relative to the data points. */
  double evaluate(IModel *model) const;

  /** Compares two score values */
  bool isBetterThan(double scoreToTest, double referenceScore) const;

protected:

  std::vector<std::vector<double> > m_x; //!< Stores the \f$\vec{x_i}\f$ coordinates.
  std::vector<double> m_y; //!< Stores the \f$y_i\f$ coordinates.
  std::vector<double> m_ey; //!< Stores the \f$\sigma_{y_i}\f$ errors.
};

#endif
