// This may look like C, but it's really -*- C++ -*-
//
//
// Header file for Win32 C/C++/COM interface to a CORBA ORB.
//
// This file defines way in which CORBA exceptions are reported.
//

//
// CORBA2-specified exception hierarchy.
//
// All exceptions have a type (represented by a TypeCode) and a widely
// scoped type ID (in the TypeCode) that generated by any OMG-IDL compiler 
// and available through the Interface Repositories.  Think of it as a
// "globally scoped" name distinguishing each exception.
//
#if !defined(ACE_ROA_EXCEPT_H)
#  define ACE_ROA_EXCEPT_H

#  include <ace/Synch.h>

extern const IID	IID_CORBA_Exception;
extern const IID	IID_CORBA_UserException;
extern const IID	IID_CORBA_SystemException;

class _EXPCLASS CORBA_Exception : public IUnknown
{
public:
  CORBA_Exception (const CORBA_Exception &src);
  CORBA_Exception		&operator = (const CORBA_Exception &src);

  void			*operator new (size_t, const void *p)
  { return (void *) p; }
  void			*operator new (size_t s)
  { return ::operator new (s); }
  void			operator delete (void *p)
  { ::operator delete (p); }


  const CORBA_String		id () const;
  const CORBA_TypeCode_ptr	type () const;

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall		AddRef ();
  ULONG __stdcall		Release ();
  HRESULT __stdcall           QueryInterface (
					      REFIID	riid,
					      void	**ppv
					      );

  CORBA_Exception (CORBA_TypeCode_ptr type);
  virtual ~CORBA_Exception ();
private:
  CORBA_TypeCode_ptr		_type;

  ACE_Thread_Mutex lock_;
  unsigned			_refcnt;
};

//
// User exceptions are those defined by application developers
// using OMG-IDL.
//
class _EXPCLASS CORBA_UserException : public CORBA_Exception {
  public:
			    CORBA_UserException (CORBA_TypeCode_ptr tc);
			    ~CORBA_UserException ();
  protected:
			    // copy and assignment operators
};

//
// System exceptions are those defined in the CORBA spec; OMG-IDL
// defines these.
//
enum CORBA_CompletionStatus {
    COMPLETED_YES,		// successful or exceptional completion
    COMPLETED_NO,		// didn't change any state; retry is OK
    COMPLETED_MAYBE		// can't say what happened; retry unsafe
};

class _EXPCLASS CORBA_SystemException : public CORBA_Exception {
  public:
    // 94-9-14 also sez:  public copy constructor
    // and assignment operator.

				CORBA_SystemException (
				    CORBA_TypeCode_ptr		tc,
				    CORBA_ULong			code,
				    CORBA_CompletionStatus	completed
				);
				~CORBA_SystemException ();

    CORBA_ULong			minor () const { return _minor; }
    void			minor (CORBA_ULong m) { _minor = m; }

    CORBA_CompletionStatus	completion () const { return _completed; }
    void			completion (CORBA_CompletionStatus c)
				{ _completed = c; }

  private:
    CORBA_ULong			_minor;
    CORBA_CompletionStatus	_completed;
};


//
// Declarations for all of the CORBA standard exceptions.
//
// XXX shouldn't have a default minor code, at least for code that's
// inside the ORB.  All minor codes should be symbolically catalogued.
//
#define SYSEX(name) \
extern CORBA_TypeCode_ptr		_tc_CORBA_ ## name ; \
class _EXPCLASS CORBA_ ## name : public CORBA_SystemException { \
  public: \
			    CORBA_ ## name ( \
				CORBA_CompletionStatus	completed, \
				CORBA_ULong		code = 0xffff0000L \
			    ) : CORBA_SystemException ( \
				    _tc_CORBA_ ## name, \
				    code, completed) { } }

SYSEX (UNKNOWN);
SYSEX (BAD_PARAM);
SYSEX (NO_MEMORY);
SYSEX (IMP_LIMIT);
SYSEX (COMM_FAILURE);
SYSEX (INV_OBJREF);
SYSEX (OBJECT_NOT_EXIST);
SYSEX (NO_PERMISSION);
SYSEX (INTERNAL);
SYSEX (MARSHAL);
SYSEX (INITIALIZE);
SYSEX (NO_IMPLEMENT);
SYSEX (BAD_TYPECODE);
SYSEX (BAD_OPERATION);
SYSEX (NO_RESOURCES);
SYSEX (NO_RESPONSE);
SYSEX (PERSIST_STORE);
SYSEX (BAD_INV_ORDER);
SYSEX (TRANSIENT);
SYSEX (FREE_MEM);
SYSEX (INV_IDENT);
SYSEX (INV_FLAG);
SYSEX (INTF_REPOS);
SYSEX (BAD_CONTEXT);
SYSEX (OBJ_ADAPTER);
SYSEX (DATA_CONVERSION);

#undef	SYSEX

//
// A CORBA_Environment is a way to automagically ensure that exception
// data is freed -- the "var" class for Exceptions.  It adds just a bit
// of convenience function support, helping classify exceptions as well
// as reducing memory leakage.
//
enum CORBA_ExceptionType {
    NO_EXCEPTION,
    SYSTEM_EXCEPTION,
    USER_EXCEPTION
};

class CORBA_Environment {
  public:
				CORBA_Environment () : _exception (0) { }
				~CORBA_Environment () { clear (); }

    CORBA_Exception_ptr		exception () const { return _exception; }
    void			exception (CORBA_Exception *ex)
				{ clear (); _exception = ex; }

    CORBA_ExceptionType		exception_type () const;
    const CORBA_String		exception_id () const;

    void			clear ()
    				{
				    if (_exception) {
					_exception->Release ();
					_exception = 0;	// XXX
				    }
				}

  private:
    CORBA_Exception_ptr		_exception;

    // these are not provided
				CORBA_Environment (const CORBA_Environment &src);
    CORBA_Environment		&operator = (const CORBA_Environment &src);
};
#endif
