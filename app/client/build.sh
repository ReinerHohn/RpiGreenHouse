#!/bin/sh

SOURCE_DIR=$(pwd)
BUILD_DIR="../client-build"
CMAKE_ARGS="" # "-DCMAKE_BUILD_TYPE=Debug"
PROJ_NAME="rpiGreenHouse_client"
PROJ_TARGET="rpi_client"
TARGET_UUID="{e0e941c2-ac8b-48d3-839c-f4c4c7039296}" # CLIENT

TOOLCH_FILE="/home/michael/development/git/toolchain/usr/share/buildroot/toolchainfile.cmake"



../config/build_project.sh "$SOURCE_DIR" "$BUILD_DIR" "$CMAKE_ARGS" "$PROJ_NAME" "$PROJ_TARGET" "$TARGET_UUID" "$TOOLCH_FILE"
