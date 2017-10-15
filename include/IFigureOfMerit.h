#ifndef IFIGUREOFMERIT_H
#define IFIGUREOFMERIT_H

class IModel;

/**
 * @brief Abstract class representing a figure of merit.
 *
 * A figure of merit should implement three functionalities:
 * - Evaluate the fittness of a model. The details of that evaluation is kept for the derive classes.
 * - Decide whether a model is good enough to be accepted as the final answer to a problem. 
 * The default behaviour is to apply a threshold on the score.
 * - Compares two models and decide which is the fittest. 
 * The default behaviour is that a larger score is better. 
 *
 * Deriving from this class:
 * - Derived classes should at least implement the `evaluate()` method if the score handling behavior is adequate.
 * - In addition, derived classes may reimplement `accept()` and/or `isBetterThan()` versions that take scores as input
 * to modify the handling of the scores, e.g. a lower score is may be better.
 * - For more complex cases where the decisions are not taken solely on the score, the derived class may
 * reimplement the overloaded versions of `accept()` and `isBetterThan()` that take a IModel as input. 
 * In this case, the fuction `evaluate()` may simply return a dummy value.
 * 
 */
class IFigureOfMerit {

public:

  /** Default Constructor */
  IFigureOfMerit();

  /** Destructor */
  virtual ~IFigureOfMerit();

  /**
   * @brief Evaluate the fittness of a model.
   *
   * Derived classes should implement this method.
   */
  virtual double evaluate(IModel *model) const =0;

  /** Decide if a model can be accepted as a final answer. */
  virtual bool accept(IModel *model) const;

  /** Compares two Models */
  virtual bool isBetterThan(IModel *scoreToTest, IModel *referenceModel) const;

  /** Sets the score threshold to accept a model as a final answer. */
  void setAcceptThreshold(double acceptThreshold);

  /** Returns the score threshold to accept a model as a final answer. */
  double getAcceptThreshold() const;

protected:
  
  /** Decide if a model's score can be accepted as a final answer. */
  virtual bool accept(double scrore) const;
  
  /** Compares two scores */
  virtual bool isBetterThan(double scoreToTest, double referenceScore) const;

  double m_acceptThreshold; //!< Stores the score threshold to accept a model as a final answer.
};

#endif
