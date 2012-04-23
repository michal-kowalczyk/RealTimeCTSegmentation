/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer
{
public:
    Timer ();
    void StartCycle ();
    void StopCycle ();
    unsigned long long TotalTime ();
    unsigned long long AverageTimePerCycle ();
private:
    clock_t currentTime;
    unsigned long long totalTime;
    unsigned int cyclesNumber;
};

#endif // TIMER_H
