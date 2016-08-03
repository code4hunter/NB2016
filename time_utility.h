#ifndef _TIME_UTILITY_H_
#define _TIME_UTILITY_H_

#include <string>

void get_current_dt(int &d,int &t);

std::string get_current_dt_s(void);

std::string get_current_date_s(void);

long long get_current_dt(int &d, int &t, int &ms);

long long get_milliseconds(int t, int ms);

long long get_tick_count(void);

long second_between_time(long t1, long t2);

#endif