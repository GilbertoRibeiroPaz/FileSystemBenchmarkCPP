#!/bin/bash
#NTFS Flash
filesystembenchmark /media/gilberto/VOL_P 3 1m   > Flash1MiBNTFS;
filesystembenchmark /media/gilberto/VOL_P 3 50m  > Flash50MiBNTFS;
filesystembenchmark /media/gilberto/VOL_P 3 150m > Flash150MiBNTFS;
filesystembenchmark /media/gilberto/VOL_P 3 250m > Flash250MiBNTFS;
filesystembenchmark /media/gilberto/VOL_P 3 350m > Flash350MiBNTFS;