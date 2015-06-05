#ifndef INCLUDED_sage_Policy_h
#define INCLUDED_sage_Policy_h

#ifndef INCLUDED_sage_Board_h
#include "sage/Board.h"
#endif

#ifndef INCLUDED_sage_Move_h
#include "sage/Move.h"
#endif

namespace sage {

/*!
  \brief Interface class for a chess decision policy.

  A decision policy is essentially what move to take given a board and
  a list of valid moves. The policy will look at the board and choose among
  the moves using some internally defined metric.
*/
class Policy
{
 public:

  /*!
    \brief Default constructor
  */
  Policy()
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~Policy()
  {
    ;
  }

  /*!
    \brief Decides which move to take for the given board
    \param board The board on which we are making the move
    \param moveList The list of moves to consider.
    \return The index of move within moveList

    This method assumes all moves in moveList are valid moves for the
    given board. 
  */
  virtual int decide(const Board& board, const MoveList& moveList) = 0;

 private:
};

} // namespace sage

#endif
