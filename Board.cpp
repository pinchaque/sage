#include "sage/Board.h"

#ifndef INCLUDED_sage_Exception_h
#include "sage/Exception.h"
#endif

namespace sage {

Board::Board()
  : m_whiteQueenCastle(true), m_whiteKingCastle(true),
    m_blackQueenCastle(true), m_blackKingCastle(true),
    m_enPassantCol(-1), m_turnColor(Board::COLOR_white)
{
  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    for (int j = 0; j < Board::NUM_ROWS; ++j)
    {
      m_squares[i][j] = Piece(i, j, Piece::PIECE_none);
    }
  }
}

Board::~Board()
{

}

void Board::applyMove(const Move& move)
{
  // This is the piece that's moving
  Piece movingPiece = m_squares[move.getStartColumn()][move.getStartRow()];

  // This is the piece we're capturing (possibly none)
  const Piece& capturePiece
    = m_squares[move.getEndColumn()][move.getEndRow()];

  // Do some rudimentary checking to make sure the move "makes sense"
  if (move.getPiece() != movingPiece)
  {
    throw InvalidMoveException("Moving piece does not match start space");
  }
  
  if (move.getCapture() && (capturePiece.getType() == Piece::PIECE_none))
  {
    throw InvalidMoveException("Move is not capturing anything");
  }

  if ((movingPiece.getType() & static_cast<int>(Piece::PIECE_whiteAll))
      && (m_turnColor != Board::COLOR_white))
  {
    throw InvalidMoveException("Moving white piece on black's turn");
  }

  if ((movingPiece.getType() & static_cast<int>(Piece::PIECE_blackAll))
      && (m_turnColor != Board::COLOR_black))
  {
    throw InvalidMoveException("Moving black piece on white's turn");
  }

  // white castles kingside
  if ((movingPiece.getType() == Piece::PIECE_whiteKing)
      && (move.getStartRow() == 0)
      && (move.getEndRow() == 0)
      && (move.getStartColumn() == 4)
      && (move.getEndColumn() == 6))
  {
    m_squares[4][0] = Piece(4, 0, Piece::PIECE_none);
    m_squares[5][0] = Piece(5, 0, Piece::PIECE_whiteRook);
    m_squares[6][0] = Piece(6, 0, Piece::PIECE_whiteKing);
    m_squares[7][0] = Piece(7, 0, Piece::PIECE_none);
  }
  // white castles queenside
  else if ((movingPiece.getType() == Piece::PIECE_whiteKing)
           && (move.getStartRow() == 0)
           && (move.getEndRow() == 0)
           && (move.getStartColumn() == 4)
           && (move.getEndColumn() == 2))
  {
    m_squares[4][0] = Piece(4, 0, Piece::PIECE_none);
    m_squares[3][0] = Piece(3, 0, Piece::PIECE_whiteRook);
    m_squares[2][0] = Piece(2, 0, Piece::PIECE_whiteKing);
    m_squares[0][0] = Piece(0, 0, Piece::PIECE_none);
  }
  else if ((movingPiece.getType() == Piece::PIECE_blackKing)
      && (move.getStartRow() == (Board::NUM_ROWS - 1))
      && (move.getEndRow() == (Board::NUM_ROWS - 1))
      && (move.getStartColumn() == 4)
      && (move.getEndColumn() == 6))
  {
    m_squares[4][Board::NUM_ROWS - 1]
      = Piece(4, (Board::NUM_ROWS - 1), Piece::PIECE_none);
    m_squares[5][Board::NUM_ROWS - 1]
      = Piece(5, (Board::NUM_ROWS - 1), Piece::PIECE_blackRook);
    m_squares[6][Board::NUM_ROWS - 1]
      = Piece(6, (Board::NUM_ROWS - 1), Piece::PIECE_blackKing);
    m_squares[7][Board::NUM_ROWS - 1]
      = Piece(7, (Board::NUM_ROWS - 1), Piece::PIECE_none);
  }
  // black castles queenside
  else if ((movingPiece.getType() == Piece::PIECE_blackKing)
           && (move.getStartRow() == (Board::NUM_ROWS - 1))
           && (move.getEndRow() == (Board::NUM_ROWS - 1))
           && (move.getStartColumn() == 4)
           && (move.getEndColumn() == 2))
  {
    m_squares[4][Board::NUM_ROWS - 1]
      = Piece(4, (Board::NUM_ROWS - 1), Piece::PIECE_none);
    m_squares[3][Board::NUM_ROWS - 1]
      = Piece(3, (Board::NUM_ROWS - 1), Piece::PIECE_blackRook);
    m_squares[2][Board::NUM_ROWS - 1]
      = Piece(2, (Board::NUM_ROWS - 1), Piece::PIECE_blackKing);
    m_squares[0][Board::NUM_ROWS - 1]
      = Piece(0, (Board::NUM_ROWS - 1), Piece::PIECE_none);
  }
  else
  {
    // Make the move
    m_squares[move.getStartColumn()][move.getStartRow()] 
      = Piece(move.getStartColumn(), move.getStartRow(), Piece::PIECE_none);
    
    m_squares[move.getEndColumn()][move.getEndRow()] 
      = Piece(move.getEndColumn(), move.getEndRow(), movingPiece.getType());
  }

  // adjust en passant square
  adjustEnPassant(move);

  // adjust castling flags
  adjustCastling(move);

  // switch whose turn it is
  switchTurn();
}

PieceList Board::getPieceList() const
{
  PieceList pieceList;

  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    for (int j = 0; j < Board::NUM_ROWS; ++j)
    {
      if (m_squares[i][j].getType() != Piece::PIECE_none)
      {
        pieceList.push_back(m_squares[i][j]);
      }
    }
  }

  return pieceList;
}

void Board::adjustEnPassant(const Move& move)
{
  // This is the piece that's moving
  Piece movingPiece = m_squares[move.getStartColumn()][move.getStartRow()];

  if ((movingPiece.getType() == Piece::PIECE_whitePawn)
      && (move.getStartRow() == 1)
      && (move.getEndRow() == 3))
  {
    m_enPassantCol = move.getStartColumn();
  }
  else if ((movingPiece.getType() == Piece::PIECE_blackPawn)
      && (move.getStartRow() == 6)
      && (move.getEndRow() == 4))
  {
    m_enPassantCol = move.getStartColumn();
  }
  else
  {
    m_enPassantCol = -1;
  }
}


void Board::adjustCastling(const Move& move)
{
  if (m_turnColor == Board::COLOR_white)
  {
    if (m_squares[0][0].getType() != Piece::PIECE_whiteRook)
    {
      m_whiteQueenCastle = false;
    }

    if (m_squares[NUM_COLUMNS - 1][0].getType() != Piece::PIECE_whiteRook)
    {
      m_whiteKingCastle = false;
    }

    if (m_squares[4][0].getType() != Piece::PIECE_whiteKing)
    {
      m_whiteKingCastle = false;
      m_whiteQueenCastle = false;
    }
  }
  else
  {
    if (m_squares[0][NUM_ROWS - 1].getType() != Piece::PIECE_blackRook)
    {
      m_blackQueenCastle = false;
    }

    if (m_squares[NUM_COLUMNS - 1][NUM_ROWS - 1].getType() 
        != Piece::PIECE_blackRook)
    {
      m_blackKingCastle = false;
    }

    if (m_squares[4][NUM_ROWS - 1].getType() != Piece::PIECE_blackKing)
    {
      m_blackKingCastle = false;
      m_blackQueenCastle = false;
    }
  }
}


void Board::switchTurn()
{
  if (m_turnColor == Board::COLOR_white)
  {
    m_turnColor = Board::COLOR_black;
  }
  else
  {
    m_turnColor = Board::COLOR_white;
  }
}

} // namespace sage
