#!/bin/sh
#zhangguoqiang
#start search_foxea module
source ./const_def.sh
cd  ../bin
cp $src_bin $exe_bin_name

export LD_LIBRARY_PATH=../lib;ulimit -c unlimited; ulimit -s 2048;
ulimit -c unlimited

echo -e "\n\n======================================="

ret=`ps -ef|grep $exe_bin_name | grep -v check_$exe_bin_name.sh | grep -v grep|grep -v gdb |grep -v ddu | wc -l`
echo ret=$ret
if(($ret==0))
then
	nohup ./$exe_bin_name >/dev/null &
	echo "start $exe_bin_name!"
elif(($ret==1))
then
	echo "$exe_bin_name is already running!"
fi


pid=`ps -ef|grep $exe_bin_name | grep -v  check_$exe_bin_name.sh | grep -v grep|grep -v gdb | grep -v ddu | awk '{print $2}'`
echo "pid=$pid"


echo -e "=========================================\n\n"

