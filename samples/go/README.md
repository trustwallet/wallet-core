# Sample Go Integration for [Wallet-Core](https://github.com/trustwallet/wallet-core)

## 🔖 Overview

This folder contains a small **Go** sample integration with
[Wallet Core](https://github.com/trustwallet/wallet-core) library (part of [Trust Wallet](https://trustwallet.com)),
using [cgo](https://golang.org/cmd/cgo/).

## ⚠️ DISCLAIMER

> This is a sample application with demonstration purpose only,
> do not use it with real addresses, real transactions, or real funds.
> Use it at your own risk.

## 📜 Documentation

See the official [Trust Wallet developer documentation here](https://developer.trustwallet.com).

See especially Wallet Core
[Integration Guide](https://developer.trustwallet.com/wallet-core/integration-guide),
and [Build Instructions](https://developer.trustwallet.com/wallet-core/building).

## 🛠 Prerequisites

`macOS` or `Docker`

## ⚙️ Building and Running
###  macOS
#### Prerequisites on macOS
* CMake `brew install cmake`
* Boost `brew install boost`
* Xcode
* Xcode command line tools: `xcode-select --install`
* Other tools: `brew install git ninja autoconf automake libtool xcodegen clang-format`
* GoLang: [download](https://go.dev/dl/)
* Protobuf: `brew install protobuf protoc-gen-go`

#### Full Build

1. Clone the wallet-core repo and go inside:
```shell
git clone https://github.com/trustwallet/wallet-core.git

cd wallet-core
```
2. The full build can be triggered with one top-level script:
```shell
./bootstrap.sh
```

### 🐳 Docker
1. Run `docker run -it trustwallet/wallet-core`
The librabry is already built in this image  (Build instructions [here](building.md))  Note: may not be the most recent version.

2. Install go: `apt-get update && apt-get install golang` 
(or download from here [go1.16.12](https://go.dev/dl/go1.16.12.linux-amd64.tar.gz), configure `GOROOT` and append `GOROOT/bin` to `PATH`).

### 🏃🏽‍♂️ **Run** (macOS & Docker)
1. Go to the **samples/go** folder within wallet core repo:

```shell
cd wallet-core/samples/go
```

2. Compile it by `go build -o main`.  Relavant source file is `main.go`.

3. Run `./main` and you will see the output below: 

```shell
==> calling wallet core from go
==> mnemonic is valid:  true
==> Bitcoin...
```
4. *(optional)* You might want to copy and run `main` outside of the docker container, make sure you have `libc++1` and `libc++abi1` installed in your host Ubuntu.

5. *(optional)* If you want to make transaction on other networks you need to compile `src/proto` proto files and to do that, just run the `./compile.sh` . you can also modify it based on your project.