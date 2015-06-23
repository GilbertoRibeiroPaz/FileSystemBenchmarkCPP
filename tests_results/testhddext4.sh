#!/bin/bash
#Ext4 HDD
sudo filesystembenchmark /media/gilberto/2c75216f-8dbf-4bcd-8f48-6ef6b8f2183d 3 1m   > HDD1MiBExt4;
sudo filesystembenchmark /media/gilberto/2c75216f-8dbf-4bcd-8f48-6ef6b8f2183d 3 50m  > HDD50MiBExt4;
sudo filesystembenchmark /media/gilberto/2c75216f-8dbf-4bcd-8f48-6ef6b8f2183d 3 150m > HDD150MiBExt4;
