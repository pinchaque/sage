#ifndef INCLUDED_sage_BoardUtil_h
#define INCLUDED_sage_BoardUtil_h

#ifndef INCLUDED_sage_Board_h
#include "sage/Board.h"
#endif

#ifndef INCLUDED_sage_Move_h
#include "sage/Move.h"
#endif

#ifndef INCLUDED_sage_State_h
#include "sage/State.h"
#endif

namespace sage {

/*!
  \brief Various utilities that help with board evaluation

  These methods are placed in this class in order to avoid pollution of
  the Board class. All methods are declared static so you don't have to 
  instantiate this class.
*/
class BoardUtil
{
 public:

  typedef int AttackMatrix[Board::NUM_COLUMNS][Board::NUM_ROWS];

  /*!
    \brief Default constructor
  */
  BoardUtil()
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~BoardUtil()
  {
    ;
  }

  /*!
    \brief Populates the given move list with all valid moves for the given
    board position.
    \param board The board for which we are calculating the move list
    \moveList [out] The move list to populate

    This method clears the move list before adding anything to it. There is
    no guaranteed order for the moves placed into the move list.
  */
  static void populateMoveList(const Board& board, MoveList& moveList);

  /*!
    \brief Populates the given move list with all valid attacks for the given
    board position.
    \param board The board for which we are calculating the move list
    \param color The color for which the move list is generated
    \moveList [out] The move list to populate

    This method clears the move list before adding anything to it. There is
    no guaranteed order for the moves placed into the move list.

    Note that the color is specified as a parameter in spite of the possession
    arrow stored in the actual board. This is so you can evaluate the attacks
    being threatened by the non-moving color.
  */
  static void populateAttackList(const Board& board, 
                                 const Board::Color color, 
                                 MoveList& moveList);

  /*!
    \brief Determines if the given color is in check on the specified board
    \param board The current board
    \param color The color to check for being in check
  */
  static bool inCheck(const Board& board, Board::Color color);

  /*!
    \brief Calculates the state of the given board
    \param board The board
    \return The board state
  */
  static State calculateState(const Board& board);

  /*!
    \brief Initializes given board to default chess starting position.
    \param board [inout] The board
  */
  static void initializeBoard(Board& board);

  /*!
    \brief Validates the specified board
    \param Board the board to validate
    \retval true If board is valid
    \retval false If board is not valid

    This method will check to see if the specified board position is valid
    for chess play. The following items are verified:
    - Pawns cannot be in the first and eighth ranks
    - There can only be one king per side
  */
  static bool validateBoard(const Board& board);

 private:
  /*!
    \brief Fills moveList with all possible king moves
    \param piece The king we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateKingAttacks(const Piece& piece,
                                  const Board& board, 
                                  MoveList& moveList);

  /*!
    \brief Fills moveList with all possible rook moves
    \param piece The rook we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateRookAttacks(const Piece& piece,
                                  const Board& board, 
                                  MoveList& moveList);

  /*!
    \brief Fills moveList with all possible bishop moves
    \param piece The bishop we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateBishopAttacks(const Piece& piece,
                                    const Board& board, 
                                    MoveList& moveList);

  /*!
    \brief Fills moveList with all possible knight moves
    \param piece The knight we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateKnightAttacks(const Piece& piece,
                                    const Board& board, 
                                    MoveList& moveList);

  /*!
    \brief Fills moveList with all possible WhitePawn moves
    \param piece The WhitePawn we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateWhitePawnMoves(const Piece& piece,
                                     const Board& board, 
                                     MoveList& moveList);

  /*!
    \brief Fills moveList with all possible BlackPawn moves
    \param piece The BlackPawn we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateBlackPawnMoves(const Piece& piece,
                                     const Board& board, 
                                     MoveList& moveList);

  /*!
    \brief Fills moveList with all possible WhitePawn attacks
    \param piece The WhitePawn we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateWhitePawnAttacks(const Piece& piece,
                                       const Board& board, 
                                       MoveList& moveList);

  /*!
    \brief Fills moveList with all possible BlackPawn attacks
    \param piece The BlackPawn we're moving
    \param board The board we're moving on
    \param moveList [out] The move list we're populating
  */
  static void populateBlackPawnAttacks(const Piece& piece,
                                       const Board& board, 
                                       MoveList& moveList);

  /*!
    \brief Adds castling moves to a movelist
    \param board The board we're moving on
    \param isAttacked array of whether a square is attacked
    \param moveList [out] The move list we're populating
  */
  static void populateCastle(
    const Board& board, 
    int isAttacked[Board::NUM_COLUMNS][Board::NUM_ROWS],
    MoveList& moveList);

  /*
    \brief Determines if the specified move will put the moving color
    into check.
    \param move The move that we want to make
    \param isAttacked Matrix of squares attacked by the opponent
    \param board The board on which we're making the move
    \param myKing The location of my king
  */
  static bool moveInducesCheck(const Move& move, 
                               const Board& board, 
                               const Piece& myKing);
};

} // namespace sage

#endif
