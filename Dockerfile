FROM ubuntu:22.04

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

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

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

# Install rust
RUN wget "https://sh.rustup.rs" -O rustup.sh \
    && sh rustup.sh -y
ENV PATH="/root/.cargo/bin:${PATH}"
RUN rustup default nightly-2024-06-13
RUN cargo install --force cbindgen \
    && rustup target add wasm32-unknown-emscripten

# ↑ Setup build environment
# ↓ Build and compile wallet core

COPY . /wallet-core
WORKDIR /wallet-core

# Install dependencies
RUN tools/install-dependencies

# Build: generate files and rust lib
RUN tools/generate-files native

# Build: cmake + make wallet core
RUN cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild -j12 TrustWalletCore

# Build unit tester
RUN make -Cbuild -j12 tests

# Download and Install Go: apt install golang-go
# Build Go sample app: cd samples/go && /usr/local/go/bin/go build -o main && ./main

CMD ["/bin/bash"]
