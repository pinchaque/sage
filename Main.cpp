
#include "sage/Piece.h"
#include "sage/Board.h"
#include "sage/Move.h"
#include "sage/Game.h"
#include "sage/Exception.h"
#include "sage/BoardEvaluator.h"
#include "sage/Policy.h"
#include "sage/RandomPolicy.h"
#include "sage/HumanPolicy.h"
#include "sage/Engine.h"
#include "sage/State.h"

int main(int argc, char** argv)
{
  // get board starting position
  sage::Board board;
  sage::BoardUtil::initializeBoard(board);

  sage::HumanPolicy whitePolicy;
  sage::HumanPolicy blackPolicy;

  sage::Engine gameEngine(whitePolicy, blackPolicy, board);

  gameEngine.run();
  return 0;
}
