#ifndef INCLUDED_sage_HumanPolicy_h
#define INCLUDED_sage_HumanPolicy_h

#ifndef INCLUDED_sage_Policy_h
#include "sage/Policy.h"
#endif

#ifndef INCLUDED_std_string
#include <string>
#define INCLUDED_std_string
#endif

namespace sage {

/*!
  \brief Chess policy that randomly chooses among moves.

  This class initializes the rand48 number generator upon construction
  and them uses a different random move from movelist each time.
*/
class HumanPolicy : public Policy
{
 public:

  /*!
    \brief Default constructor
  */
  HumanPolicy()
  {
    ;
  }

  /*!
    \brief Destructor
  */
  virtual ~HumanPolicy()
  {
    ;
  }

  virtual int decide(const Board& board, const MoveList& moveList);

 private:
  std::string getPieceName(Piece::Type type) const;
  void printBoard(const Board& board) const;
  void printMove(const Move& move) const;
  void printMoveList(const MoveList& moveList) const;
  std::string getResponse();
};

} // namespace sage

#endif
