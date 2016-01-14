#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("PMUON");

int init_module(void) {
    	unsigned int v;
    	printk("Turn PMU on\n");

    	// 1. Enable "User Enable Register"
    	asm volatile("mcr p15, 0, %0, c9, c14, 0\n\t" :: "r" (0x00000001)); 

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


