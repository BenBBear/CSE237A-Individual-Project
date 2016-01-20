#include <stdio.h>

#define FIRST 0x0
#define SECOND 0x1
#define THIRD 0x2
#define FOURTH 0x4

#define L1_ACCESS 0x13
#define L1_MISS 0x15
#define L2_ACCESS 0x16
#define L2_MISS 0x17
#define STR_CONC(str1,str2,str3,str4,str5) str1 " " str2 " " str3 " " str4 " " str5

int main() {
    char *x = STR_CONC("mcr p15, 0, %0, c9,","Rm",",","op2","\n\t");
    printf(x);
    unsigned int event_counters = {FIRST,SECOND,THIRD,FOURTH};
    char *message[] = {
        "L1 Cache Access Number: %d\n",
        "L1 Cache Miss Number: %d\n",
        "L2 Cache Access Number: %d\n",
        "L2 Cache Miss Number: %d\n"
    };
}
