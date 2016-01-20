#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("PMUON");

#define ASM(...) asm volatile(__VA_ARGS__);
#define STR_CONC(s,str1,str2,str3,str4,str5) s " " str1 " " str2 " " str3 " " str4 " " str5
#define _MCR(s, Rm,op2, r,arm_reg) ASM(STR_CONC(s," p15, 0, %0, c9, ", Rm,", ", op2, "\n\t") :: r (arm_reg));
#define _MRC(s, Rm,op2, r,arm_reg) ASM(STR_CONC(s," p15, 0, %0, c9, ", Rm,", ", op2, "\n\t") : r (arm_reg));
#define MCR(Rm,op2,arm_reg) _MCR("mcr",Rm,op2,"r",arm_reg);
#define MRC(Rm,op2,arm_reg) _MRC("mrc",Rm,op2,"=r",arm_reg);

#define FIRST 0x0
#define SECOND 0x1
#define THIRD 0x2
#define FOURTH 0x4

#define L1_ACCESS 0x13
#define L1_MISS 0x15
#define L2_ACCESS 0x16
#define L2_MISS 0x17

#define TO_INT(v) (v >> 11) & 0x1f

#define SELECT(num) MCR("c12", "5", num)
#define SET_EVENT(eventId) MCR("c13","1", eventId)
#define READ_EVENT_COUNTER(v) MRC("c13","2",v)
    
/* 0x13	[17]	Data memory access. */
/* 0x15	[19]	Data cache eviction. */
/* 0x16	-	Level 2 data cache access */
/* 0x17	-	Level 2 data cache refill */


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
        for(;i<len;i++){
            SELECT(event_counters[i]);
            SET_EVENT(event_types[i]);
            READ_EVENT_COUNTER(v);
            printk(message[i],v);
        }
        printk("\n Finished! \n");           
	return 0;
}

void cleanup_module(void) {
	printk("GOODBYE, PMU Off\n");
}


