#ifndef INCLUDED_sage_Piece_h
#define INCLUDED_sage_Piece_h

#ifndef INCLUDED_std_vector
#include <vector>
#endif

namespace sage {

class Piece
{
  public:
//! Enumeration of chess pieces
  enum Type
  {
    PIECE_none        = 0x0000,
    PIECE_whiteKing   = 0x0001,
    PIECE_whiteQueen  = 0x0002,
    PIECE_whiteRook   = 0x0004,
    PIECE_whiteBishop = 0x0008,
    PIECE_whiteKnight = 0x0010,
    PIECE_whitePawn   = 0x0020,
    PIECE_blackKing   = 0x0100,
    PIECE_blackQueen  = 0x0200,
    PIECE_blackRook   = 0x0400,
    PIECE_blackBishop = 0x0800,
    PIECE_blackKnight = 0x1000,
    PIECE_blackPawn   = 0x2000
  };

//! Masks to be used in conjunction with Piece
  enum Mask
  {
    PIECE_whiteAll  = (PIECE_whiteKing
                       | PIECE_whiteQueen
                       | PIECE_whiteRook
                       | PIECE_whiteBishop
                       | PIECE_whiteKnight
                       | PIECE_whitePawn),
    PIECE_blackAll  = (PIECE_blackKing
                       | PIECE_blackQueen
                       | PIECE_blackRook
                       | PIECE_blackBishop
                       | PIECE_blackKnight
                       | PIECE_blackPawn),
    PIECE_anyKing   = (PIECE_whiteKing | PIECE_blackKing),
    PIECE_anyQueen  = (PIECE_whiteQueen | PIECE_blackQueen),
    PIECE_anyRook   = (PIECE_whiteRook | PIECE_blackRook),
    PIECE_anyBishop = (PIECE_whiteBishop | PIECE_blackBishop),
    PIECE_anyKnight = (PIECE_whiteKnight | PIECE_blackKnight),
    PIECE_anyPawn   = (PIECE_whitePawn | PIECE_blackPawn)
  };

  /*!
    \brief Constructor
   */
  Piece(int column, int row, Type type)
    : m_type(type), m_column(column), m_row(row)
  {
    ;
  }

  /*!
    \brief Default constructor
   */
  Piece()
    : m_type(PIECE_none), m_column(0), m_row(0)
  {
    ;
  }

  virtual ~Piece()
  {
    ;
  }

  /*!
    \brief Sets the piece type
    \param type The piece type
  */
  void setType(Type type) { m_type = type; }

  /*!
    \brief Sets the piece column
    \param column The piece column
  */
  void setColumn(int val) { m_column = val; }

  /*!
    \brief Sets the piece row
    \param row The piece row
  */
  void setRow(int val) { m_row = val; }

  /*!
    \brief Returns the piece type
  */
  Type getType() const { return m_type; }

  /*!
    \brief Returns the piece column
  */
  int getColumn() const { return m_column; }

  /*!
    \brief Returns the piece row
  */
  int getRow() const { return m_row; }

  /*!
    \brief Equality operator
  */
  bool operator==(const Piece& other) const
  {
    return ((m_type == other.m_type)
            && (m_column == other.m_column)
            && (m_row == other.m_row));
  }

  /*!
    \brief Inequality operator
  */
  bool operator!=(const Piece& other) const { return !(*this == other); }

 private:
  //! Type of this piece
  Type m_type;

  //! Column of this piece
  int m_column;

  //! Row of this piece
  int m_row;
};

//! Handy typedef for a list of pieces
typedef std::vector<Piece> PieceList;

} // namespace sage

#endif
