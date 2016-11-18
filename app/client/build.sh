#!/bin/sh

SOURCE_DIR=$(pwd)/../..
BUILD_DIR=$SOURCE_DIR/../client-build

CMAKE_ARGS="$1 -C$SOURCE_DIR/config/configurations/client.cmake"
TOOLCH_FILE="/home/michael/development/git/toolchain/usr/share/buildroot/toolchainfile.cmake"
#TOOLCH_FILE="/home/michael/development/git/toolchainfile_clang.cmake"
#TOOLCH_FILE="/opt/tools/usr/share/buildroot/toolchainfile.cmake"

$SOURCE_DIR/config/build_project.sh "$SOURCE_DIR" "$BUILD_DIR" "$CMAKE_ARGS" "$TOOLCH_FILE"
