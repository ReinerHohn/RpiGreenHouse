#!/bin/sh

SOURCE_DIR=$1
BUILD_DIR=$2
CMAKE_ARGS=$3
PROJ_NAME=$4
PROJ_TARGET=$5
TARGET_UUID=$6
TOOLCH_FILE=$7

SOURCE_DIR2=$(readlink -m $SOURCE_DIR)
BUILD_DIR2=$(readlink -m $BUILD_DIR)

# Delete build folder if it exists
if [ -d "$BUILD_DIR2" ]; then
	rm -r $BUILD_DIR2
fi

# Then recreate it!
mkdir -p "$BUILD_DIR2"

# Generate CMakeLists.txt.user (holding project information)
cd ../config && python "qtcreator_project.py" "$SOURCE_DIR2" "$BUILD_DIR2" "$CMAKE_ARGS" "$PROJ_NAME" "$PROJ_TARGET" "$TARGET_UUID"

# Generate Makefiles with CMake
cd $BUILD_DIR2 && cmake -DCMAKE_TOOLCHAIN_FILE=$TOOLCH_FILE -G"Unix Makefiles" $SOURCE_DIR2 # Ninja  

# Start qtcreator in project folder
/home/michael/qtcreator-3.5.82/bin/qtcreator $SOURCE_DIR2 & 


