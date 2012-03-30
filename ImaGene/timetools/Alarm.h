/** @file Alarm.h */
///////////////////////////////////////////////////////////////////////////////
// Alarm.h
//
// JOL - 2001/04/27
// 
// Basic tools for installing functions calls by timer once or on a regular
// basis.
//
///////////////////////////////////////////////////////////////////////////////


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class Alarm
///////////////////////////////////////////////////////////////////////////////
/**
 * Description of class 'SWindow'
 * Aim: To provide simple functions to set up, start, stops, alarms and 
 * clocks.
 */
class Alarm 
{
public:
  /**
   * Alarm function type definition. The integer in unput is the signal number.
   */
  typedef void (* AlarmFunction )( int );

  // ------------------ managing alarm function -------------------------------

  /**
   * Installs a new function to be called when an alarm signal is sent.
   * @param new_fct the function to be called when the alarm signal is sent.
   * @param old_fct the function that was called when the alarm signal is sent.
   * @return 'true' if the function [new_fct] has been correctly installed, 'false' otherwise.
   */
  static bool setAlarmFunction( AlarmFunction new_fct,
				AlarmFunction & old_fct );
  /**
   * Installs a new function to be called when an alarm signal is sent.
   * (Don't care about the previous alarm function.)
   * @param new_fct the function to be called when the alarm signal is sent.
   * @return 'true' if the function [new_fct] has been correctly installed, 'false' otherwise.
   */
  static bool setAlarmFunction( AlarmFunction new_fct );
  /**
   * @return the function that is called when an alarm signal is sent.
   */
  static AlarmFunction getAlarmFunction();
  /**
   * @return the default alarm function.
   */
  static AlarmFunction defaultAlarmFunction();
  /**
   * Resets the alarm to default alarm system.
   */
  static bool resetAlarm();

  // ------------------ managing alarm clock ----------------------------------

  /**
   * Schedules the alarm signal to be sent in [time_s] seconds plus [time_us]
   * microseconds. 
   * @param time_s the time in seconds.
   * @param time_us the time in microseconds (added to [time_s]).
   * @return 'true' on success.
   */
  static bool scheduleAlarmOnce( long time_s, long time_us );
  /**
   * Schedules the alarm signal to be sent every [interval_time_s] seconds plus
   * [interval_time_us] microseconds. The alarmFunction associated with the
   * alarm signal will be regularly.
   * @param interval_time_s the time in seconds.
   * @param interval_time_us the time in microseconds (added to [time_s]).
   * @return 'true' on success.
   */
  static bool scheduleAlarmRegularly( long interval_time_s,
				      long interval_time_us );
  /**
   * Schedules the alarm signal to be sent every [interval_time_s] seconds plus
   * [interval_time_us] microseconds. The alarmFunction associated with the
   * alarm signal will be regularly. The first alarm signal is sent after
   * [initial_time_s] seconds plus [initial_time_us] microseconds.
   * @param initial_time_s the time in seconds.
   * @param initial_time_us the time in microseconds (added to [time_s]).
   * @param interval_time_s the time in seconds.
   * @param interval_time_us the time in microseconds (added to [time_s]).
   * @return 'true' on success.
   */
  static bool scheduleAlarmRegularly( long initial_time_s,
				      long initial_time_us,
				      long interval_time_s,
				      long interval_time_us );
  /**
   * Stops the alarm so that no more alarm signal is sent.
   * @return 'true' on success.
   */
  static bool unscheduleAlarm();
  
};


} // namespace ImaGene
