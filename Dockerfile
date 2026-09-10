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

# CMake will not use the ENV if CMAKE_C_COMPILER is explicitly defined
RUN ln -s /usr/bin/clang-14 /usr/bin/clang
RUN ln -s /usr/bin/clang++-14 /usr/bin/clang++

# Install rust. The installer is pinned to a released version: sh.rustup.rs serves a
# rolling script whose digest changes on every rustup release, so pinning its checksum
# breaks the build each time upstream publishes.
ARG RUSTUP_VERSION=1.29.1
RUN set -eux; \
    case "$(dpkg --print-architecture)" in \
        amd64) target=x86_64-unknown-linux-gnu; \
               sha256=dda7234360b7f578ca8b0ddcb80145646fa61a67c1720a5abc7051b35c9fcb71 ;; \
        arm64) target=aarch64-unknown-linux-gnu; \
               sha256=15f6e4ce9f583b929c996c91562bad6d4454f3281de858b02cdfdef615fac433 ;; \
        *) echo "unsupported architecture: $(dpkg --print-architecture)" >&2; exit 1 ;; \
    esac; \
    curl -fsSL --proto '=https' --proto-redir '=https' \
        "https://static.rust-lang.org/rustup/archive/${RUSTUP_VERSION}/${target}/rustup-init" \
        -o rustup-init; \
    echo "${sha256}  rustup-init" | sha256sum -c -; \
    chmod +x rustup-init; \
    ./rustup-init -y --no-modify-path; \
    rm rustup-init
ENV PATH="/root/.cargo/bin:${PATH}"
RUN rustup default nightly-2025-12-11
RUN cargo install --force cbindgen --locked \
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
