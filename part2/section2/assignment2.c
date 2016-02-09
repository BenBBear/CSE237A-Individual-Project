#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <stdio.h>
#include <limits.h>
#include <float.h>
#define LOW 0
#define HIGH 1
#define Kbit(n,k) (n >> k) & 1
#define END_SEQ 255
#define LIMIT 1.0
// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h

// Assignment: You need to implement the following two functions.

typedef  void* (*thread_function_t)(void*);


long long currentDeadlines[] = {0,0,0,0,0,0,0,0};

thread_function_t functions[] = {&thread_button, &thread_twocolor, &thread_temp,
               &thread_track, &thread_shock, &thread_rgbcolor,&thread_aled, &thread_buzzer };


void printTasks(const int *aliveTasks){
    int i = 0;
    printDBG("Current Alive Tasks:  ");
    for(;i<8;i++){
        printDBG("%d ", *(aliveTasks+i));
    }
    printDBG(" :::::: ");
}

void printFreq(int *freq){
    int i = 0;
    printf("Frequency Preference: ");
    for(;i<NUM_TASKS;i++){
        printf("%d ", freq[i]);        
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



float calculate_utilization(int *prefered_freq, long long *w_900, long long *w_600, long long *deadlines){
    float sum = 0.0;
    int i = 0;
    for(;i<NUM_TASKS;i++){
        if(prefered_freq[i] == HIGH){
            sum+= ((float)w_900[i])/deadlines[i];
        }else{
            sum+= ((float)w_600[i])/deadlines[i];
        }
    }

    return sum;
}



int possible(int *prefered_freq, long long *w_900, long long *w_600, long long *deadlines){
    float s = calculate_utilization(prefered_freq, w_900, w_600, deadlines);
    return s <= 1.0;
}

int lessThanOneMaxIndex(float *arr){
    float max = FLT_MIN;
    int idx = -1;
    int i;
    for(i=0;i<NUM_TASKS;i++){
        if(arr[i] > max && arr[i] <= 1.0){
            max = arr[i];
            idx = i;
        }
    }
    return idx;
};

int maxIndex(float *arr){
    float max = FLT_MIN;
    int idx = -1;
    int i;
    for(i=0;i<NUM_TASKS;i++){
        if(arr[i] > max){
            max = arr[i];
            idx = i;
        }
    }
    return idx;
};

int sum(int *p){
    int i = 0, s = 0;
    for(;i<NUM_TASKS;i++){
        s+=p[i];
    }
    return s;
}


int prefered_freq[] = {HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};
int P_900 = 800;
int P_600 = 400;

void learn_workloads(SharedVariable* sv) {    
    long long start, end;    
    long long workloads_900[] = {0,0,0,0,0,0,0,0};
    long long workloads_600[] = {0,0,0,0,0,0,0,0};
    unsigned int i = 0;
    for(i = 0; i<NUM_TASKS;i++){
        
        set_by_max_freq();
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        printDBG("900MHZ  %s: %lld\n",taskNum[i], end-start);
        workloads_900[i] = end-start;

        set_by_min_freq();
        start = get_current_time_us();            
        (*(functions[i]))(sv);        
        end = get_current_time_us();
        printDBG("600MHZ  %s: %lld\n",taskNum[i], end-start);
        workloads_600[i] = end-start;
                
        currentDeadlines[i] = workloadDeadlines[i];
    }
    
    /* get workload_900, workload_600 */
    for(i=0;i<NUM_TASKS;i++){
        if(workloads_900[i]*P_900 > workloads_600[i]*P_600){
            prefered_freq[i] = LOW;
        }
    }
    printFreq(prefered_freq);
    /* check schedulility */

    printf("Begin to check schedulility\n");
    int idx = -1;
    float util[8] = {0,0,0,0,0,0,0,0};
    float u = 2;
    while(u > LIMIT && sum(prefered_freq) != NUM_TASKS){
        idx = -1;
        for(i=0;i<NUM_TASKS;i++){
            if(prefered_freq[i] == LOW){
                prefered_freq[i] = HIGH;
                util[i] = calculate_utilization(prefered_freq,workloads_900, workloads_600, workloadDeadlines);
                prefered_freq[i] = LOW;                
            }
        }

        idx = lessThanOneMaxIndex(util);
        if(idx == -1){
            idx = maxIndex(util);
        }        
        for(i=0;i<NUM_TASKS;i++)
            util[i] = 0.0;                
        prefered_freq[idx] = HIGH;
        
        u = calculate_utilization(prefered_freq, workloads_900, workloads_600, workloadDeadlines);
        printf("util %f :: ",u);
        printFreq(prefered_freq);
    }
    printf("\n Finished with ");
    printFreq(prefered_freq);
    printf("\n");
    
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

void updateCurrentDeadlines(long long time_difference, int* lastAliveTasks, const int* aliveTasks, long long idleTime){
    int i = 0;
    for(;i<NUM_TASKS;i++){
        if(aliveTasks[i] == 1){
            if(lastAliveTasks[i] == 1 && idleTime == 0){
                currentDeadlines[i] = currentDeadlines[i] - time_difference;
            }else{
                currentDeadlines[i] = workloadDeadlines[i];
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
    long long minDead = LONG_MAX;
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
int chooseFreq(int i){
    return prefered_freq[i];
};




long long totalIdleTime = 0;
int lastAliveTasks[] = {0, 0, 0, 0, 0, 0, 0, 0};

void updateLastAliveTasks(const int* aliveTasks){
    int i = 0;
    for(;i<NUM_TASKS;i++){
        lastAliveTasks[i] = *(aliveTasks+i);
    }
}

void printTask(TaskSelection t){
    printDBG("Task  id:%d, freq:%d", t.task, t.freq);
    printDBG("   ::::   ");
}

void printDeadlines(){
    int i = 0;
    printDBG("Current Deadline: ");
    for(;i<NUM_TASKS;i++){
        printDBG("");
        printDBG("%lld  ",currentDeadlines[i]);
    }
    printDBG("  :::::  ");
}

// sv->workloads     => estimated time to execute
// currentDeadlines  => current time to deadline
// workloadDeadlines => period & deadline
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
    
    /* totalIdleTime += idleTime */;
    /* printDBG("Total idleTime is %lld\n", totalIdleTime); */
    
    static long long last_timestamp = -1;
    long long current_timestamp = get_scheduler_elapsed_time_us();
    long long time_difference = 0;
    
    if (last_timestamp != -1){
        time_difference = current_timestamp - last_timestamp;        
    }
    last_timestamp = current_timestamp;
    
    updateCurrentDeadlines(time_difference, lastAliveTasks, aliveTasks, idleTime);
        
    TaskSelection sel;
    sel.task = chooseTask(currentDeadlines, aliveTasks); 
    sel.freq = chooseFreq(sel.task);
    
    
    printTasks(aliveTasks);
    printTask(sel);
    printDeadlines();
    printDBG("idleTime is %lld\n", idleTime);

    updateLastAliveTasks(aliveTasks);
    return sel;
}
