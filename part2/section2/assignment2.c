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
long long currentDeadlines[] = {0,0,0,0,0,0,0,0};

thread_function_t functions[] = {&thread_button, &thread_twocolor, &thread_temp,
               &thread_track, &thread_shock, &thread_rgbcolor,&thread_aled, &thread_buzzer };


void printTasks(const int *aliveTasks){
    int i = 0;
    for(;i<8;i++){
        printf("%d ", *(aliveTasks+i));
    }
    printf("\n");
}

char *taskNum[] = {
    "BUTTON",
    "TWOCOLOR",
    "TEMP",
    "TRACK",
    "SHOCK",
    "RGBCOLOR",
    "ALED",
    "BUZZER"
};

void learn_workloads(SharedVariable* sv) {
    sv->workloads = workloads;    
    long long start, end;
    int i = 0;
    for(i = 0; i<NUM_TASKS;i++){
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        printf("%s: %lld\n",taskNum[i], end-start);
        sv->workloads[i] = end-start;
        currentDeadlines[i] = workloadDeadlines[i];
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
/* static int prev_selection = -1; */

/* int i = prev_selection + 1; */
/* while(1) { */
/*     if (i == NUM_TASKS) */
/*         i = 0; */

/*     if (aliveTasks[i] == 1) { */
/*         prev_selection = i; */
/*         break; */
/*     } */
/*     ++i; */
/* } */

/* // The retun value can be specified like this: */
/* TaskSelection sel; */
/* sel.task = prev_selection;  */
/* sel.freq = 1;  */

#define LOW 0
#define HIGH 1

void updateCurrentDeadlines(long long time_difference, int* lastAliveTasks, const int* aliveTasks){
    int i = 0;
    for(;i<NUM_TASKS;i++){
        if(aliveTasks[i] == 1){
            if(lastAliveTasks[i] == 1){
                lastAliveTasks[i] = lastAliveTasks[i] - time_difference;
            }else{
                lastAliveTasks[i] = workloadDeadlines[i];
            }
        }else{
            currentDeadlines[i] = 0;
        }
    }
}


/* we must choose based on the edf rule
   choose process is seperate from update the current deadline
 */
int chooseTask(long long *currentDeadlines ,const int* aliveTasks){
    long long minDead = 2147483647;
    int taskIdx = -1,i = 0;    
    for(;i<NUM_TASKS;i++){
        if(aliveTasks[i] == 1){
            if(currentDeadlines[i]< minDead){
                minDead = currentDeadlines[i];
                taskIdx = i;
            }
        }
    }
    return taskIdx;
}


/* how to choose the freq is seperate */
int chooseFreq(){
    return HIGH;
};




long long totalIdleTime = 0;
int lastAliveTasks[] = {0, 0, 0, 0, 0, 0, 0, 0};

void updateLastAliveTasks(const int* aliveTasks){
    int i = 0;
    for(;i<NUM_TASKS;i++){
        currentDeadlines[i] = *(aliveTasks+i);
    }
}


// sv->workloads     => estimated time to execute
// currentDeadlines  => current time to deadline
// workloadDeadlines => period & deadline
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
    
    totalIdleTime += idleTime;
    printDBG("Total idleTime is %lld\n", totalIdleTime);
    
    static long long last_timestamp = -1;
    long long current_timestamp = get_scheduler_elapsed_time_us();
    long long time_difference = 0;
    
    if (last_timestamp != -1){
        time_difference = current_timestamp - last_timestamp;        
    }
    last_timestamp = current_timestamp;
    
    updateCurrentDeadlines(time_difference, lastAliveTasks, aliveTasks);
        
    TaskSelection sel;
    sel.task = chooseTask(currentDeadlines, aliveTasks); 
    sel.freq = chooseFreq();
    updateLastAliveTasks(aliveTasks);
    
    return sel;
}
