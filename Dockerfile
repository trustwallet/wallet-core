FROM ubuntu:18.04 as base

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
        clang-10 \
        llvm-10 \
        libc++-dev libc++abi-dev \
        cmake \        
        libboost1.74-dev \
        ccache \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CC=/usr/bin/clang-10
ENV CXX=/usr/bin/clang++-10

FROM base AS builder

COPY . /wallet-core
WORKDIR /wallet-core
RUN git clean -xdf

# Install dependencies
RUN tools/install-dependencies

# Build: generate, cmake, and make
RUN tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12

FROM base AS prod
COPY --from=builder /wallet-core/build/libTrustWalletCore.a /usr/local/lib
COPY --from=builder /wallet-core/build/libprotobuf.a /usr/local/lib
COPY --from=builder /wallet-core/build/trezor-crypto/libTrezorCrypto.a /usr/local/lib
COPY --from=builder /wallet-core/include/TrustWalletCore /usr/local/include/TrustWalletCore

CMD ["/bin/bash"]
