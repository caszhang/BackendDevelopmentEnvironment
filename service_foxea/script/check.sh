#!/bin/sh
#zhangguoqiang
source ./const_def.sh
echo -e "\n======================================="
ret=`ps -ef|grep $exe_bin_name | grep -v check_$exe_bin_name.sh |  grep -v grep|grep -v gdb |grep -v ddu | wc -l`
if(($ret==0))
then
        echo "$exe_bin_name is not exist!"
elif(($ret==1))
then
        echo "$exe_bin_name is running!"
fi
echo -e "=======================================\n"

