#!/bin/bash
#
# This script uploads android build to bintray.

tools/generate-files

set -e

version=`git describe --long --tags | cut -f 1 -d "-"`
export ANDROID_HOME="$HOME/Library/Android/sdk"
pushd android
./gradlew clean build assembleRelease bintrayUpload -Pversion="$version"
echo "Android build uploaded"
