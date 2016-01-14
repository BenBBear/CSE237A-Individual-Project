#include <linux/module.h>
#include <linux/kernel.h>


int init_module(void){
    printk(KERN_INFO "CSE237A: Hello world.\n");
}

void cleanup_module(void){
    printk(KERN_INFO "CSE237A: Goodbye world.\n");
}
