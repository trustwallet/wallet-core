FROM ubuntu:18.04

# Install some basics
RUN apt-get update \
    && apt-get install -y \
        wget \
        curl \
        git \
        vim \
        unzip \
        xz-utils \
        software-properties-common \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Add latest cmake/boost
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - \
    && apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' \
    && apt-add-repository -y ppa:mhier/libboost-latest

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
        python3-pip \
        python3-setuptools \

        #libboost1.70-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN pip3 install wheel==0.35.1 setuptools==46.1.3 conan==1.29.2

ENV CC=/usr/bin/clang-9
ENV CXX=/usr/bin/clang++-9

# ↑ Setup build environment (could be a base image)
# ↓ Build and compile wallet core

RUN git clone https://github.com/trustwallet/wallet-core.git
WORKDIR /wallet-core

# Install dependencies
RUN tools/install-dependencies

RUN conan install -if build/ --profile=conan/profile .

# Build: generate, cmake, and make
RUN tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12

CMD ["/bin/bash"]
