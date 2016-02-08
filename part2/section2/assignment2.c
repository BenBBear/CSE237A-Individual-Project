#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <stdio.h>

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h

// Assignment: You need to implement the following two functions.

typedef  void* (*thread_function_t)(void*);

long long workloads[] = {0,0,0,0,0,0,0,0};


thread_function_t functions[] = {&thread_button, &thread_twocolor, &thread_temp,
               &thread_track, &thread_shock, &thread_rgbcolor,&thread_aled, &thread_buzzer };


void printTasks(const int *aliveTasks){
    int i = 0;
    for(;i<8;i++){
        printf("%d ", *(aliveTasks+i));
    }
    printf("\n");
}


void learn_workloads(SharedVariable* sv) {
    sv->workloads = workloads;    
    long long start, end;
    int i = 0;
    for(i = 0; i<NUM_TASKS;i++){
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        printf("Button: %lld", start-end);
        sv->workloads[i] = start - end;
    }
}


// select_task(SharedVariable* sv, const int* aliveTasks):
// This function is called while runnning the actual scheduler
// - Parameters
// sv: The variable which is shared for every function over all threads
// aliveTasks: an array where each element indicates whether the corresponed task is alive(1) or not(0).
// idleTime: a time duration in microsecond. You can know how much time was waiting without any workload
//           (i.e., it's larger than 0 only when all threads are finished and not reache the next preiod.)
// - Return value
// TaskSelection structure which indicates the scheduled task and the CPU frequency
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
	// Sample scheduler: Round robin
	// It selects a next thread using aliveTasks.
    printf("%lld", idleTime);
    printTasks(aliveTasks);
    
	static int prev_selection = -1;

	int i = prev_selection + 1;
	while(1) {
		if (i == NUM_TASKS)
			i = 0;

		if (aliveTasks[i] == 1) {
			prev_selection = i;
			break;
		}
		++i;
	}

	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = 1; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)

    return sel;
}
