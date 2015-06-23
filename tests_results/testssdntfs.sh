#!/bin/bash
#NTFS SSD
filesystembenchmark /media/gilberto/VOL_T 3 1m   > SSD1MiBNTFS;
filesystembenchmark /media/gilberto/VOL_T 3 50m  > SSD50MiBNTFS;
filesystembenchmark /media/gilberto/VOL_T 3 150m > SSD150MiBNTFS;
filesystembenchmark /media/gilberto/VOL_T 3 250m > SSD250MiBNTFS;
filesystembenchmark /media/gilberto/VOL_T 3 350m > SSD350MiBNTFS;