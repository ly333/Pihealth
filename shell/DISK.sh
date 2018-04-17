#!/bin/bash

Time=`date +"%Y-%m-%d_%H.%m.%s"`

eval $(df -T -x tmpfs -m -x devtmpfs | tail -n +2 | awk \
    '{printf("paramount["NR"]=%d;paraleft["NR"]=%d;pararate["NR"]=%d;\
    paratype["NR"]=%s\n", $3,$5,$6,$7);DiskSum+=$3;LeftSum+=$5;}\
    END{printf("paracount=%d;DiskSum=%d;LeftSum=%d\n", NR,DiskSum,\
    LeftSum)}')

for (( i = 1; i <= $paracount; i++ )); do
    echo $Time 1 ${paratype[$i]} ${paramount[$i]} ${paraleft[$i]} \
    ${pararate[$i]}% >> disk.log
done

UsedPercSum=$[ (100-$LeftSum*100/$DiskSum) ]

echo $Time 0 disk $DiskSum $LeftSum $UsedPercSum% >> disk.log
