#ifndef INCLUDED_sage_Exception_h
#define INCLUDED_sage_Exception_h

#ifndef INCLUDED_std_exception
#include <exception>
#define INCLUDED_std_exception
#endif

#ifndef INCLUDED_std_string
#include <string>
#define INCLUDED_std_string
#endif

namespace sage {

/*!
  \brief Exception that's thrown by Board::applyMove when a move is invalid.
*/
class Exception : public std::exception
{
 public:
  Exception(const char* msg)
    : m_msg(msg)
  {
    ;
  }

  virtual ~Exception() throw ()
  {
    ;
  }

 private:
  std::string m_msg;
};

/*!
  \brief Exception that's thrown by Board::applyMove when a move is invalid.
*/
class InvalidMoveException : public Exception
{
 public:
  InvalidMoveException(const char* msg)
    : Exception(msg)
  {
    ;
  }
};

} // namespace sage

#endif
