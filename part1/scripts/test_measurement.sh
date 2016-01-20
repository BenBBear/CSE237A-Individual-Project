#!sh
sudo insmod ../pmuon/pmuon.ko
../memmeasurement/memmeasurement
sudo rmmod pmuon
dmesg
