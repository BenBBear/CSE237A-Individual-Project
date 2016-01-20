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

#define L1_ACCESS 0x3
#define L1_MISS 0x4
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



unsigned int read_cycle(void){
    unsigned int v;
    MRC("c13","0",v);    
    return v;
}
