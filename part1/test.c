#include <stdio.h>
#define STR_CONC(str1,str2,str3,str4,str5) str1 " " str2 " " str3 " " str4 " " str5

int main() {
    char *x = STR_CONC("mcr p15, 0, %0, c9,","Rm",",","op2","\n\t");
    printf(x);

}
