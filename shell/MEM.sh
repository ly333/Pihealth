#!/bin/bash

Time=`date +"%y-%m-%d %h:%m:%s"`

Info=(`free -m | head -n 2 | tail -n 1 | awk '{printf("%d %d %d", $2, $3, $5)}'`)

Total=${Info[0]}

Used=${Info[1]}

Free=${Info[2]}

AverUsage=`echo "scale=2;$Used/$Total" | bc`

echo $Time $Total $Used $Free $AverUsage >> mem.log
