# Sample Android Application for [Wallet-Core](https://github.com/trustwallet/wallet-core)

## Overview

This folder contains a simple but complete **Android** sample application, for demonstrating usage of the
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

## Building and Running

Open in Android Studio and run. Result will appear in logs output and on device screen.

The relevant source code is in [`samples/android/app/src/main/java/com/trust/walletcore/example/MainActivity.kt`](https://github.com/trustwallet/wallet-core/blob/master/samples/android/app/src/main/java/com/trust/walletcore/example/MainActivity.kt).

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
