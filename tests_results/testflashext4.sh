#!/bin/bash
#Ext4 Flash
sudo filesystembenchmark /media/gilberto/2d9887e7-2ad9-4438-bbea-f8aa6b640aed 3 1m   > Flash1MiBExt4;
sudo filesystembenchmark /media/gilberto/2d9887e7-2ad9-4438-bbea-f8aa6b640aed 3 50m  > Flash50MiBExt4;
sudo filesystembenchmark /media/gilberto/2d9887e7-2ad9-4438-bbea-f8aa6b640aed 3 150m > Flash150MiBExt4;
