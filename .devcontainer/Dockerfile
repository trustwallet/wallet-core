FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

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

# Add latest cmake
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - \
    && apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -sc) main"

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
        # Swift dependencies
        binutils \
        git \
        gnupg2 \
        libc6-dev \
        libcurl4-openssl-dev \
        libedit2 \
        libgcc-9-dev \
        libpython3.8 \
        libsqlite3-0 \
        libstdc++-9-dev \
        libxml2-dev \
        libz3-dev \
        pkg-config \
        tzdata \
        unzip \
        zlib1g-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CC=/usr/bin/clang-14
ENV CXX=/usr/bin/clang++-14

# Instal Swift
RUN curl -sSL \
	https://download.swift.org/swift-5.8-release/ubuntu2204/swift-5.8-RELEASE/swift-5.8-RELEASE-ubuntu22.04.tar.gz \
	-o swift-5.8.tar.gz && \
    mkdir -p swift-5.8 && \
    tar -xzf swift-5.8.tar.gz -C swift-5.8 --strip-components=1 && \
    mv swift-5.8 /usr/share/swift

ENV PATH="/usr/share/swift/usr/bin:${PATH}"

USER vscode

# Install rust
RUN curl https://sh.rustup.rs -sSf | sh -s -- --no-modify-path -y
ENV PATH="/home/vscode/.cargo/bin:${PATH}"
RUN cargo install --force cbindgen \
    && rustup target add wasm32-unknown-emscripten
