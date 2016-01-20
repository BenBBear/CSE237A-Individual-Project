#include <linux/module.h>
#include <linux/kernel.h>
#include "../asm_util.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("PMUON");

void cache_performance_test_with_setup(void){
    printk("\n\nNow use event counters to read data cache performance.\n\n");   
    unsigned int event_counters[] = {FIRST,SECOND,THIRD,FOURTH};
    unsigned int event_types[] = {L1_ACCESS,L1_MISS,L2_ACCESS,L2_MISS};
    char *message[] = {
        "L1 Cache Access Number: %d\n",
        "L1 Cache Miss Number: %d\n",
        "L2 Cache Access Number: %d\n",
        "L2 Cache Miss Number: %d\n"
    };        
    unsigned int len = 4;
    unsigned int i = 0;
    unsigned int v;
    for(;i<len;i++){
        SELECT(event_counters[i]);
        SET_EVENT(event_types[i]);
        READ_EVENT_COUNTER(v);
        printk(message[i],v);
    }
    printk("\n Finished! \n");           
}





int init_module(void) {
    	unsigned int v;
    	printk("Turn PMU on\n");

    	// 1. Enable "User Enable Register"
        // MCR{cond} P15, <Opcode_1>, <Rd>, <CRn>, <CRm>, <Opcode_2>
        // P15: coprocessor
        MCR("c14","0",0x00000001);      
        


	// 2. Reset Performance Monitor Control Register(PMCR), Count Enable Set Register, and Overflow Flag Status Register
    	asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));
    	asm volatile ("mcr p15, 0, %0, c9, c12, 1\n\t" :: "r"(0x8000000f));
    	asm volatile ("mcr p15, 0, %0, c9, c12, 3\n\t" :: "r"(0x8000000f));

    	// 3. Disable Interrupt Enable Clear Register
    	asm volatile("mcr p15, 0, %0, c9, c14, 2\n\t" :: "r" (~0));

    	// 4. Read how many event counters exist 
    	asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v)); // Read PMCR
    	printk("pmon_init(): have %d configurable event counters.\n", TO_INT(v));



        
	// 5. Set event counter registers (Project Assignment you need to IMPLEMENT)
        cache_performance_test_with_setup();
	return 0;
}


void cleanup_module(void) {
	printk("GOODBYE, PMU Off\n");
}

