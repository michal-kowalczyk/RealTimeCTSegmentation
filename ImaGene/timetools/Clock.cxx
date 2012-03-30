///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Clock.cxx
//
// Creation : 2000/??/??
//
// Version : 2000/??/??
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.u-bordeaux.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2000/??/?? : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

#include <cstdlib>
#if ( (defined(UNIX)||defined(unix)||defined(linux)) )
#include <sys/time.h>
#elif ( (defined(WIN32)) )
#include <time.h>
#else
#include <sys/time.h>
#endif
#include "ImaGene/timetools/Clock.h"

#if defined(NO_DEBUG)
#define ASSERT_Clock(x)
#define DEBUGCMD_Clock(x)
#else //defined(NO_DEBUG)
#define ASSERT_Clock(x) if(!(x)) \
    { cerr << "Assertion failed : (" << #x << ')' << endl \
    << "In file : " << __FILE__ << " at line #" << __LINE__ << endl \
    << "Compiled the " << __DATE__ << " at " << __TIME__ << endl; abort();}
#define DEBUGCMD_Clock(x) x
#endif // else defined(NO_DEBUG)

const char* const Clock_RCS_ID = "@(#)class Clock definition.";

///////////////////////////////////////////////////////////////////////////////
// class Clock
///////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

///////////////////////////////////////////////////////////////////////////////
// -------------------------- timing services -------------------------------

#if ( (defined(UNIX)||defined(unix)||defined(linux)) )
static struct itimerval timer_init;
#elif ( (defined(WIN32)) )
static clock_t first_tick;
#else
static struct itimerval timer_init;
#endif


namespace ImaGene {


//- Starts a clock.
void Clock::startClock()
{
#if ( (defined(UNIX)||defined(unix)||defined(linux)) )
  struct itimerval old_timer;
  timer_init.it_interval.tv_sec=0;
  timer_init.it_interval.tv_usec=0;
  timer_init.it_value.tv_sec=100000;
  timer_init.it_value.tv_usec=0;
  if (setitimer(ITIMER_VIRTUAL, &timer_init, &old_timer)!=0)
    {
      cerr << "[Clock::startClock] Erreur sur 'setitimer()'." << endl;
    }
#elif ( (defined(WIN32)) )
  first_tick = clock();
  if (first_tick == (clock_t) -1)
    {
      cerr << "[Clock::startClock] Erreur sur 'clock()'." << endl;
    }
#else
  struct itimerval old_timer;
  timer_init.it_interval.tv_sec=0;
  timer_init.it_interval.tv_usec=0;
  timer_init.it_value.tv_sec=100000;
  timer_init.it_value.tv_usec=0;
  if (setitimer(ITIMER_VIRTUAL, &timer_init, &old_timer)!=0)
    {
      cerr << "[Clock::startClock] Erreur sur 'setitimer()'." << endl;
    }
#endif
}

//- @return the time (in ms) since the last 'startClock()'.
long Clock::stopClock()
{
#if ( (defined(UNIX)||defined(unix)||defined(linux)) )
  long d;
  struct itimerval timer_current;

  if (getitimer(ITIMER_VIRTUAL, &timer_current)!=0)
    {
      cerr << "[Clock::stopClock] Erreur sur 'getitimer()'." << endl;
    }
  d=(timer_init.it_value.tv_sec-timer_current.it_value.tv_sec)*1000-
    (timer_current.it_value.tv_usec)/1000;
  return(d >= 0 ? d : 0);
#elif ( (defined(WIN32)) )
  clock_t last_tick = clock();
  if (last_tick == (clock_t) -1)
    {
      cerr << "[Clock::stopClock] Erreur sur 'clock()'." << endl;
    }
  return (long) ((float) 1000.0 * (float)(last_tick - first_tick)
		 / (float) CLOCKS_PER_SEC);
#else
  long d;
  struct itimerval timer_current;

  if (getitimer(ITIMER_VIRTUAL, &timer_current)!=0)
    {
      cerr << "[Clock::stopClock] Erreur sur 'getitimer()'." << endl;
    }
  d=(timer_init.it_value.tv_sec-timer_current.it_value.tv_sec)*1000-
    (timer_current.it_value.tv_usec)/1000;
  return(d >= 0 ? d : 0);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

void Clock::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Clock]";
}

bool Clock::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
