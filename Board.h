#ifndef INCLUDED_sage_Board_h
#define INCLUDED_sage_Board_h

#ifndef INCLUDED_sage_Piece_h
#include "sage/Piece.h"
#endif

#ifndef INCLUDED_sage_Move_h
#include "sage/Move.h"
#endif

namespace sage {

/*!
  \brief Class that captures the entire state of a chess game.
*/
class Board
{
 public:

  //! Enumeration of player colors
  enum Color
  {
    COLOR_white = 0x01,
    COLOR_black = 0x02
  };

  //! Constants defined by the chess board
  enum Constant
  {
    NUM_COLUMNS = 8, //!< Number of columns on the board
    NUM_ROWS = 8     //!< Number of rows on the board
  };

  /*!
    \brief Default constructor: builds empty board
  */
  Board();

  /*!
    \brief Destructor
  */
  virtual ~Board();

  /*!
    \brief Returns whether white can castle on the queen side
    \retval true If white can castle on Queen side
    \retval false If white cannot castle on Queen side
   */
  bool getWhiteQueenCastle() const { return m_whiteQueenCastle; }

  /*!
    \brief Returns whether white can castle on the king side
    \retval true If white can castle on King side
    \retval false If white cannot castle on King side
   */
  bool getWhiteKingCastle() const { return m_whiteKingCastle; }

  /*!
    \brief Returns whether black can castle on the queen side
    \retval true If black can castle on Queen side
    \retval false If black cannot castle on Queen side
   */
  bool getBlackQueenCastle() const { return m_blackQueenCastle; }

  /*!
    \brief Returns whether black can castle on the king side
    \retval true If black can castle on King side
    \retval false If black cannot castle on King side
   */
  bool getBlackKingCastle() const { return m_blackKingCastle; }

  /*!
    \brief Returns the en passant column available for the next move.
    \retval -1 If no en passant column
    \retval [0 - Board::NUM_COLUMNS] If en passant column available
  */
  int getEnPassantColumn() const { return m_enPassantCol; }

  /*!
    \brief Returns the color whose turn it is to move
    \retval COLOR_white If it is white's turn to move
    \retval COLOR_black If it is black's turn to move
  */
  Color getTurn() const { return m_turnColor; }

  /*!
    \brief Returns the color who is not moving
    \retval COLOR_white If it is black's turn to move
    \retval COLOR_black If it is white's turn to move
  */
  Color getOppositeTurn() const 
  {
    if (m_turnColor == COLOR_white)
    {
      return COLOR_black;
    }
    else
    {
      return COLOR_white;
    }
  }

  /*!
    \brief Returns the chess piece at the specified coordinates
    \param col The column at which to look. [0, NUM_COLUMNS - 1]
    \param row The row at which to look. [0, NUM_ROW - 1]
    \return The chess piece at that position
  */
  const Piece& getPiece(int col, int row) const
  {
    return m_squares[col][row];
  }

  /*!
    \brief Checks if the specified square is empty
    \param col The column to check
    \param row The row to check
    \retval true If the specified square is empty
    \retval false If there is a piece in the specified square
  */
  bool isEmpty(int col, int row) const
  {
    return (m_squares[col][row].getType() == Piece::PIECE_none);
  }

  /*!
    \brief Sets whether white can castle on the queen's side
    \param val true if white can castle on queen's side; false otherwise
  */
  void setWhiteQueenCastle(bool val) { m_whiteQueenCastle = val; }

  /*!
    \brief Sets whether white can castle on the king's side
    \param val true if white can castle on king's side; false otherwise
  */
  void setWhiteKingCastle(bool val) { m_whiteKingCastle = val; }

  /*!
    \brief Sets whether black can castle on the queen's side
    \param val true if black can castle on queen's side; false otherwise
  */
  void setBlackQueenCastle(bool val) { m_blackQueenCastle = val; }

  /*!
    \brief Sets whether black can castle on the king's side
    \param val true if black can castle on king's side; false otherwise
  */
  void setBlackKingCastle(bool val) { m_blackKingCastle = val; }

  /*!
    \brief Sets the en passant column
    \param val The new en passant column

    The valid values for val are -1 if no en passant column, and any 
    number in the range [0, NUM_COLUMNS-1]
  */
  void setEnPassantColumn(int val) { m_enPassantCol = val; }

  /*!
    \brief Sets the color whose turn it is to play
    \param val Color to play next
  */
  void setTurn(Color val) { m_turnColor = val; }

  /*!
    \brief Sets the specified column and row to a specific piece
    \param piece The piece to add

    The piece is added at the coordinates specified in the piece argument.
  */ 
  void addPiece(const Piece& piece)
  {
    m_squares[piece.getColumn()][piece.getRow()] = piece;
  }

  /*!
    \brief Applies the given move to the board
    \param move The move to apply

    If the move affects the other board paramters, such as castling
    and en passant, then they are updated as well.
  */
  void applyMove(const Move& move);

  /*!
    \brief Gets the list of pieces on this board
    \return The piece list
  */
  PieceList getPieceList() const;

 private:

  /*!
    \brief Adjusts m_enPassantCol for the given move
    \param move The move made on this board
  */
  void adjustEnPassant(const Move& move);

  /*!
    \brief Adjusts m_(white|black)(Queen|King)Castle  for the given move
    \param move The move made on this board
  */
  void adjustCastling(const Move& move);

  /*!
    \brief Switches the value of m_turnColor
  */
  void switchTurn();

  //! White can castle on queen side
  bool m_whiteQueenCastle;

  //! White can castle on king side
  bool m_whiteKingCastle;

  //! Black can castle on queen side
  bool m_blackQueenCastle;

  //! Black can castle on king side
  bool m_blackKingCastle;

  //! En passant column; set to -1 if none
  int m_enPassantCol;

  //! The color whose turn it is to move
  Color m_turnColor;

  //! Pieces on the board. Indexed by [column][row] starting at 0.
  Piece m_squares[NUM_COLUMNS][NUM_ROWS];
};

} // namespace sage

#endif
