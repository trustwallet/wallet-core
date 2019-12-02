# Building

Find here instructions for building the Wallet Core library locally.

## Platforms

The following *target platforms* are supported:

* **iOS** (Swift language is used)
* **Android**

The following *development platforms* are supported:

* **MacOS**: is the *primary* development platform, as it supports compiling for both target platforms.
* **Linux**: is partially supported as a development platform.

## Build Methods

Wallet Core can be build inside the provided Docker image, or natively.

* Inside **Docker** image:  This way is easier to get the prerequisites, as they are all inside the provided Docker image.  However, all building has to be done inside the Docker image.
* Natively, in your dev OS (macOS, Linux).

## Prerequisites

Here is the list of required prerequisites.

### Prerequisites on MacOS

* CMake `brew install cmake`
* Boost `brew install boost`
* Xcode
* Xcode command line tools: `xcode-select --install`
* Other tools: `brew install git ninja autoconf automake libtool xcodegen clang-format`
* Cocoapods for iOS: `sudo gem install cocoapods`
* [Android Studio](https://developer.android.com/studio/index.html)
* [Android NDK](https://developer.android.com/ndk/guides/)

### Prerequisites on Linux

* Various packages `sudo apt-get install autoconf git libcurl4-openssl-dev libicu-dev libssl-dev libtool ninja-build nodejs pkg-config unzip`
* CLang
* Ruby 2.6 (`rbenv` from https://github.com/rbenv/ruby-build, then `rbenv install 2.6.3`)
* Boost (from https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz)
* CMake (from https://github.com/Kitware/CMake/releases)
* [Android Studio](https://developer.android.com/studio/index.html)

### Managed Prerequisites

Additionally, the following prerequisites are also needed, but they are managed by Wallet Core.  The script `tools/install-dependencies` downloads and compiles them (see below).

* Google test
* Check
* Json
* Protobuf

## Full Build

The full build can be triggered with one top-level script:

    ./bootstrap.sh

Or, broken up in smaller steps:

    ./tools/install-dependencies

This script downloads and compiles some prerequisites.

    ./tools/generate-files

This script generates source files, coin- and protobuf files.  Needs to be re-run whenever `coins.json` or protobuf files are changes.

    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
    make -Cbuild

Standard CMake and make commands for building the library.  The `cmake` command needs to be re-run whenever a new source file is added.

After build, *unit tests* can be executed:

    ./build/tests/tests tests --gtest_filter=*

If you'd rather use and IDE for building and debugging you can specify the `-G` option to cmake. For instance to use Xcode call `cmake -Bxcode -GXcode -DCMAKE_BUILD_TYPE=Debug` and use the generated project in the xcode folder.

## Docker

Here are the instructions to build Wallet Core within the provided Docker image.

Prerequisite is a working Docker installation.

The command for starting a new Docker container with the wallet-core image, and a shell within it:

    docker run -i -t trustwallet/wallet-core /bin/bash

Inside the container the build commands can be executed (as described above; note that install-dependencies is not necessary):

    cd wallet-core
    ./tools/generate-files
    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
    make -Cbuild

## Linux

Building on Linux is possible, but not fully supported, it requires some extra work. If you have access to macOS we highly recommend developing on that platform.  Using the Docker image is also recommended.
Otherwise, the prerequisites have to be installed manually.
