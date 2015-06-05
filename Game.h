#ifndef INCLUDED_sage_Game_h
#define INCLUDED_sage_Game_h

#ifndef INCLUDED_sage_Board_h
#include "sage/Board.h"
#endif

#ifndef INCLUDED_sage_BoardUtil_h
#include "sage/BoardUtil.h"
#endif

#ifndef INCLUDED_sage_Move_h
#include "sage/Move.h"
#endif

#ifndef INCLUDED_sage_State_h
#include "sage/State.h"
#endif

namespace sage {

/*!
  \brief A chess game

  A chess game consists of a starting position (Board), a series of moves,
  and a current position.

  Note that the state is not figured out by this class, but must be 
  calculated and set externally. Therefore it is possible to have this
  object in an inconsistent state--e.g. where the contained board is drawn
  but the state is STATE_ongoing. It is the responsibility of the objects
  manipulating this class to enforce consistent state.
*/
class Game
{
 public:

  /*!
    \brief Initializes a game with the given starting position
    \param board The initial board position
    
    Initially, the initial and current boards are set to the same value
    and the move list is empty.
  */
  Game(const Board& board)
    : m_initialBoard(board), m_currentBoard(board), m_moves(), 
    m_state(BoardUtil::calculateState(board))
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~Game()
  {
    ;
  }

  /*! 
    \brief Makes the specified move on the board
    \param The move

    Refer to Board::applyMove for a description of the error handling for
    this method. The move will be appended to the move list for this 
    game only if successfully made.
  */
  void applyMove(const Move& move)
  {
    // This will throw an error if there's a problem
    m_currentBoard.applyMove(move);

    // Add move to list
    m_moves.push_back(move);

    // Update the board state
    m_state = BoardUtil::calculateState(m_currentBoard);
  }

  /*!
    \brief Returns the list of moves
  */
  const MoveList& getMoveList() const { return m_moves; }

  /*!
    \brief Retruns the initial board
  */
  const Board& getInitialBoard() const { return m_initialBoard; }

  /*!
    \brief Returns the current board
  */
  const Board& getCurrentBoard() const { return m_currentBoard; }

  /*!
    \brief Returns the current state
  */
  State getState() const { return m_state; }

  /*!
    \brief Sets the state of the board
  */
  void setState(State state) { m_state = state; }

 private:
  // Default constructor not defined
  Game();

  //! The initial board position. This is not modified after construction
  Board m_initialBoard;

  /*!
    The current board position. This is modified as moves are made in the
    game. One can reconstruct this board by applying m_moves to 
    m_initialBoard. This is time consuming, so we save the current board.
  */
  Board m_currentBoard;
  
  //! The vector of moves taking place in this game.
  MoveList m_moves;

  //! The state of the board
  State m_state;
};

} // namespace sage

#endif
