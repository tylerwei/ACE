#include "params.hh"
#include "connect.hh"
#include "objtable.hh"

ROA_Parameters::ROA_Parameters()
  : using_threads_(0),
    thread_flags_(THR_NEW_LWP),
    context_p_(0),
    upcall_(0),
    forwarder_(0),
    oa_(0)
{
}

// Determine the appropriate default thread flags, based on system.
// When I put the concurrency strategy into the factory, then this will
// go away b/c the concurrency strategy will do this appropriate
// for each platform!
#  if defined(linux)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(WIN32)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(sparc)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  else
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  endif

ROA_Factory::CONCURRENCY_STRATEGY*
ROA_Factory::concurrency_strategy()
{
  ROA_Parameters* p = ROA_PARAMS::instance();


  if (p->using_threads())
    {
      // Set the strategy parameters
      threaded_strategy_.open(ACE_Service_Config::thr_mgr(), ROA_DEFAULT_THREADFLAGS);
      concurrency_strategy_ = &threaded_strategy_;
    }
  else
    {
      reactive_strategy_.open(ACE_Service_Config::reactor());
      concurrency_strategy_ = &reactive_strategy_;
    }

  return concurrency_strategy_;
}

TAO_Object_Table* ROA_Factory::objlookup_strategy()
{
  ROA_Parameters* p = ROA_PARAMS::instance();

  switch(p->demux_strategy())
    {
    case ROA_Parameters::TAO_LINEAR:
      break;
    case ROA_Parameters::TAO_USER_DEFINED:
      // it is assumed that the user will use the hooks to supply a
      // user-defined instance of the object table
      break;
    case ROA_Parameters::TAO_ACTIVE_DEMUX:
      break;
    case ROA_Parameters::TAO_DYNAMIC_HASH:
    default:
      this->objtable_ = new TAO_Dynamic_Hash_ObjTable (p->tablesize());
    }
  return this->objtable_;
}

ROA_Factory::ROA_Factory()
  : concurrency_strategy_(0),
    objtable_(0)
{
}


#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Thread_Strategy<ROA_Handler>;
template class ACE_Concurrency_Strategy<ROA_Handler>;
template class ACE_Creation_Strategy<ROA_Handler>;
template class ACE_Scheduling_Strategy<ROA_Handler>;
template class ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Singleton<ROA_Parameters, ACE_Thread_Mutex>;
template class ACE_Singleton<ROA_Factory, ACE_Thread_Mutex>;
#endif
