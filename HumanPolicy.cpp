#include "sage/HumanPolicy.h"

#ifndef INCLUDED_sage_Board_h
#include "sage/Board.h"
#endif

#ifndef INCLUDED_sage_Move_h
#include "sage/Move.h"
#endif

#ifndef INCLUDED_std_cctype
#include <cctype>
#define INCLUDED_std_cctype
#endif

#ifndef INCLUDED_std_iostream
#include <iostream>
#define INCLUDED_std_iostream
#endif

#ifndef INCLUDED_std_iomanip
#include <iomanip>
#define INCLUDED_std_iomanip
#endif

namespace sage {

int HumanPolicy::decide(const Board& board, const MoveList& moveList)
{
  for (;;)
  {
    printBoard(board);

    printMoveList(moveList);

    for (int i = 0; i < 10; ++i)
    {
      std::string response = getResponse();
      int idx = atoi(response.c_str());

      if ((response == "0")
          || ((idx < static_cast<int>(moveList.size()))
              && (idx > 0)))
      {
        return idx;
      }
      else
      {
        std::cout << "**** Invalid response ('" 
                  << response
                  << "') ****"
                  << "\n";
      }
    }
  }
}

std::string HumanPolicy::getPieceName(Piece::Type type) const
{
  std::string ret("");

  if (type & static_cast<int>(Piece::PIECE_whiteAll))
  {
    ret += "W";
  }
  else if (type & static_cast<int>(Piece::PIECE_blackAll))
  {
    ret += "B";
  }

  if (type & static_cast<int>(Piece::PIECE_anyKing))
  {
    ret += "K";
  }
  else if (type & static_cast<int>(Piece::PIECE_anyQueen))
  {
    ret += "Q";
  }
  else if (type & static_cast<int>(Piece::PIECE_anyRook))
  {
    ret += "R";
  }
  else if (type & static_cast<int>(Piece::PIECE_anyBishop))
  {
    ret += "B";
  }
  else if (type & static_cast<int>(Piece::PIECE_anyKnight))
  {
    ret += "N";
  }
  else if (type & static_cast<int>(Piece::PIECE_anyPawn))
  {
    ret += "P";
  }

  return ret;
}

namespace {
  char getFill(int col, int row)
  {
    if ((col + row) % 2)
    {
      return '.';
    }
    else
    {
      return ' ';
    }
  }
} // anonymous namespace 

void HumanPolicy::printBoard(const Board& board) const
{
  int squareWidth = 6;

  std::cout << std::setfill('-') 
            << std::setw(squareWidth * Board::NUM_COLUMNS + 5)
            << "-"
            << "\n";

  for (int j = 0; j < Board::NUM_ROWS; ++j)
  {
    std::cout << "     ";
    for (int i = 0; i < Board::NUM_COLUMNS; ++i)
    {
      std::cout << std::setfill(getFill(i, j))
                << std::setw(squareWidth)
                << "";
    }
    std::cout << "\n";

    std::cout << "     ";
    for (int i = 0; i < Board::NUM_COLUMNS; ++i)
    {
      std::cout << std::setfill(getFill(i, j))
                << std::setw(squareWidth)
                << "";
    }
    std::cout << "\n";

    std::cout << "  " 
              << std::setw(3)
              << std::setfill(' ')
              << std::left
              << (Board::NUM_ROWS - j);
    for (int i = 0; i < Board::NUM_COLUMNS; ++i)
    {
      std::cout << std::setfill(getFill(i, j))
                << std::setw((squareWidth - 2) / 2)
                << "";

      const sage::Piece& piece = board.getPiece(i, (Board::NUM_ROWS - j - 1));
      std::cout << std::setfill(getFill(i, j))
                << std::setw(2)
                << getPieceName(piece.getType());

      std::cout << std::setfill(getFill(i, j))
                << std::setw((squareWidth - 2) / 2)
                << "";
    }
    std::cout << "\n";

    std::cout << "     ";
    for (int i = 0; i < Board::NUM_COLUMNS; ++i)
    {
      std::cout << std::setfill(getFill(i, j))
                << std::setw(squareWidth)
                << "";
    }
    std::cout << "\n";
  }

  std::cout << "     ";
  const char* letters = "abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    std::cout << std::setfill(' ')
              << std::setw((squareWidth - 2) / 2)
              << "";
    
    std::cout << std::setfill(' ')
              << std::setw(2)
              << std::left
              << letters[i];
    
    std::cout << std::setfill(' ')
              << std::setw((squareWidth - 2) / 2)
              << "";
  }
  std::cout << "\n";

  std::cout << std::setfill('-') 
            << std::setw(squareWidth * Board::NUM_COLUMNS + 5)
            << "-"
            << "\n\n";
}

void HumanPolicy::printMove(const Move& move) const
{
  const char* letters = "abcdefghijklmnopqrstuvwxyz";

  std::cout << letters[move.getStartColumn()]
            << move.getStartRow() + 1
            << (move.getCapture() ? 'x' : '-')
            << letters[move.getEndColumn()]
            << move.getEndRow() + 1;
}

void HumanPolicy::printMoveList(const MoveList& moveList) const
{
  const int MOVES_PER_LINE = 5;

  for (int i = 0; i < static_cast<int>(moveList.size()); ++i)
  {
    // print move index
    std::cout << "[" 
              << std::setw(2)
              << std::setfill(' ')
              << i
              << "] ";

    printMove(moveList[i]);
    
    std::cout << "   ";

    // print newline
    if (!((i + 1) % MOVES_PER_LINE))
    {
      std::cout << "\n";
    }
  }
  std::cout << "\n";
}

std::string HumanPolicy::getResponse()
{
  std::string response;

  std::cout << "Enter move: ";
  std::cin >> response;
  return response;
}

} // namespace sage
