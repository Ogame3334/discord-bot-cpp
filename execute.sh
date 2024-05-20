#!/bin/bash
 
DIR="build"

cd /root/ogame-notify
 
if [ ! -d $DIR ];then
  mkdir $DIR
fi

cd $DIR
cmake ..
make
./ogame-notify
