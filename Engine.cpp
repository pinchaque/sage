#include "sage/Engine.h"

#ifndef INCLUDED_sage_Policy_h
#include "sage/Policy.h"
#endif

#ifndef INCLUDED_sage_State_h
#include "sage/State.h"
#endif

#ifndef INCLUDED_sage_Exception_h
#include "sage/Exception.h"
#endif

#ifndef INCLUDED_sage_BoardUtil_h
#include "sage/BoardUtil.h"
#endif

#ifndef INCLUDED_std_iostream
#include <iostream>
#define INCLUDED_std_iostream
#endif

namespace sage {

void Engine::run()
{ 

  std::cout << "Engine::run()" << std::endl;
  int turn = 1;

  while (m_game.getState() == STATE_ongoing)
  {
    // get list of legal moves
    MoveList moveList;
    BoardUtil::populateMoveList(m_game.getCurrentBoard(), moveList);

    int moveNum = 0;

    if (m_game.getCurrentBoard().getTurn() == Board::COLOR_white)
    {
      // get move from white
      moveNum = m_white.decide(m_game.getCurrentBoard(), moveList);
    }
    else // if (m_game.getCurrentBoard().getTurn() == Board::COLOR_black)
    {
      // get move from black
      moveNum = m_black.decide(m_game.getCurrentBoard(), moveList);
    }

    if ((moveNum >= (int) moveList.size()) || (moveNum < 0))
    {
      throw InvalidMoveException("Move number out of range");
    }

    // apply the move
    m_game.applyMove(moveList[moveNum]);
    
    std::cout << "Turn " << turn << " move: " << moveNum << std::endl;
    turn++;
  }
}


} // namespace sage

