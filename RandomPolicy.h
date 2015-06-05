#ifndef INCLUDED_sage_RandomPolicy_h
#define INCLUDED_sage_RandomPolicy_h

#ifndef INCLUDED_sage_Policy_h
#include "sage/Policy.h"
#endif

#ifndef INCLUDED_std_ctime
#include <ctime>
#define INCLUDED_std_ctime
#endif

namespace sage {

/*!
  \brief Chess policy that randomly chooses among moves.

  This class initializes the rand48 number generator upon construction
  and them uses a different random move from movelist each time.
*/
class RandomPolicy : public Policy
{
 public:

  /*!
    \brief Default constructor
  */
  RandomPolicy()
  {
    srand48(time(0));
  }

  /*!
    \brief Destructor
  */
  virtual ~RandomPolicy()
  {
    ;
  }

  virtual int decide(const Board& board, const MoveList& moveList)
  {
    return static_cast<int>(drand48() * moveList.size());
  }

 private:
};

} // namespace sage

#endif
