#!sh
sudo insmod `pwd`/../pmuon/pmuon.ko
sudo ../memmeasurement/memmeasurement
sudo rmmod pmuon
dmesg
