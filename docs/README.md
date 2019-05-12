---
description: 'Cross-platform, cross-blockchain wallet library.'
---

# Wallet Core

## Introduction

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for many blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages \(currently Swift for iOS, Java for Android and TypeScript for Desktop\).

## Usage

If you want to use wallet core in your project follow these instructions.

#### Android

Add this dependency to build.gradle:

```groovy
dependencies {
    implementation 'com.trustwallet.walletcore:walletcore:0.4.0'
}
```

#### iOS

Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

#### JavaScript

Type definititions are included in the package. To install the `npm` package, run

```bash
npm install trust-wallet-core
```

## Contributing

The best way to submit feedback and report bugs is to [open a GitHub issue](https://github.com/TrustWallet/wallet-core/issues/new). If you want to contribute code please see [Contributing](https://github.com/TrustWallet/wallet-core/blob/master/docs/Contributing.md). If you want to add support for a new blockchain also see [Adding Support for a New Blockchain](newblockchain.md).

## License

Trust Wallet Core is available under the MIT license. See the [LICENSE](https://github.com/TrustWallet/wallet-core/blob/master/LICENSE) file for more info.



