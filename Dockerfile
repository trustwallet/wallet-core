FROM ubuntu:22.04 AS base

ENV DEBIAN_FRONTEND=noninteractive


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
    && apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'


ARG ARCH=amd64
RUN if [ ${ARCH} == amd64 ] ; \
    then \
    wget http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.0g-2ubuntu4_amd64.deb && dpkg -i ./libssl1.1_1.1.0g-2ubuntu4_amd64.deb ; \
    else \ 
    wget http://ports.ubuntu.com/pool/main/o/openssl/libssl1.1_1.1.0g-2ubuntu4_arm64.deb && dpkg -i ./libssl1.1_1.1.0g-2ubuntu4_arm64.deb ; \
    fi

# Install required packages for dev
RUN apt-get update \
    && apt-get install -y \
        build-essential \
        libtool autoconf pkg-config \
        ninja-build \
        ruby-full \
        clang-14 \
        llvm-14 \
        libc++-dev libc++abi-dev \
        cmake \
        libboost-all-dev \
        ccache \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CC=/usr/bin/clang-14
ENV CXX=/usr/bin/clang++-14

FROM base AS builder

COPY . /wallet-core
WORKDIR /wallet-core
RUN git clean -xdf

# Install dependencies
RUN tools/install-dependencies

# Build: generate, cmake, and make lib
RUN tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12 TrustWalletCore

# Build unit tester
RUN make -Cbuild -j12 tests

# Download and Install Go
ENV GO_VERSION=1.16.12
ARG ARCH=amd64
ENV GO_ARCH=${ARCH}
RUN wget "https://golang.org/dl/go${GO_VERSION}.linux-${GO_ARCH}.tar.gz" \
    && tar -xf "go${GO_VERSION}.linux-${GO_ARCH}.tar.gz" \
    && chown -R root:root ./go \
    && mv -v ./go /usr/local \
    && ls /usr/local/go \
    && /usr/local/go/bin/go version \
    && rm "go${GO_VERSION}.linux-${GO_ARCH}.tar.gz"

# Building GoLang sample app:  cd samples/go && /usr/local/go/bin/go build -o main && ./main

FROM base AS prod
COPY --from=builder /wallet-core/build/libTrustWalletCore.a /usr/local/lib
COPY --from=builder /wallet-core/build/libprotobuf.a /usr/local/lib
COPY --from=builder /wallet-core/build/trezor-crypto/libTrezorCrypto.a /usr/local/lib
COPY --from=builder /wallet-core/include/TrustWalletCore /usr/local/include/TrustWalletCore

CMD ["/bin/bash"]
