// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Initiator_Impl.h
//
// = DESCRIPTION
//    This class implements the Initiator of the 
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#if !defined (INITIATOR_IMPL_H)
#  define INITIATOR_IMPL_H

#include "Triangle_TestS.h"

class Initiator_Impl : public POA_Initiator
{
  // = TITLE
  //     Implement the <Initiator> IDL interface.
public:
  Initiator_Impl (Object_A_ptr object_A_ptr, 
                  Object_B_ptr object_B_ptr);
  // Constructor.

  virtual ~Initiator_Impl (void);
  // Destructor.

  virtual void foo_object_B (CORBA::Environment &env);

private:
  Object_A_var object_A_var_;
  // reference to object A

  Object_B_var object_B_var_;
  // reference to object B
};

#endif /* INITIATOR_IMPL_H */
