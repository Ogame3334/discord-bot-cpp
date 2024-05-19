#!/bin/bash
 
DIR="build"

cd /root/ogame-notify
 
if [ ! -d $DIR ];then
  mkdir $DIR
fi

echo "Asia/Tokyo" > /etc/timezone

cd $DIR
cmake ..
make
./ogame-notify
