function show_success()
{
	echo "     xxxx                                                                              "
	echo "   xxxxxxxx                                                                            "
	echo "  xxxx xxxxx                                                                           "
	echo "  xxx   xxxx                                                                           "
	echo "  xxxx    xxx                                                                           "
	echo "  xxx                                                                                  "
	echo "  xxxx        xxx    xxxx   xxxxxxx      xxxxxxx     xxxxxxx     xxxxxxx      xxxxxxx  "
	echo "   xxxxxx     xxx    xxxx  xxxxxxxxx    xxxxxxxxx   xxxxxxxxx   xxxxxxxxx    xxxxxxxxx "
	echo "    xxxxxx    xxx    xxxx xxxx   xxxx  xxxx  xxxx  xxxx   xxx   xxx   xxxx  xxxx   xxx "
	echo "       xxxxx  xxx    xxxx xxxx   xxxx xxxx    xxx  xxx    xxxx  xxx         xxxx       "
	echo "        xxxx  xxx    xxxx xxx         xxxx         xxxxxxxxxxx  xxxxxx       xxxxx     "
	echo " xxxx    xxxx xxx    xxxx xxx         xxxx         xxx            xxxxxxx     xxxxxxx  "
	echo " xxxx    xxxx xxx    xxxx xxx     xxx xxxx    xxxx xxx                xxxx        xxxx "
	echo " xxxx    xxx  xxx   xxxxx xxxx   xxxx  xxx    xxx  xxxx   xxxx  xxx   xxxx  xxx    xxx "
	echo "  xxxx  xxxx  xxxx  xxxxx  xxxx  xxxx  xxxx  xxxx   xxxx  xxx   xxx   xxxx  xxxx  xxxx "
	echo "   xxxxxxxx    xxxxxxxxxx   xxxxxxxx    xxxxxxxx    xxxxxxxxx   xxxxxxxxx    xxxxxxxxx "
	echo "    xxxxx       xxxx xxxx    xxxxx        xxxx        xxxxx       xxxxx        xxxxx   "
}

function show_fail()
{

	echo "xxxxxxxxxxxx                             xxxx                                          "
	echo "xxxxxxxxxxxx                 xxx         xxxx                                          "
	echo "xxxx                         xxx         xxxx                                          "
	echo "xxxx                                     xxxx                                          "
	echo "xxxx                                     xxxx                                          "
	echo "xxxx                                     xxxx                                          "
	echo "xxxx           xxxxxxx       xxx         xxxx     xxx    xxxx   xxx xxxx     xxxxxx    "
	echo "xxxx          xxxxxxxxx      xxx         xxxx     xxx    xxxx   xxxxxxxx    xxxxxxxxx  "
	echo "xxxxxxxxxx   xxxx   xxx      xxx         xxxx     xxx    xxxx   xxxxx      xxxx  xxxx  "
	echo "xxxx                xxx      xxx         xxxx     xxx    xxxx   xxxx       xxx    xxx  "
	echo "xxxx            xxxxxxx      xxx         xxxx     xxx    xxxx   xxx       xxxxxxxxxxx  "
	echo "xxxx          xxxxxxxxx      xxx         xxxx     xxx    xxxx   xxx       xxxx         "
	echo "xxxx         xxxx   xxx      xxx         xxxx     xxx    xxxx   xxx       xxxx         "
	echo "xxxx         xxx   xxxx      xxx         xxxx     xxxx   xxxx   xxx        xxx    xxxx "
	echo "xxxx         xxx  xxxxx      xxx         xxxx     xxxx  xxxxx   xxx        xxxx  xxxx  "
	echo "xxxx         xxxxxxxxxx      xxx         xxxx      xxxxxxxxxx   xxx         xxxxxxxx   "
	echo "xxxx           xxxx xxxx     xxx         xxxx       xxxx xxxx   xxx           xxxxx    "
}

VERSION=$(sed -n 2p versionHistory* | awk -F ': ' '{print $2}')

mkdir -p out
mkdir -p example/out

rm -rf out/*
rm -rf example/out/*

cd out
cmake -DPLATFORM=linux -DBUILD_MODE=debug ..;make clean;make -j32
if [ $? -ne 0 ];then
	show_fail
	exit -1
fi

# cp -rf libcutils.so libcutils.so.$VERSION
cd -

cd example/out
cmake -DPLATFORM=linux -DBUILD_MODE=debug ..;make clean;make -j32
if [ $? -ne 0 ];then
	show_fail
	exit -1
fi
cd -

show_success

exit 0
