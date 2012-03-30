///////////////////////////////////////////////////////////////////////////////
// Alarm.cxx
//
// JOL - 2001/04/27
// 
// Basic tools for installing functions calls by timer once or on a regular
// basis.
//
///////////////////////////////////////////////////////////////////////////////

#include <signal.h>
#include <sys/time.h>
#include "ImaGene/timetools/Alarm.h"

#define ALARM_SIGNAL SIGALRM
#define ALARM_ITIMER ITIMER_REAL
//  #define ALARM_SIGNAL SIGVTALRM
//  #define ALARM_ITIMER ITIMER_VIRTUAL


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// ------------------ managing alarm function --------------------------------

//- Installs a new function to be called when an alarm signal is sent.
//- @param new_fct the function to be called when the alarm signal is sent.
//- @param old_fct the function that was called when the alarm signal is sent.
//- @return 'true' if the function [new_fct] has been correctly installed, 'false' otherwise.
bool 
Alarm::setAlarmFunction( AlarmFunction new_fct, AlarmFunction & old_fct )
{
  old_fct = signal( ALARM_SIGNAL, new_fct );
  return old_fct != SIG_ERR;
}

//- Installs a new function to be called when an alarm signal is sent.
//- (Don't care about the previous alarm function.)
//- @param new_fct the function to be called when the alarm signal is sent.
//- @return 'true' if the function [new_fct] has been correctly installed, 'false' otherwise.
bool
Alarm::setAlarmFunction( AlarmFunction new_fct )
{
  AlarmFunction old_fct;
  return setAlarmFunction( new_fct, old_fct );
}

//- @return the function that is called when an alarm signal is sent.
Alarm::AlarmFunction 
Alarm::getAlarmFunction()
{
  AlarmFunction old_fct;
  setAlarmFunction( defaultAlarmFunction(), old_fct );
  setAlarmFunction( old_fct );
  return old_fct;
}

//- @return the default alarm function.
Alarm::AlarmFunction 
Alarm::defaultAlarmFunction()
{
  return SIG_DFL;
}

//- Resets the alarm to default alarm system.
bool 
Alarm::resetAlarm()
{
  return setAlarmFunction( defaultAlarmFunction() );
}

///////////////////////////////////////////////////////////////////////////////
// ------------------ managing alarm clock -----------------------------------

//- Schedules the alarm signal to be sent in [time_s] seconds plus [time_us]
//- microseconds. 
//- @param time_s the time in seconds.
//- @param time_us the time in microseconds (added to [time_s]).
//- @return 'true' on success.
bool 
Alarm::scheduleAlarmOnce( long time_s, long time_us )
{
  itimerval new_time;
  new_time.it_interval.tv_sec = 0;
  new_time.it_interval.tv_usec = 0;
  new_time.it_value.tv_sec = time_s;
  new_time.it_value.tv_usec = time_us;
  int ok = setitimer( ALARM_ITIMER, &new_time, 0 );
  return ok == 0;
}

//- Schedules the alarm signal to be sent every [interval_time_s] seconds plus
//- [interval_time_us] microseconds. The alarmFunction associated with the
//- alarm signal will be regularly.
//- @param interval_time_s the time in seconds.
//- @param interval_time_us the time in microseconds (added to [time_s]).
//- @return 'true' on success.
bool 
Alarm::scheduleAlarmRegularly( long interval_time_s, long interval_time_us )
{
  itimerval new_time;
  new_time.it_interval.tv_sec = interval_time_s;
  new_time.it_interval.tv_usec = interval_time_us;
  new_time.it_value.tv_sec = interval_time_s;
  new_time.it_value.tv_usec = interval_time_us;
  int ok = setitimer( ALARM_ITIMER, &new_time, 0 );
  return ok == 0;
}

//- Schedules the alarm signal to be sent every [interval_time_s] seconds plus
//- [interval_time_us] microseconds. The alarmFunction associated with the
//- alarm signal will be regularly. The first alarm signal is sent after
//- [initial_time_s] seconds plus [initial_time_us] microseconds.
//- @param initial_time_s the time in seconds.
//- @param initial_time_us the time in microseconds (added to [time_s]).
//- @param interval_time_s the time in seconds.
//- @param interval_time_us the time in microseconds (added to [time_s]).
//- @return 'true' on success.
bool 
Alarm::scheduleAlarmRegularly( long initial_time_s, long initial_time_us,
			       long interval_time_s, long interval_time_us )
{
  itimerval new_time;
  new_time.it_interval.tv_sec = interval_time_s;
  new_time.it_interval.tv_usec = interval_time_us;
  new_time.it_value.tv_sec = initial_time_s;
  new_time.it_value.tv_usec = initial_time_us;
  int ok = setitimer( ALARM_ITIMER, &new_time, 0 );
  return ok == 0;
}

//- Stops the alarm so that no more alarm signal is sent.
//- @return 'true' on success.
bool 
Alarm::unscheduleAlarm()
{
  itimerval new_time;
  new_time.it_interval.tv_sec = 0;
  new_time.it_interval.tv_usec = 0;
  new_time.it_value.tv_sec = 0;
  new_time.it_value.tv_usec = 0;
  int ok = setitimer( ALARM_ITIMER, &new_time, 0 );
  return ok == 0;
}


} // namespace ImaGene

