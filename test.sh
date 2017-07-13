#!/bin/sh

# send sensor data at one second intarvals.

PATH_TO_MQTTSNCLIENT=
ID=$1
HOSTIP=
TOPIC=
CLIENT-ID=

while :
do
	MESSAGE=$ID,13,1,1,1,`date +%Y/%m/%d`,`date +%H:%M:%S`,34.694305,135.269547,13.7,1,0,0,0,1000,32,,,,,,,,0.000196,1,0,0,0,0
	$PATH_TO_MQTTSNCLIENT/mqttsnclient -h $HOSTIP -t $TOPIC -c $CLIENT-ID -m $MESSAGE
	sleep 1
done
