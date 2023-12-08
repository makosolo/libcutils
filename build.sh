#!/usr/bin/env bash

VERSION=$(sed -n 2p versionHistory* | awk -F ': ' '{print $2}')

mkdir -p out/$1
mkdir -p example/out/$1

rm -rf out/$1/*
rm -rf example/out/$1/*

cd out/$1
cmake -DPLATFORM=$1 -DBUILD_MODE=$2 ../../;make clean;make -j32
if [ $? -ne 0 ];then
	exit -1
fi

cd -

cd example/out/$1
cmake -DPLATFORM=$1 -DBUILD_MODE=$2 ../../;make clean;make -j32
if [ $? -ne 0 ];then
	exit -1
fi
cd -

exit 0
