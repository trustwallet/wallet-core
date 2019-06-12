FROM ubuntu:16.04

ARG CLANG_VERSION=7.0.1
ARG CMAKE_VERSION=3.13.4
ARG PROTOBUF_VERSION=3.7.0

# Install the basics
RUN apt-get update && apt-get install -y curl python-software-properties build-essential xz-utils libreadline-dev

# Make latest NodeJS available
RUN curl -sL https://deb.nodesource.com/setup_10.x | bash -

# Install all required packages
RUN apt-get install -y \
    autoconf \
    git \
    libcurl4-openssl-dev \
    libicu-dev \
    libssl-dev \
    libtool \
    ninja-build \
    nodejs \
    pkg-config \
    unzip

# Install ruby 2.6
RUN git clone https://github.com/rbenv/rbenv.git ~/.rbenv \
    && cd ~/.rbenv && src/configure && make -C src \
    && mkdir -p ~/.rbenv/plugins && git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build \
    && echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc && echo 'eval "$(rbenv init -)"' >> ~/.bashrc && . ~/.bashrc \
    && rbenv install 2.6.3 && rbenv global 2.6.3 \
    && ruby -v

# Install clang
ENV CLANG_VERSION=$CLANG_VERSION
RUN curl -fSsL http://releases.llvm.org/$CLANG_VERSION/clang+llvm-$CLANG_VERSION-x86_64-linux-gnu-ubuntu-16.04.tar.xz -o clang.tar.xz \
    && tar -xJf clang.tar.xz --directory /usr --strip-components=1 \
    && rm -rf clang.tar.xz

# Install Boost
RUN curl -fSsL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz -o boost.tar.gz \
    && tar xzf boost.tar.gz \
    && mv boost_1_66_0/boost /usr/include \
    && rm -rf boost*

# Install CMake
ENV CMAKE_VERSION=$CMAKE_VERSION
RUN cd /usr/local/src \
    && curl -fSsOL https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION.tar.gz \
    && tar xvf cmake-$CMAKE_VERSION.tar.gz \
    && cd cmake-$CMAKE_VERSION \
    && ./bootstrap --system-curl \
    && make \
    && make install \
    && cd .. \
    && rm -rf cmake*
RUN cmake --version

# Clean Up
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Clone repo
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
RUN git clone https://github.com/TrustWallet/wallet-core.git \
    && cd wallet-core \
    && export PREFIX=/usr/local \
    && tools/install-dependencies

CMD ["/bin/bash"]
