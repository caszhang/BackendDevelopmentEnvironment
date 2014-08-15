#!/bin/sh
#caszhang

src_bin=service_foxea
exe_bin_name=run_service_foxea
export LD_LIBRARY_PATH=.:../lib:$LD_LIBRARY_PATH
ulimit -c unlimited
ulimit -s 2048;
#ulimit -n 65535

