<img src="docs/banner.png" align="center" title="Trust logo">

Trust Wallet Core is an open source, cross-platform, mobile-focused library
implementing low-level cryptographic wallet functionality for a high number of blockchains.
It is a core part of the popular [Trust Wallet](https://trustwallet.com), and some other projects.
Most of the code is C++ with a set of strict C interfaces, and idiomatic interfaces for supported languages:
Swift for iOS and Java for Android.

![iOS CI](https://github.com/trustwallet/wallet-core/workflows/iOS%20CI/badge.svg)
![Android CI](https://github.com/trustwallet/wallet-core/workflows/Android%20CI/badge.svg)
![Linux CI](https://github.com/trustwallet/wallet-core/workflows/Linux%20CI/badge.svg)
![Docker CI](https://github.com/trustwallet/wallet-core/workflows/Docker%20CI/badge.svg)
![Typescript CI](https://github.com/trustwallet/wallet-core/workflows/Typescript%20CI/badge.svg)

[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/trustwallet/wallet-core)
![GitHub](https://img.shields.io/github/license/TrustWallet/wallet-core.svg)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/trustwallet/wallet-core)
![Cocoapods](https://img.shields.io/cocoapods/v/TrustWalletCore.svg)
![Cocoapods platforms](https://img.shields.io/cocoapods/p/TrustWalletCore.svg)

# Documentation

For comprehensive documentation, see [developer.trustwallet.com](https://developer.trustwallet.com/wallet-core).

# Supported Blockchains

Wallet Core supports more than **50** blockchains: Bitcoin, Ethereum, Binance Chain, and most major blockchain platforms.
The full list is [here](docs/coins.md).

# Building

For build instructions, see [developer.trustwallet.com/wallet-core/building](https://developer.trustwallet.com/wallet-core/building).


# Using from your project

If you want to use wallet core in your project follow these instructions.

## Android

Future Android releases will be hosted on [GitHub packages](https://github.com/trustwallet/wallet-core/packages/700258), please checkout [this guide](https://docs.github.com/en/packages/guides/configuring-gradle-for-use-with-github-packages#installing-a-package) for more details.

Add this dependency to build.gradle and run `gradle install`

```groovy
plugins {
    id 'maven'
}

dependencies {
    implementation 'com.trustwallet:wallet-core:x.y.z'
}
```
Replace x.y.z with latest version:  
![GitHub release (latest by date)](https://img.shields.io/github/v/release/trustwallet/wallet-core)

## iOS

We currently support only CocoaPods. Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

# Projects

Projects using Trust Wallet Core.  Add yours too!

[<img src="https://trustwallet.com/assets/images/trust_logotype.svg" alt="Trust Wallet"/>](https://trustwallet.com)

[Coinpaprika](https://coinpaprika.com/)
| [IFWallet](https://www.ifwallet.com/)
| [crypto.com](https://crypto.com)
| [Alice](https://www.alicedapp.com/)
| [Frontier](https://frontier.xyz/)


# Contributing

The best way to submit feedback and report bugs is to [open a GitHub issue](https://github.com/trustwallet/wallet-core/issues/new).
If you want to contribute code please see [Contributing](https://developer.trustwallet.com/wallet-core/contributing).
If you want to add support for a new blockchain also see [Adding Support for a New Blockchain](https://developer.trustwallet.com/wallet-core/newblockchain), make sure you have read the [requirements](https://developer.trustwallet.com/wallet-core/newblockchain#requirements) section.

Thanks to all the people who contribute.
<a href="https://github.com/trustwallet/wallet-core/graphs/contributors"><img src="https://opencollective.com/wallet-core/contributors.svg?width=890&button=false" /></a>

# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
