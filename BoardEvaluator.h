#ifndef INCLUDED_sage_BoardEvaluator_h
#define INCLUDED_sage_BoardEvaluator_h

namespace sage {

class Board;

/*!
  \brief Interface class for board evaluation function
*/
class BoardEvaluator
{
 public:

  /*!
    \brief Default constructor
  */
  BoardEvaluator()
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~BoardEvaluator()
  {
    ;
  }

  /*!
    \brief Evaluates the given board
    \param board The board to evaluate
    \return A number in the range [-1.0, 1.0]

    Positive numbers indicate an advantage for white; negative numbers
    an advantage for black. The larger the absolute value of the number,
    the larger the advantage. A value of 1.0 means that white can force
    a win; -1.0 means black can force a win.
  */
  virtual double evaluate(const Board& board) = 0;

 private:
};

} // namespace sage

#endif
