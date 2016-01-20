CC=$(CROSS_COMPILE)gcc
ARCH ?=arm
CROSS_COMPILE ?=arm-linux-gnueabihf-

all:
	$(CC) -o memmeasurement memmeasurement.c governor_part1.c -static

clean:
	rm memmeasurement
