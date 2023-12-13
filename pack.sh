#!/bin/bash
current_dir=$(cd $(dirname $0);pwd)
cd ${current_dir}
if [ $# -lt 1 ]; then
	echo "pack.sh"
	echo -e "\t1:get build result"
	echo -e "\t2:sync build result"
	exit -1
fi
case $1 in
	1)
        rm -rf archive_dir
        mkdir archive_dir
        cd archive_dir

        # J5 SDK
        mkdir -p sdk/J5_Common/include/
        cp -r  ${current_dir}/include/cutils                    sdk/J5_Common/include/
        mkdir -p  sdk/J5_Common/lib/release
        cp -rf ${current_dir}/out/j5/libcutils.a                sdk/J5_Common/lib/release/
        
        # TDA4 SDK
        mkdir -p sdk/TDA4VH_Common/include/
        cp -r  ${current_dir}/include/cutils                     sdk/TDA4VH_Common/include/
        mkdir -p sdk/TDA4VH_Common/lib/release 
        cp -rf ${current_dir}/out/tda4vh/libcutils.a             sdk/TDA4VH_Common/lib/release/

        cp ${current_dir}/$0 .
		;;
	2)
		if [ $# != 2 ]; then
            echo "please input workspace path for repository/sdk "
            exit -1
        fi
        workspace=$2
        if [ -d ${workspace}/sdk/ ];then
            cp -r sdk/* ${workspace}/sdk/
        else 
           echo "sdk dir is not exit"
           exit 2
        fi
		;;
	*)
		echo "params error"
		exit 3
		;;
esac
