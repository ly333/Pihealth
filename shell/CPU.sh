#!/bin/bash

#输出时间
NowTime=`date +"%y-%m-%d_%h:%m:%s"`

#输出前15分钟的负载
LoadAvg=`cut -d " " -f 1-3 /proc/loadavg`
#输出温度
CpuTime=`cat /sys/class/thermal/thermal_zone0/temp`
CpuTemp=`echo "scale=2;$CpuTime/1000" | bc`

#eval将里面的语句产生的结果当成shell脚本的执行
#第一个时间cpu的使用情况
eval $(head -n 1 /proc/stat | awk -v sum1=0 -v idel1=0 \
    '{for (i=2;i<=8;i++) {sum1=sum1+$i} printf("sum1=%d;idel1=%d;", sum1,$5)}'
    )

sleep 0.5

#第二个时间cpu使用情况
eval $(head -n 1 /proc/stat | awk -v sum2=0 -v idel2=0 \
    '{for (i=2;i<=8;i++) {sum2=sum2+$i} printf("sum2=%d;idel2=%d;", sum2,$5)}'
    )

CpuUsedPerc=`echo "scale=4;(1-($idel1-$idel2)/($sum1-$sum2))*100" | bc`
CpuUsedPerc=`printf "%.2f\n" $CpuUsedPerc`

#警告等级
WarnLevel="normal"
if [[ `echo $CpuTemp '>=' 85 | bc -l` = 1 ]]; then
    WarnLevel="fault"
elif [[ `echo $CpuTemp '>=' 70 | bc -l` = 1 ]]; then
    WarnLevel="warning"
elif [[ `echo $CpuTemp '>=' 50 | bc -l` = 1 ]]; then
    WarnLevel="note"
fi

CpuLog="./cpu.log"
echo "$NowTime $LoadAvg $CpuUsedPerc ${CpuTemp}摄氏度 $WarnLevel" >> cpu.log





