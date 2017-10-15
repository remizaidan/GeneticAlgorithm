#ifndef IMODEL_H
#define IMODEL_H

/**
 * @brief Abstract class describing the interface for a model.
 *
 * A model is an entity that can be scored to describe it's fittness to answer the solution to a given problem.
 * The implementation details are left for the derived classes.
 *
 * The only functionality provided in this interface is set/get accessors for the score.
 * The score calculation is expected to be delegated to a class inheriting from IFigureOfMerit.
 */
class IModel {

public:

  /** Default Constructor. */
  IModel();

  /** Destructor. */
  virtual ~IModel()=0;

  /** Returns the score for this model. */
  double getScore() const;

  /** Sets the score for this model. */
  void setScore(double score);
  
protected:

  double m_score; //!< Holds the score for this model.
};

#endif
