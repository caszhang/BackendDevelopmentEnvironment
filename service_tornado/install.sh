#!/bin/sh
function mkDir()
{
    if [ ! -d $1 ];then
        mkdir $1
        echo "mkdir "$1
    else
        echo $1" already exist"
    fi
}
echo "------------begin---------"
mkDir "src"
mkDir "conf"
mkDir "lib"
mkDir "data"
mkDir "include"
mkDir "script"
mkDir "ctrl"
mkDir "log"
mkDir "bin"
echo "-------------ok-----------"
