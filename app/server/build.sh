#!/bin/sh

SOURCE_DIR=$(pwd)/../..
ROOT=$SOURCE_DIR
BUILD_DIR=$ROOT/../server-build
CMAKE_ARGS="$1 -C$ROOT/config/configurations/server.cmake"
PROJ_NAME="rpiGreenHouse_server"
PROJ_TARGET="rpi_server"
TARGET_UUID="{b91582c8-2694-46d2-8264-c37861ff70b2}" # SERVER

TOOLCH_FILE="/home/michael/development/git/toolchain/usr/share/buildroot/toolchainfile.cmake"
#TOOLCH_FILE="/home/michael/development/git/toolchainfile_clang.cmake"
#TOOLCH_FILE="/opt/tools/usr/share/buildroot/toolchainfile.cmake"

echo $SOURCE_DIR/config/build_project.sh "$ROOT" "$SOURCE_DIR" "$BUILD_DIR" "$CMAKE_ARGS" "$PROJ_NAME" "$PROJ_TARGET" "$TARGET_UUID" "$TOOLCH_FILE"
$ROOT/config/build_project.sh $ROOT $SOURCE_DIR $BUILD_DIR $CMAKE_ARGS $PROJ_NAME $PROJ_TARGET $TARGET_UUID $TOOLCH_FILE
