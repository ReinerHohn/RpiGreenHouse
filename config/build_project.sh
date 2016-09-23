#!/bin/sh

SOURCE_DIR=$1
BUILD_DIR=$2
TOOLCH_FILE=$3

rm -r $BUILD_DIR

if [ ! -d "$BUILD_DIR" ]; then
	echo "$BUILD_DIR"
	mkdir -p "$BUILD_DIR"
fi

echo SOURCE_DIR $SOURCE_DIR
echo BUILD_DIR $BUILD_DIR
echo TOOLCH_FILE $TOOLCH_FILE

cd $BUILD_DIR && echo "PDW" && pwd && cmake -DCMAKE_TOOLCHAIN_FILE=$TOOLCH_FILE -G "Unix Makefiles" $SOURCE_DIR # Ninja  


