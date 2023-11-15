#!/bin/sh
IMAGES="goal.png halfstone_1.png halfstone_2.png halfstone_3.png halfstone_4.png man.png object.png saveman.png stone_1.png stone_2.png stone_3.png stone_4.png stone_5.png stone_6.png treasure.png starfield.png IDK.png"

list=""; for i in $IMAGES; do list="$list $i"; done; 
./bin2c "" $list

