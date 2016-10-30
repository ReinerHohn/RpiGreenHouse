#!/bin/sh

SOURCE_DIR=$(pwd)
BUILD_DIR="../server-build"
CMAKE_ARGS="$1"
PROJ_NAME="rpiGreenHouse_server"
PROJ_TARGET="rpi_server"
TARGET_UUID="{b91582c8-2694-46d2-8264-c37861ff70b2}" # SERVER

TOOLCH_FILE="/home/michael/development/git/toolchain/usr/share/buildroot/toolchainfile.cmake"



../config/build_project.sh "$SOURCE_DIR" "$BUILD_DIR" "$CMAKE_ARGS" "$PROJ_NAME" "$PROJ_TARGET" "$TARGET_UUID" "$TOOLCH_FILE"
