#!/bin/sh

SOURCE_DIR=$1
BUILD_DIR=$2
CMAKE_ARGS=$3
TOOLCH_FILE=$4

SOURCE_DIR2=$(readlink -m $SOURCE_DIR)
BUILD_DIR2=$(readlink -m $BUILD_DIR)

# Delete build folder if it exists
if [ -d "$BUILD_DIR2" ]; then
	rm -r $BUILD_DIR2
fi

# Then recreate it!
mkdir -p "$BUILD_DIR2"

# Generate Makefiles with CMake
cd $BUILD_DIR2 && cmake -DCMAKE_TOOLCHAIN_FILE=$TOOLCH_FILE -G"Unix Makefiles" $SOURCE_DIR2 $CMAKE_ARGS # Ninja  

# Start qtcreator in project folder
/home/michael/qtcreator-3.5.82/bin/qtcreator $SOURCE_DIR2 & 


