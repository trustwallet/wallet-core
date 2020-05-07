FROM ubuntu:18.04

ARG CLANG_VERSION=7.0.1
ARG CMAKE_VERSION=3.13.4
ARG PROTOBUF_VERSION=3.9.0

# Install some basics
RUN apt-get update \
    && apt-get install -y \
        curl \
        git \
        nano \
        unzip \
        xz-utils \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Install required packages for dev
RUN apt-get update \
    && apt-get install -y \
        autoconf \
        build-essential \
        libcurl4-openssl-dev \
        libicu-dev \
        libreadline-dev \
        libssl-dev \
        libtool \
        ninja-build \
        pkg-config \
        ruby-full \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Install clang
ENV CLANG_VERSION=$CLANG_VERSION
RUN curl -fSsL http://releases.llvm.org/$CLANG_VERSION/clang+llvm-$CLANG_VERSION-x86_64-linux-gnu-ubuntu-18.04.tar.xz -o clang.tar.xz \
    && tar -xJf clang.tar.xz --directory /usr --strip-components=1 \
    && rm -rf clang.tar.xz

# Install Boost
RUN curl -fSsL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz -o boost.tar.gz \
    && tar xzf boost.tar.gz \
    && mv boost_1_66_0/boost /usr/include \
    && rm -rf boost*

# Install CMake, binaries from GitHub
ENV CMAKE_VERSION=$CMAKE_VERSION
RUN cd /usr/local/src \
    && curl -fSsOL https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-Linux-x86_64.tar.gz \
    && ls -l cmake-$CMAKE_VERSION-Linux-x86_64.tar.gz \
    && tar xf cmake-$CMAKE_VERSION-Linux-x86_64.tar.gz \
    && cd cmake-$CMAKE_VERSION-Linux-x86_64 \
    && cp -r bin /usr/ \
    && cp -r share /usr/ \
    && cp -r doc /usr/share/ \
    && cp -r man /usr/share/ \
    && cd .. \
    && rm -rf cmake*
RUN cmake --version

# Clone repo
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
RUN git clone https://github.com/TrustWallet/wallet-core.git

# Prepare dependencies
RUN cd /wallet-core \
    && export PREFIX=/usr/local \
    && tools/install-dependencies

# Clean Up
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Version checks
RUN ruby --version \
    && clang --version \
    && cmake --version \
    && protoc --version

# Build library: generate, cmake, and make
# rbenv trick is needed as Ruby is installed to non-standard location
RUN cd /wallet-core \
    && ./tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12

CMD ["/bin/bash"]
