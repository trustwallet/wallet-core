# Linux

Trust Wallet Core was developed mainly on macOS because it's the only platform that can compile to every supported platform. Because of this, getting to compile on Linux is a bit of extra work. If you have access to macOS we highly recommend developing on that platform.

## Docker

The easiest way of getting Trust Wallet Core running on a Linux instance is to use the provided Dockerfile. The Dockerfile sets up an Ubuntu container with the necessary dependencies.

To run the pre-built image run `docker run -it trustwallet/wallet-core /bin/bash`. To build the docker image run `docker build .` in the root folder. Use `docker push` if you want to upload your own image to Docker Hub.

## Manual Setup

To run on Linux you need to have the following:

* Clang version 7.0 or higher.
* CMake version 3.12 or higher.
* Protobuf version 3.7 or higher.
* Boost version 1.66 or higher.

Most of these you will have to install from sources because package managers usually don't provide latest versions. You also need to make sure you are compiling with C++17 support.

