#include<ctime>
#include "timer.h"

timespec start_time;

void reset_timer(){
  clock_gettime(TIMER_TYPE, &start_time);
}

double get_time(){
  timespec t;
  clock_gettime(TIMER_TYPE, &t);
  return double(t.tv_sec - start_time.tv_sec) + double(t.tv_nsec - start_time.tv_nsec) * 1.e-9;
}
