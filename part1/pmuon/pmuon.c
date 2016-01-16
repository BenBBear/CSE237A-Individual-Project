#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("PMUON");

#define ASM(...) asm volatile(__VA_ARGS__);
#define STR_CONC(str1,str2,str3,str4,str5) str1 " " str2 " " str3 " " str4 " " str5
#define MSR(Rm,op2,arm_reg) ASM(STR_CONC("mcr p15, 0, %0, c9, ", Rm,", ", op2, "\n\t") :: "r" (arm_reg));

int init_module(void) {
    	unsigned int v;
    	printk("Turn PMU on\n");

    	// 1. Enable "User Enable Register"
        // MCR{cond} P15, <Opcode_1>, <Rd>, <CRn>, <CRm>, <Opcode_2>
        // P15: coprocessor
        MSR("c14","0",0x00000001);
    	/* ASM("mcr p15, 0, %0, c9, c14, 0\n\t" :: "r" (0x00000001));  */
        


	// 2. Reset Performance Monitor Control Register(PMCR), Count Enable Set Register, and Overflow Flag Status Register
    	asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));
    	asm volatile ("mcr p15, 0, %0, c9, c12, 1\n\t" :: "r"(0x8000000f));
    	asm volatile ("mcr p15, 0, %0, c9, c12, 3\n\t" :: "r"(0x8000000f));

    	// 3. Disable Interrupt Enable Clear Register
    	asm volatile("mcr p15, 0, %0, c9, c14, 2\n\t" :: "r" (~0));

    	// 4. Read how many event counters exist 
    	asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v)); // Read PMCR
    	printk("pmon_init(): have %d configurable event counters.\n", (v >> 11) & 0x1f);


	// 5. Set event counter registers (Project Assignment you need to IMPLEMENT)
        
	return 0;
}

void cleanup_module(void) {
	printk("GOODBYE, PMU Off\n");
}


