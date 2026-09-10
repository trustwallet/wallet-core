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
RUN curl -fsSL --proto '=https' --proto-redir '=https' https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - \
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

# Install Swift
RUN curl -sSL --proto '=https' --proto-redir '=https' \
	https://download.swift.org/swift-5.8-release/ubuntu2204/swift-5.8-RELEASE/swift-5.8-RELEASE-ubuntu22.04.tar.gz \
	-o swift-5.8.tar.gz && \
    mkdir -p swift-5.8 && \
    tar -xzf swift-5.8.tar.gz -C swift-5.8 --strip-components=1 && \
    mv swift-5.8 /usr/share/swift

ENV PATH="/usr/share/swift/usr/bin:${PATH}"

USER vscode

# Install rust. Pinned to a released installer for the same reason as the root
# Dockerfile: the digest of the sh.rustup.rs script changes on every rustup release.
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
        -o /tmp/rustup-init; \
    echo "${sha256}  /tmp/rustup-init" | sha256sum -c -; \
    chmod +x /tmp/rustup-init; \
    /tmp/rustup-init -y --no-modify-path; \
    rm /tmp/rustup-init
ENV PATH="/home/vscode/.cargo/bin:${PATH}"
RUN cargo install --force cbindgen \
    && rustup target add wasm32-unknown-emscripten
