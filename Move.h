#ifndef INCLUDED_sage_Move_h
#define INCLUDED_sage_Move_h

#ifndef INCLUDED_sage_Piece_h
#include "sage/Piece.h"
#endif

#ifndef INCLUDED_std_vector
#include <vector>
#endif

namespace sage {

/*!
  \brief Class that encapsultes a single move on a chessboard

  The basic information needed is simply the start and end positions,
  represented as [column,row] pairs. However, for completeness, this class
  also includes the piece identifier (which piece was moved) and a
  flag to indicate if this move is a capture.
*/
class Move
{
 public:
  /*!
    \brief Default constructor: a null move

    Once constructed, this move isn't good for anything so you have to fill
    it with information before using it.
  */
  Move()
    : m_piece(), m_startColumn(0), m_startRow(0), m_endColumn(0),
    m_endRow(0), m_capture(false), m_enPassant(false), 
    m_promotionType(Piece::PIECE_none)
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~Move() { ; }

  /*!
    \brief Returns the identifier of the piece that is moving
  */
  const Piece& getPiece() const { return m_piece; }

  /*!
    \brief Returns the starting column of the move
  */
  int getStartColumn() const { return m_startColumn; }

  /*!
    \brief Returns the starting row of the move
  */
  int getStartRow() const { return m_startRow; }

  /*!
    \brief Returns the ending column of the move
  */
  int getEndColumn() const { return m_endColumn; }

  /*!
    \brief Returns the ending row of the move
  */
  int getEndRow() const { return m_endRow; }

  /*! 
    \brief Returns whether this move is a capture
    \retval true If the move is a capture
    \retval false If the move is not a capture
  */
  bool getCapture() const { return m_capture; }

  /*!
    \brief Gets the en passant flag
    \retval true if this is an en passant capture
    \retval false otherwise
  */
  bool getEnPassant() const { return m_enPassant; }

  /*!
    \brief Gets the piece type if this is a promotion
    \retval piece type
  */
  Piece::Type getPromotionType() const { return m_promotionType; }

  /*!
    \brief Sets the piece being moved
    \param piece The piece identifier 
  */
  void setPiece(const Piece& piece) { m_piece = piece; }

  /*!
    \brief Sets the start column of the move
    \param val The start column
    
    Note that any value can be passed in here but some values may not be
    legal in the context of the actual chess board.
  */
  void setStartColumn(int val) { m_startColumn = val; }

  /*!
    \brief Sets the start row of the move
    \param val The start row
    
    Note that any value can be passed in here but some values may not be
    legal in the context of the actual chess board.
  */
  void setStartRow(int val) { m_startRow = val; }

  /*!
    \brief Sets the end column of the move
    \param val The end column
    
    Note that any value can be passed in here but some values may not be
    legal in the context of the actual chess board.
  */
  void setEndColumn(int val) { m_endColumn = val; }

  /*!
    \brief Sets the end row of the move
    \param val The end row
    
    Note that any value can be passed in here but some values may not be
    legal in the context of the actual chess board.
  */
  void setEndRow(int val) { m_endRow = val; }

  /*!
    \brief Sets whether this move is a capture
    \param val True if the move is a capture, false otherwise
  */
  void setCapture(bool val) { m_capture = val; }

  /*!
    \brief Sets en passant flag
    \param val En passant flag
  */
  void setEnPassant(bool val) { m_enPassant = val; }

  /*!
    \brief Sets the promotion type
    \param val Piece type that we're promoting to
  */
  void setPromotionType(Piece::Type type) { m_promotionType = type; }

 private:
  //! The piece that is moving (not strictly necessary, but helpful)
  Piece m_piece;

  //! The starting column of the move
  int m_startColumn;

  //! The starting row of the move
  int m_startRow;

  //! The ending column of the move
  int m_endColumn;

  //! The ending row of the move
  int m_endRow;

  //! A flag to indicate if this is a capture move
  bool m_capture;

  //! Flag to indicate if this is an enpassant capture
  bool m_enPassant;

  //! Piece type if this is a pawn promotion
  Piece::Type m_promotionType;
};

//! Handy typedef for a list of moves
typedef std::vector<Move> MoveList;

} // namespace sage

#endif
