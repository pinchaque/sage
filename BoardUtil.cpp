#include "sage/BoardUtil.h"

namespace sage {

void BoardUtil::populateMoveList(const Board& board, MoveList& moveList)
{
  moveList.clear();

  MoveList tempMoveList;

  int pieceMask = ((board.getTurn() == Board::COLOR_white)
                   ? Piece::PIECE_whiteAll
                   : Piece::PIECE_blackAll);

  PieceList pieceList = board.getPieceList();

  // keep track of all squares being attacked by opponent
  AttackMatrix isAttacked;
  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    for (int j = 0; j < Board::NUM_ROWS; ++j)
    {
      isAttacked[i][j] = 0;
    }
  }

  // find my king on board. used to determine if I'm still in check after
  // I make a move
  Piece myKing;
  for (PieceList::const_iterator iter = pieceList.begin();
       iter != pieceList.end();
       ++iter)
  {
    if ((iter->getType() & pieceMask)
        && (iter->getType() & static_cast<int>(Piece::PIECE_anyKing)))
    {
      myKing = *iter;
      break;
    }
  }

  // iterate through all pieces on board
  for (PieceList::const_iterator iter = pieceList.begin();
       iter != pieceList.end();
       ++iter)
  {
    if (iter->getType() == Piece::PIECE_none)
    {
      continue;
    }

    // populate isattacked for pieces of opposite color
    if (!(iter->getType() & pieceMask))
    {
      isAttacked[iter->getColumn()][iter->getRow()]++;
      continue;
    }

    if (iter->getType() & static_cast<int>(Piece::PIECE_anyKing))
    {
      populateKingAttacks(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyQueen))
    {
      populateRookAttacks(*iter, board, tempMoveList);
      populateBishopAttacks(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyRook))
    {
      populateRookAttacks(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyBishop))
    {
      populateBishopAttacks(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyKnight))
    {
      populateKnightAttacks(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_whitePawn))
    {
      populateWhitePawnMoves(*iter, board, tempMoveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_blackPawn))
    {
      populateBlackPawnMoves(*iter, board, tempMoveList);
    }
  }

  // filter out any illegal moves
  for (MoveList::iterator iter = tempMoveList.begin();
       iter != tempMoveList.end();
       ++iter)
  {
    int col = iter->getEndColumn();
    int row = iter->getEndRow();

    // is this move a capture?
    Piece capturePiece = board.getPiece(col, row);
    if (capturePiece.getType() != Piece::PIECE_none)
    {
      // can't capture own pieces!
      if (capturePiece.getType() & pieceMask)
      {
        continue;
      }
      else
      {
        iter->setCapture(true);
      }
    }

    // can't put own king in check!
    // create temporary board, make this move on it, and check if we're in
    // check still
    if (moveInducesCheck(*iter, board, myKing))
    {
      continue;
    }

    // add move to returned move list
    moveList.push_back(*iter);
  }

  // get castling moves
  populateCastle(board, isAttacked, moveList);

}

void BoardUtil::populateAttackList(const Board& board, 
                                   const Board::Color color, 
                                   MoveList& moveList)
{
  moveList.clear();

  int pieceMask = ((color == Board::COLOR_white)
                   ? Piece::PIECE_whiteAll
                   : Piece::PIECE_blackAll);

  PieceList pieceList = board.getPieceList();

  // iterate through all pieces on board
  for (PieceList::const_iterator iter = pieceList.begin();
       iter != pieceList.end();
       ++iter)
  {
    // skip pieces of wrong color
    if (!(iter->getType() & pieceMask))
    {
      continue;
    }

    if (iter->getType() & static_cast<int>(Piece::PIECE_anyKing))
    {
      populateKingAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyQueen))
    {
      populateRookAttacks(*iter, board, moveList);
      populateBishopAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyRook))
    {
      populateRookAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyBishop))
    {
      populateBishopAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_anyKnight))
    {
      populateKnightAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_whitePawn))
    {
      populateWhitePawnAttacks(*iter, board, moveList);
    }
    else if (iter->getType() & static_cast<int>(Piece::PIECE_blackPawn))
    {
      populateBlackPawnAttacks(*iter, board, moveList);
    }
  }
}

void BoardUtil::populateKingAttacks(const Piece& piece,
                                    const Board& board, 
                                    MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);


  move.setEndColumn(col);
  if (row > 0)
  {
    // Moving king to S
    move.setEndRow(row - 1);
    moveList.push_back(move);
  }
  
  if (row < (Board::NUM_ROWS - 1))
  {
    // Moving king to N
    move.setEndRow(row + 1);
    moveList.push_back(move);
  }

  move.setEndRow(row);
  if (col > 0)
  {
    // Moving king to W
    move.setEndColumn(col - 1);
    moveList.push_back(move);

    if (row > 0)
    {
      // Moving king to SW
      move.setEndRow(row - 1);
      moveList.push_back(move);
    }
    
    if (row < (Board::NUM_ROWS - 1))
    {
      // Moving king to NW
      move.setEndRow(row + 1);
      moveList.push_back(move);
    }
  }

  move.setEndRow(row);
  if (col < (Board::NUM_COLUMNS - 1))
  {
    // Moving king to E
    move.setEndColumn(col + 1);
    moveList.push_back(move);

    if (row > 0)
    {
      // Moving king to SE
      move.setEndRow(row - 1);
      moveList.push_back(move);
    }
    
    if (row < (Board::NUM_ROWS - 1))
    {
      // Moving king to SW
      move.setEndRow(row + 1);
      moveList.push_back(move);
    }
  }
}


void BoardUtil::populateRookAttacks(const Piece& piece,
                                    const Board& board, 
                                    MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);
  move.setEndRow(row);

  // Add moves to the EAST
  for (int i = col + 1; i < Board::NUM_COLUMNS; ++i)
  {
    move.setEndColumn(i);
    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  // Add moves to the WEST
  for (int i = col - 1; i >= 0; --i)
  {
    move.setEndColumn(i);
    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  move.setEndColumn(col);

  // Add moves to the NORTH
  for (int i = row + 1; i < Board::NUM_ROWS; ++i)
  {
    move.setEndRow(i);
    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  // Add moves to the SOUTH
  for (int i = row - 1; i >= 0; --i)
  {
    move.setEndRow(i);
    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }
}

void BoardUtil::populateBishopAttacks(const Piece& piece,
                                      const Board& board, 
                                      MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);

  // Add moves to the NORTHEAST
  for (int i = col + 1, j = row + 1; 
       (i < Board::NUM_COLUMNS) && (j < Board::NUM_ROWS);
       ++i, ++j)
  {
    move.setEndColumn(i);
    move.setEndRow(j);

    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  // Add moves to the NORTHWEST
  for (int i = col - 1, j = row + 1; 
       (i >= 0) && (j < Board::NUM_ROWS);
       --i, ++j)
  {
    move.setEndColumn(i);
    move.setEndRow(j);

    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  // Add moves to the SOUTHEAST
  for (int i = col + 1, j = row - 1; 
       (i < Board::NUM_COLUMNS) && (j >= 0);
       ++i, --j)
  {
    move.setEndColumn(i);
    move.setEndRow(j);

    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }

  // Add moves to the SOUTHWEST
  for (int i = col - 1, j = row - 1; 
       (i >= 0) && (j >= 0);
       --i, --j)
  {
    move.setEndColumn(i);
    move.setEndRow(j);

    moveList.push_back(move);
    if (board.getPiece(move.getEndColumn(), move.getEndRow()).getType()
        != Piece::PIECE_none)
    {
      break;
    }
  }
}

void BoardUtil::populateKnightAttacks(const Piece& piece,
                                      const Board& board, 
                                      MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);

  if (col > 0)
  {
    if (row > 1)
    {
      // Knight move to SSW
      move.setEndColumn(col - 1);
      move.setEndRow(row - 2);
      moveList.push_back(move);
    }

    if (row < (Board::NUM_ROWS - 2))
    {
      // Knight move to NNW
      move.setEndColumn(col - 1);
      move.setEndRow(row + 2);
      moveList.push_back(move);
    }
  }

  if (col > 1)
  {
    if (row > 0)
    {
      // Knight move to WWS
      move.setEndColumn(col - 2);
      move.setEndRow(row - 1);
      moveList.push_back(move);
    }

    if (row < (Board::NUM_ROWS - 1))
    {
      // Knight move to WWN
      move.setEndColumn(col - 2);
      move.setEndRow(row + 1);
      moveList.push_back(move);
    }
  }

  if (col < (Board::NUM_COLUMNS - 1))
  {
    if (row > 1)
    {
      // Knight move to SSE
      move.setEndColumn(col + 1);
      move.setEndRow(row - 2);
      moveList.push_back(move);
    }

    if (row < (Board::NUM_ROWS - 2))
    {
      // Knight move to NNE
      move.setEndColumn(col + 1);
      move.setEndRow(row + 2);
      moveList.push_back(move);
    }
  }

  if (col < (Board::NUM_COLUMNS - 2))
  {
    if (row > 0)
    {
      // Knight move to EES
      move.setEndColumn(col + 2);
      move.setEndRow(row - 1);
      moveList.push_back(move);
    }

    if (row < (Board::NUM_ROWS - 1))
    {
      // Knight move to EEN
      move.setEndColumn(col + 2);
      move.setEndRow(row + 1);
      moveList.push_back(move);
    }
  }
}

void BoardUtil::populateWhitePawnMoves(const Piece& piece,
                                       const Board& board, 
                                       MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);
  move.setEndColumn(col);

  // pawn can move ahead one square
  if (board.isEmpty(col, row + 1))
  {
    move.setEndRow(row + 1);

    // if this move puts us on the last rank, then we add all promotions
    if (row == (Board::NUM_ROWS - 2))
    {
      move.setPromotionType(Piece::PIECE_whiteBishop);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_whiteKnight);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_whiteQueen);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_whiteRook);
      moveList.push_back(move);
    }
    else
    {
      moveList.push_back(move);
    }

    // if we're on the second rank, then we can move ahead 2 
    if ((row == 1) && board.isEmpty(col, row + 2))
    {
      move.setEndRow(row + 2);
      moveList.push_back(move);
    }
  }
}


void BoardUtil::populateBlackPawnMoves(const Piece& piece,
                                       const Board& board, 
                                       MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);
  move.setEndColumn(col);

  // pawn can move ahead one square
  if (board.isEmpty(col, row - 1))
  {
    move.setEndRow(row - 1);

    // if this move puts us on the first rank, then we add all promotions
    if (row == 1)
    {
      move.setPromotionType(Piece::PIECE_blackBishop);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_blackKnight);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_blackQueen);
      moveList.push_back(move);

      move.setPromotionType(Piece::PIECE_blackRook);
      moveList.push_back(move);
    }
    else
    {
      moveList.push_back(move);
    }

    // if we're on the seventh rank, then we can move ahead 2 
    if ((row == 6) && board.isEmpty(col, row - 2))
    {
      move.setEndRow(row - 2);
      moveList.push_back(move);
    }
  }
}

void BoardUtil::populateWhitePawnAttacks(const Piece& piece,
                                         const Board& board, 
                                         MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);

  // capturing to the WEST
  if (col > 0)
  {
    move.setEndColumn(col - 1);
    move.setEndRow(row + 1);
    moveList.push_back(move);
  }

  // capturing to the EAST
  if (col < (Board::NUM_COLUMNS - 1))
  {
    move.setEndColumn(col + 1);
    move.setEndRow(row + 1);
    moveList.push_back(move);
  }
}

void BoardUtil::populateBlackPawnAttacks(const Piece& piece,
                                         const Board& board, 
                                         MoveList& moveList)
{
  int col = piece.getColumn();
  int row = piece.getRow();

  // This is the move we're going to add
  Move move;
  move.setStartColumn(col);
  move.setStartRow(row);
  move.setPiece(piece);

  // capturing to the WEST
  if (col > 0)
  {
    move.setEndColumn(col - 1);
    move.setEndRow(row - 1);
    moveList.push_back(move);
  }

  // capturing to the EAST
  if (col < (Board::NUM_COLUMNS - 1))
  {
    move.setEndColumn(col + 1);
    move.setEndRow(row - 1);
    moveList.push_back(move);
  }
}



void BoardUtil::populateCastle(
  const Board& board, 
  int isAttacked[Board::NUM_COLUMNS][Board::NUM_ROWS],
  MoveList& moveList)
{
  // add castling moves: The king cannot be in check currently and cannot
  // be in check during any part of the castling maneuver
  if (board.getTurn() == Board::COLOR_white)
  {
    if (board.getWhiteQueenCastle()
        && (board.getPiece(3, 0).getType() == Piece::PIECE_none)
        && (board.getPiece(2, 0).getType() == Piece::PIECE_none)
        && !isAttacked[4][0]
        && !isAttacked[3][0]
        && !isAttacked[2][0])
    {
      Move move;
      move.setStartColumn(4);
      move.setStartRow(0);
      move.setPiece(board.getPiece(4, 0));
      move.setEndColumn(2);
      move.setEndRow(0);
      moveList.push_back(move);
    }

    if (board.getWhiteKingCastle()
        && (board.getPiece(5, 0).getType() == Piece::PIECE_none)
        && (board.getPiece(6, 0).getType() == Piece::PIECE_none)
        && !isAttacked[4][0]
        && !isAttacked[5][0]
        && !isAttacked[6][0])
    {   
      Move move;
      move.setStartColumn(4);
      move.setStartRow(0);
      move.setPiece(board.getPiece(4, 0));
      move.setEndColumn(6);
      move.setEndRow(0);
      moveList.push_back(move);
    }    
  }
  else
  {
    if (board.getBlackQueenCastle()
        && (board.getPiece(3, Board::NUM_ROWS - 1).getType()
            == Piece::PIECE_none)
        && (board.getPiece(2, Board::NUM_ROWS - 1).getType()
            == Piece::PIECE_none)
        && !isAttacked[4][Board::NUM_ROWS - 1]
        && !isAttacked[3][Board::NUM_ROWS - 1]
        && !isAttacked[2][Board::NUM_ROWS - 1])
    {   
      Move move;
      move.setStartColumn(4);
      move.setStartRow(Board::NUM_ROWS - 1);
      move.setPiece(board.getPiece(4, Board::NUM_ROWS - 1));
      move.setEndColumn(2);
      move.setEndRow(Board::NUM_ROWS - 1);
      moveList.push_back(move);
    }

    if (board.getBlackKingCastle()
        && (board.getPiece(5, Board::NUM_ROWS - 1).getType()
            == Piece::PIECE_none)
        && (board.getPiece(6, Board::NUM_ROWS - 1).getType()
            == Piece::PIECE_none)
        && !isAttacked[4][Board::NUM_ROWS - 1]
        && !isAttacked[5][Board::NUM_ROWS - 1]
        && !isAttacked[6][Board::NUM_ROWS - 1])
    {   
      Move move;
      move.setStartColumn(4);
      move.setStartRow(Board::NUM_ROWS - 1);
      move.setPiece(board.getPiece(4, Board::NUM_ROWS - 1));
      move.setEndColumn(6);
      move.setEndRow(Board::NUM_ROWS - 1);
      moveList.push_back(move);
    }    
  }
}

bool BoardUtil::moveInducesCheck(const Move& move, 
                                 const Board& board, 
                                 const Piece& myKing)
{
  Board tempBoard(board);
  tempBoard.applyMove(move);
  MoveList moveList;
  BoardUtil::populateAttackList(tempBoard, tempBoard.getTurn(), moveList);
  for (MoveList::const_iterator iter = moveList.begin();
       iter != moveList.end();
       ++iter)
  {
    if ((iter->getEndColumn() == myKing.getColumn())
        && (iter->getEndRow() == myKing.getRow()))
    {
      return true;
    }
  }

  return false;
}

#if 0
bool BoardUtil::moveInducesCheck(const Move& move, 
                                 const AttackMatrix& isAttacked,
                                 const Board& board, 
                                 const Piece& myKing)
{
  // the starting location of the piece that's moving
  int pieceColumn = move.getStartColumn();
  int pieceRow = move.getStartRow();

  // the location of my king
  int kingColumn = myKing.getColumn();
  int kingRow = myKing.getRow();

  // if it's my king that's moving, then we just check if it's final
  // destination is attacked
  if (move.getPiece().getType() & static_cast<int>(Piece::PIECE_anyKing))
  {
    return isAttacked[move.getEndColumn()][move.getEndRow()];
  }

  // The move can put my king in check only if the moving piece is being
  // attacked by the enemy 
  if (!isAttacked[pieceColumn][pieceRow])
  {
    return false;
  }

  // my king has to be on the same column, row, or diagonal for this to 
  // put me in check

  // COLUMN
  ////////////////////////////////////////////////////////////////////////
  if (kingColumn == pieceColumn)
  {
    // if the piece is not moving off this column, then it's not putting
    // the king in check
    if (pieceColumn == move.getEndColumn())
    {
      return false;
    }

    // this is a mask for lateral attackers of the opposite color
    int mask = 0;
    if (myKing.getType() == Piece::PIECE_whiteKing)
    {
      mask = (static_cast<int>(Piece::PIECE_blackRook)
              | static_cast<int>(Piece::PIECE_blackQueen));
    }
    else
    {
      mask = (static_cast<int>(Piece::PIECE_whiteRook)
              | static_cast<int>(Piece::PIECE_whiteQueen));
    }

    if (kingRow > pieceRow)
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = pieceRow + 1; i < kingRow; ++i)
      {
        if (board.getPiece(pieceColumn, i).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceRow - 1; i >= 0; --i)
      {
        Piece::Type otherPieceType = board.getPiece(pieceColumn, i).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }        
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
    else
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = kingRow + 1; i < pieceRow; ++i)
      {
        if (board.getPiece(pieceColumn, i).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceRow + 1; i < Board::NUM_ROWS; ++i)
      {
        Piece::Type otherPieceType = board.getPiece(pieceColumn, i).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
  }
  // ROW
  ////////////////////////////////////////////////////////////////////////
  else if (kingRow == pieceRow)
  {
    // if the piece is not moving off this row, then it's not putting
    // the king in check
    if (pieceRow == move.getEndRow())
    {
      return false;
    }

    // this is a mask for lateral attackers of the opposite color
    int mask = 0;
    if (myKing.getType() == Piece::PIECE_whiteKing)
    {
      mask = (static_cast<int>(Piece::PIECE_blackRook)
              | static_cast<int>(Piece::PIECE_blackQueen));
    }
    else
    {
      mask = (static_cast<int>(Piece::PIECE_whiteRook)
              | static_cast<int>(Piece::PIECE_whiteQueen));
    }

    if (kingColumn > pieceColumn)
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = pieceColumn + 1; i < kingColumn; ++i)
      {
        if (board.getPiece(i, pieceRow).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn - 1; i >= 0; --i)
      {
        Piece::Type otherPieceType = board.getPiece(i, pieceRow).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }        
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
    else
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = kingColumn + 1; i < pieceColumn; ++i)
      {
        if (board.getPiece(i, pieceRow).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn + 1; i < Board::NUM_COLUMNS; ++i)
      {
        Piece::Type otherPieceType = board.getPiece(i, pieceRow).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
  }
  // DIAGONAL 1 (/)
  ////////////////////////////////////////////////////////////////////////
  else if ((kingColumn - kingRow) == (pieceColumn - pieceRow))
  {
    // if the piece is not moving off this diagonal, then it's not putting
    // the king in check
    if ((pieceColumn - pieceRow) == (move.getEndColumn() - move.getEndRow()))
    {
      return false;
    }

    // this is a mask for diagonal attackers of the opposite color
    int mask = 0;
    if (myKing.getType() == Piece::PIECE_whiteKing)
    {
      mask = (static_cast<int>(Piece::PIECE_blackBishop)
              | static_cast<int>(Piece::PIECE_blackQueen));
    }
    else
    {
      mask = (static_cast<int>(Piece::PIECE_whiteBishop)
              | static_cast<int>(Piece::PIECE_whiteQueen));
    }

    if (kingRow > pieceRow)
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = pieceColumn + 1, j = pieceRow + 1;
           i < kingColumn; // don't need to check j 
           ++i, ++j)
      {
        if (board.getPiece(i, j).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn - 1, j = pieceRow - 1;
           i >= 0; // don't need to check j
           --i, --j)
      {
        Piece::Type otherPieceType = board.getPiece(i, j).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
    else
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = kingColumn + 1, j = kingRow + 1;
           i < pieceColumn; // don't need to check j 
           ++i, ++j)
      {
        if (board.getPiece(i, j).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn + 1, j = pieceRow + 1;
           i < board::NUM_COLUMNS; // don't need to check j
           ++i, ++j)
      {
        Piece::Type otherPieceType = board.getPiece(i, j).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
  }
  // DIAGONAL (\)
  ////////////////////////////////////////////////////////////////////////
  else if ((kingColumn + kingRow) == (pieceColumn + pieceRow))
  {
    // if the piece is not moving off this diagonal, then it's not putting
    // the king in check
    if ((pieceColumn + pieceRow) == (move.getEndColumn() + move.getEndRow()))
    {
      return false;
    }

    // this is a mask for diagonal attackers of the opposite color
    int mask = 0;
    if (myKing.getType() == Piece::PIECE_whiteKing)
    {
      mask = (static_cast<int>(Piece::PIECE_blackBishop)
              | static_cast<int>(Piece::PIECE_blackQueen));
    }
    else
    {
      mask = (static_cast<int>(Piece::PIECE_whiteBishop)
              | static_cast<int>(Piece::PIECE_whiteQueen));
    }

    // King is to NorthWest of piece
    if (kingRow > pieceRow)
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = pieceColumn - 1, j = pieceRow + 1;
           i > kingColumn; // don't need to check j 
           --i, ++j)
      {
        if (board.getPiece(i, j).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn + 1, j = pieceRow - 1;
           j >= 0; // don't need to check i
           ++i, --j)
      {
        Piece::Type otherPieceType = board.getPiece(i, j).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
    else
    {
      // in order to induce check, there can't be anything between my piece
      // and the king      
      for (int i = kingColumn - 1, j = kingRow + 1;
           j < pieceRow; // don't need to check i
           --i, ++j)
      {
        if (board.getPiece(i, j).getType() != Piece::PIECE_none)
        {
          return false;
        }
      }

      // then, on the other side of my piece, there must be a rook or queen
      for (int i = pieceColumn - 1, j = pieceRow + 1;
           i >= 0; // don't need to check j
           --i, ++j)
      {
        Piece::Type otherPieceType = board.getPiece(i, j).getType();
        if (otherPieceType != Piece::PIECE_none)
        {
          return (otherPieceType & mask);
        }
      }
      
      // we must not have found an attacker lurking, so this move won't 
      // put is in check
      return false;
    }
  }

  return false;
}
#endif

bool BoardUtil::inCheck(const Board& board, Board::Color color)
{

  // find color's king
  int col = -1;
  int row = -1;
  Board::Color oppositeColor = ((color == Board::COLOR_white)
                                ? Board::COLOR_black
                                : Board::COLOR_white);
  Piece::Type colorKing = ((color == Board::COLOR_white)
                           ? Piece::PIECE_whiteKing
                           : Piece::PIECE_blackKing);
  PieceList pieceList = board.getPieceList();
  for (PieceList::const_iterator iter = pieceList.begin();
       iter != pieceList.end();
       ++iter)
  {
    if (iter->getType() == colorKing)
    {
      col = iter->getColumn();
      row = iter->getRow();
    }
  }

  // get all available moves for the opposite color
  MoveList moveList;
  populateAttackList(board, oppositeColor, moveList);

  // look to see if any of the opposite color's move attack [col, row]
  for (MoveList::const_iterator iter = moveList.begin();
       iter != moveList.end();
       ++iter)
  {
    if ((iter->getEndColumn() == col)
        && (iter->getEndRow() == row))
    {
      return true;
    }
  }

  return false;
}

State BoardUtil::calculateState(const Board& board)
{
  MoveList moveList;

  // get all available moves for color that's supposed to be moving
  populateMoveList(board, moveList);

  // if the color cannot move, then something is up!
  if (!moveList.size())
  {
    // if the color is in check, then this is checkmate
    if (inCheck(board, board.getTurn()))
    {
      if (board.getTurn() == Board::COLOR_white)
      {
        return STATE_blackWon;
      }
      else
      {
        return STATE_whiteWon;
      }
    }
    // otherwise we're in stalemate
    else
    {
      return STATE_draw;
    }
  }
  // else there are moves left
  else
  {
    // if there are only kings left, then the situation is drawn
    if (board.getPieceList().size() == 2)
    {
      return STATE_draw;
    }
    else
    {
      return STATE_ongoing;
    }
  }
}

void BoardUtil::initializeBoard(Board& board)
{
  board.setWhiteQueenCastle(true);
  board.setWhiteKingCastle(true);
  board.setBlackQueenCastle(true);
  board.setBlackKingCastle(true);
  board.setEnPassantColumn(-1);
  board.setTurn(Board::COLOR_white);

  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    for (int j = 0; j < Board::NUM_ROWS; ++j)
    {
      board.addPiece(Piece(i, j, Piece::PIECE_none));
    }
  }

  board.addPiece(Piece(0, 0, Piece::PIECE_whiteRook));
  board.addPiece(Piece(1, 0, Piece::PIECE_whiteKnight));
  board.addPiece(Piece(2, 0, Piece::PIECE_whiteBishop));
  board.addPiece(Piece(3, 0, Piece::PIECE_whiteQueen));
  board.addPiece(Piece(4, 0, Piece::PIECE_whiteKing));
  board.addPiece(Piece(5, 0, Piece::PIECE_whiteBishop));
  board.addPiece(Piece(6, 0, Piece::PIECE_whiteKnight));
  board.addPiece(Piece(7, 0, Piece::PIECE_whiteRook));

  for (int i = 0; i < Board::NUM_COLUMNS; ++i)
  {
    board.addPiece(Piece(i, 1, Piece::PIECE_whitePawn));
    board.addPiece(Piece(i, 6, Piece::PIECE_blackPawn));
  }

  board.addPiece(Piece(0, 7, Piece::PIECE_blackRook));
  board.addPiece(Piece(1, 7, Piece::PIECE_blackKnight));
  board.addPiece(Piece(2, 7, Piece::PIECE_blackBishop));
  board.addPiece(Piece(3, 7, Piece::PIECE_blackQueen));
  board.addPiece(Piece(4, 7, Piece::PIECE_blackKing));
  board.addPiece(Piece(5, 7, Piece::PIECE_blackBishop));
  board.addPiece(Piece(6, 7, Piece::PIECE_blackKnight));
  board.addPiece(Piece(7, 7, Piece::PIECE_blackRook));  
}

} // namespace sage

