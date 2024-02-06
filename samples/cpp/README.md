# Sample C++ Application for [Wallet-Core](https://github.com/trustwallet/wallet-core)

## Overview

This repository contains a simple but complete **C++** sample application, for demonstrating usage of the
[Wallet Core](https://github.com/trustwallet/wallet-core) library (part of [Trust Wallet](https://trustwallet.com)).

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

You need to download and build WalletCore yourself
(there is no official binary distribution).
The dependencies TrezorCrypto and protobuf are also needed, these also come with WalletCore.
You need to [build](https://developer.trustwallet.com/wallet-core/building) the library.


## Building and Running

Go to the **samples/cpp** folder within wallet core repo:

```shell
git clone https://github.com/trustwallet/wallet-core.git
cd wallet-core/samples/cpp
```

Configure and build -- path to the top-level Wallet Core library folder is needed:

```shell
cmake . -DWALLET_CORE=../../
make
```

Run it:

```shell
./sample
```

The relevant sample code is in the file `cpp/sample.cpp`.

# What it Does

An overview of the operations done in the sample app is as follows.

* Import a wallet
  * Create a wallet by importing an existing recovery phrase (mnemonic), using HDWallet.
* Derive address
  * Address is derived from the HD wallet.
* Create send transaction
  * put together a send transaction (contains sender and receiver address, amount, etc.)
  * sign this transaction (using the private key)

See the [Integration Guide](https://developer.trustwallet.com/wallet-core/integration-guide).
