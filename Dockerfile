FROM ubuntu:18.04

ARG PROTOBUF_VERSION=3.9.0

# Install some basics
RUN apt-get update \
    && apt-get install -y \
        wget \
        curl \
        git \
        vim \
        unzip \
        xz-utils \
        software-properties-common

# Add latest cmake/boost
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - \
    && apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'

RUN apt-add-repository -y ppa:mhier/libboost-latest

# Install required packages for dev
RUN apt-get update \
    && apt-get install -y \
        build-essential \
        libtool autoconf pkg-config \
        ninja-build \
        ruby-full \
        clang-9 \
        llvm-9 \
        libc++-dev libc++abi-dev \
        cmake \        
        libboost1.70-dev

# Clone repo
ENV CC=/usr/bin/clang-9
ENV CXX=/usr/bin/clang++-9
RUN git clone https://github.com/TrustWallet/wallet-core.git

# Prepare dependencies
RUN cd /wallet-core \
    && export PREFIX=/usr/local \
    && tools/install-dependencies

# Clean Up
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Build library: generate, cmake, and make
RUN cd /wallet-core \
    && ./tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12

CMD ["/bin/bash"]
