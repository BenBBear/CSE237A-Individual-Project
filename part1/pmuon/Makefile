obj-m:=pmuon.o
KDIR:=/home/sayen/Documents/work/CSE237A/linux
PWD:=$(shell pwd)
ARCH ?=arm
CROSS_COMPILE ?=arm-linux-gnueabihf-

default:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules



clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) clean
