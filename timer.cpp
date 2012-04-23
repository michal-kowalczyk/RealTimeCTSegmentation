/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "timer.h"

Timer::Timer (){
    totalTime = 0;
    cyclesNumber = 0;
}


void Timer::StartCycle (){
    currentTime = clock ();
}


void Timer::StopCycle (){
    totalTime += clock () - currentTime;
    cyclesNumber++;
}


unsigned long long Timer::TotalTime (){
    return totalTime;
}


unsigned long long Timer::AverageTimePerCycle (){
    if (cyclesNumber)
        return totalTime / cyclesNumber;
    return 0;
}
