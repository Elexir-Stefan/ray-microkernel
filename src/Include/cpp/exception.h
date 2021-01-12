/*
 * exception.h
 *
 *  Created on: 07.01.2010
 *      Author: stefan
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

namespace RayKernel
{

  /**
   *  @brief Base class for all library exceptions.
   *
   *  This is the base class for all exceptions thrown by the standard
   *  library, and by certain language expressions.  You are free to derive
   *  your own %exception classes, or use a different hierarchy, or to
   *  throw non-class data (e.g., fundamental types).
   */
  class Exception
  {
  public:
    Exception() throw() { };
    virtual ~Exception() throw() {};

  };


} // namespace
#endif /* EXCEPTION_H_ */
