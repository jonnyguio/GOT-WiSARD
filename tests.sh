#!/bin/sh

CONFIDENCETHRESHOLD=0.05
NUMBITSADDRSSTART=2
NUMBITSADDRSJUMP=2
NUMBITSADDRSMAX=61

MAXRIGHTS0=0
MAXRIGHTS1=0

make
       
NUMBITSADDRS=$NUMBITSADDRSSTART
while [ $NUMBITSADDRS -lt $NUMBITSADDRSMAX ]
do
        ./bin/GOT-WiSARD.out 0 $NUMBITSADDRS 1 > ./results/$NUMBITSADDRS-1.txt
        ./bin/GOT-WiSARD.out 0 $NUMBITSADDRS 1 > ./results/$NUMBITSADDRS-2.txt
        ./bin/GOT-WiSARD.out 0 $NUMBITSADDRS 1 > ./results/$NUMBITSADDRS-3.txt
        ./bin/GOT-WiSARD.out 0 $NUMBITSADDRS 1 > ./results/$NUMBITSADDRS-4.txt
        ./bin/GOT-WiSARD.out 0 $NUMBITSADDRS 1 > ./results/$NUMBITSADDRS-5.txt
        # echo "Done ./results/$NUMBITSADDRS"
        line10=$(sed -n '1p' ./results/$NUMBITSADDRS-1.txt)
        line20=$(sed -n '1p' ./results/$NUMBITSADDRS-2.txt)
        line30=$(sed -n '1p' ./results/$NUMBITSADDRS-3.txt)
        line40=$(sed -n '1p' ./results/$NUMBITSADDRS-4.txt)
        line50=$(sed -n '1p' ./results/$NUMBITSADDRS-5.txt)
        line11=$(sed -n '2p' ./results/$NUMBITSADDRS-1.txt)
        line21=$(sed -n '2p' ./results/$NUMBITSADDRS-2.txt)
        line31=$(sed -n '2p' ./results/$NUMBITSADDRS-3.txt)
        line41=$(sed -n '2p' ./results/$NUMBITSADDRS-4.txt)
        line51=$(sed -n '2p' ./results/$NUMBITSADDRS-5.txt)
        media0=$((($line10+$line20+$line30+$line40+$line50)/5))
        media1=$((($line11+$line21+$line31+$line41+$line51)/5))
        if [ $media0 -gt $MAXRIGHTS0 ]
        then
            MAXRIGHTS0=$media0
        fi
        if [ $media1 -gt $MAXRIGHTS1 ]
        then
            MAXRIGHTS1=$media1
        fi
        echo "Medias (numBitsAddrs = $NUMBITSADDRS): Word = $media0, Tag = $media1"
    NUMBITSADDRS=$(($NUMBITSADDRS + $NUMBITSADDRSJUMP))
done
echo "Max rights word: $MAXRIGHTS0 - Max rights tag: $MAXRIGHTS1"