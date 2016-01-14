// Week 2: PMU measurement
// Important! DO NOT MODIFY this file. You will not submit this file.
// This file is provided for your convenience to implement a PMU program
// For more details, please see the instructions in the class website.

// governor_part1.c: CPU governor interface

#include "governor_part1.h"
#include <stdio.h>

static int maxFreq, minFreq;

void set_governor(const char* szNewGovernor, char* szOldGovernor) {
	FILE* fp;
	if (szOldGovernor != NULL) {
		fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "r");
		fscanf(fp, "%s", szOldGovernor);
		fclose(fp);
	}

	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "w");
	fprintf(fp, "%s", szNewGovernor);
	fclose(fp);

	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
	fscanf(fp, "%d", &maxFreq);
	fclose(fp);

	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", "r");
	fscanf(fp, "%d", &minFreq);
	fclose(fp);
}

static void set_frequency(int frequency) {
	FILE* fp;
	fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed", "w");
	fprintf(fp, "%d", frequency);
	fclose(fp);
}

void set_by_max_freq() {
	set_frequency(maxFreq);
}


void set_by_min_freq() {
	set_frequency(minFreq);
}

int get_cur_freq() {
	int curFreq;
	FILE* fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed", "r");
	fscanf(fp, "%d", &curFreq);
	fclose(fp);

	return curFreq;
}
