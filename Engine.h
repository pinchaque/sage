#ifndef INCLUDED_sage_Engine_h
#define INCLUDED_sage_Engine_h

#ifndef INCLUDED_sage_Game_h
#include "sage/Game.h"
#endif

#ifndef INCLUDED_sage_Board_h
#include "sage/Board.h"
#endif

namespace sage {

class Policy;

/*!
  \brief
*/
class Engine
{
 public:

  /*!
    \brief Constructor: Build game engine with given policies and start
    position
    \param white The policy to use for white
    \param black The policy to use for black
    \param board The starting position

    It is assumed that the board passed in here is fully constructed and
    is valid (i.e. has the state set properly).
  */
  Engine(Policy& white, Policy& black, const Board& board)
    : m_white(white), m_black(black), m_game(board)
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~Engine()
  {
    ;
  }

  /*!
    \brief Returns policy used by white
  */
  const Policy& getWhitePolicy() const { return m_white; }

  /*!
    \brief Returns policy used by black
  */
  const Policy& getBlackPolicy() const { return m_black; }

  /*!
    \brief Returns the game state
  */
  const Game& getGame() const { return m_game; }

  /*!
    \brief Runs the game

    When called, this method will alternately call Policy::decide() on
    the white and black policies to iterate through the entire game. Any
    user interaction should be set up through the derived policy classes.
    This method will stop once the game has reached a terminal state.
  */
  void run();

 private:

  //! Move decision policy to use for white
  Policy& m_white;

  //! Move decision policy to use for black
  Policy& m_black;

  //! The ongoing chess game
  Game m_game;
};

} // namespace sage

#endif


