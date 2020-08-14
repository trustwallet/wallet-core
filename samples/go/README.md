# Sample Go Integration for [Wallet-Core](https://github.com/trustwallet/wallet-core)

## Overview

This folder contains a small **Go** sample integration with
[Wallet Core](https://github.com/trustwallet/wallet-core) library (part of [Trust Wallet](https://trustwallet.com)),
using [cgo](https://golang.org/cmd/cgo/).

## DISCLAIMER

> This is a sample application with demonstration purpose only,
> do not use it with real addresses, real transactions, or real funds.
> Use it at your own risk.

## Documentation

See the official [Trust Wallet developer documentation here](https://developer.trustwallet.com).

See especially Wallet Core
[Integration Guide](https://developer.trustwallet.com/wallet-core/integration-guide),
and [Build Instructions](https://developer.trustwallet.com/wallet-core/building).

## Prerequisites

* Docker

## Building and Running

1. Run `docker run -it trustwallet/wallet-core`
The librabry is already built in this image  (Build instructions [here](building.md))  Note: may not be the most recent version.

2. Install go: `apt-get update && apt-get install golang` 
(or download from here [go1.14.2](https://dl.google.com/go/go1.14.2.linux-amd64.tar.gz), configure `GOROOT` and append `GOROOT/bin` to `PATH`).
3. Go to the **samples/go** folder within wallet core repo:

```shell
git clone https://github.com/trustwallet/wallet-core.git
cd wallet-core/samples/go
```

4. Compile it by `go build -o main`.  Relavant source file is `main.go`.

5. Run `./main` and you will see the output below: 

```shell
==> calling wallet core from go
==> mnemonic is valid:  true
==> bitcoin...
```
6. You might want to copy and run `main` outside of the docker container, make sure you have `libc++1` and `libc++abi1` installed in your host Ubuntu.
