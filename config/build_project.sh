#!/bin/sh

SOURCE_DIR=$1
BUILD_DIR=$2
TOOLCH_FILE=$3

SOURCE_DIR2=$(readlink -m $SOURCE_DIR)
BUILD_DIR2=$(readlink -m $BUILD_DIR)

rm -r $BUILD_DIR2

if [ ! -d "$BUILD_DIR2" ]; then
	echo "$BUILD_DIR2"
	mkdir -p "$BUILD_DIR2"
fi

echo SOURCE_DIR $SOURCE_DIR2
echo BUILD_DIR $BUILD_DIR2
echo TOOLCH_FILE $TOOLCH_FILE

cd ../config && python "qtcreator_project.py" "$SOURCE_DIR2" "$BUILD_DIR2"

cd $BUILD_DIR2 && echo "PDW" && pwd && cmake -DCMAKE_TOOLCHAIN_FILE=$TOOLCH_FILE -G"Unix Makefiles" $SOURCE_DIR2 # Ninja  

/home/michael/qtcreator-3.5.82/bin/qtcreator $SOURCE_DIR2 & 


