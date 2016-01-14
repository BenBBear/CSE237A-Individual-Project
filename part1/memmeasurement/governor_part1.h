// Week 2: PMU measurement
// Important! DO NOT MODIFY this file. You will not submit this file.
// This file is provided for your convenience to implement a PMU program
// For more details, please see the instructions in the class website.

// governor_part1.h: CPU governor interface

#ifndef _GOVERNOR_H_
#define _GOVERNOR_H_

void set_governor(const char* szNewGovernor, char* szOldGovernor);
void set_by_max_freq();
void set_by_min_freq();
int get_cur_freq();

#endif

