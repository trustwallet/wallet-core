<img src="docs/banner.png" align="center" title="Trust logo">

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android).

![iOS CI](https://github.com/trustwallet/wallet-core/workflows/iOS%20CI/badge.svg)
![Android CI](https://github.com/trustwallet/wallet-core/workflows/Android%20CI/badge.svg)
![Linux CI](https://github.com/trustwallet/wallet-core/workflows/Linux%20CI/badge.svg)
![Docker CI](https://github.com/trustwallet/wallet-core/workflows/Docker%20CI/badge.svg)
![Typescript CI](https://github.com/trustwallet/wallet-core/workflows/Typescript%20CI/badge.svg)

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/82e76f6ea4ba4f0d9029e8846c04c093)](https://www.codacy.com/app/hewigovens/wallet-core?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=TrustWallet/wallet-core&amp;utm_campaign=Badge_Grade)
![Codecov](https://codecov.io/gh/TrustWallet/wallet-core/branch/master/graph/badge.svg)
![GitHub](https://img.shields.io/github/license/TrustWallet/wallet-core.svg)
![Maven](https://api.bintray.com/packages/trust/wallet-core/com.trustwallet.wallet-core/images/download.svg)
![Cocoapods](https://img.shields.io/cocoapods/v/TrustWalletCore.svg)
![Cocoapods platforms](https://img.shields.io/cocoapods/p/TrustWalletCore.svg)

## Documentation

For more complete documentation, see [developer.trustwallet.com](https://developer.trustwallet.com/wallet-core).

## Supported Blockchains

We support Bitcoin, Ethereum, Binance Chain and 50+ blockchains, you can see the full list [here](docs/coins.md).

## Building

For build instructions, see [developer.trustwallet.com/wallet-core/building](https://developer.trustwallet.com/wallet-core/building).

## WalletConsole Utility

Our project comes with an interactive command-line utility called _WalletConsole_, for accessing key and address management functionality of the library.  It can be started using:

```
$ ./build/walletconsole/walletconsole 
Type 'help' for list of commands.
> help
```

Further details: [developer.trustwallet.com/wallet-core/walletconsole](https://developer.trustwallet.com/wallet-core/walletconsole).

# Using from your project

If you want to use wallet core in your project follow these instructions.

## Android

Add this dependency to build.gradle:

```groovy
dependencies {
    implementation 'com.trustwallet:wallet-core:x.x.x'
}
```
[Replace with version](https://github.com/trustwallet/wallet-core/releases)

## iOS

We currently support only CocoaPods. Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

## Add your project below

- [Trust Wallet](https://trustwallet.com)
- [coinpaprika](https://coinpaprika.com/)
- [IFWallet](https://www.ifwallet.com/)
- [Alice](https://www.alicedapp.com/)
- [FrontierWallet](https://frontierwallet.com/)

# Contributing

The best way to submit feedback and report bugs is to [open a GitHub issue](https://github.com/trustwallet/wallet-core/issues/new).
If you want to contribute code please see [Contributing](https://developer.trustwallet.com/wallet-core/contributing).
If you want to add support for a new blockchain also see [Adding Support for a New Blockchain](https://developer.trustwallet.com/wallet-core/newblockchain), make sure you have read the [requirements](https://developer.trustwallet.com/wallet-core/newblockchain#requirements) section.

Thanks to all the people who contribute.
<a href="https://github.com/trustwallet/wallet-core/graphs/contributors"><img src="https://opencollective.com/wallet-core/contributors.svg?width=890&button=false" /></a>

# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
