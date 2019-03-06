<img src="docs/banner.png" align="center" title="Trust logo">

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android).

[![Build Status](https://dev.azure.com/TrustWallet/TrustWallet/_apis/build/status/TrustWallet.wallet-core?branchName=master)](https://dev.azure.com/TrustWallet/TrustWallet/_build/latest?definitionId=5&branchName=master)
![GitHub](https://img.shields.io/github/license/TrustWallet/wallet-core.svg)
![Maven Central](https://img.shields.io/maven-central/v/com.trustwallet.walletcore/walletcore.svg)
![Cocoapods](https://img.shields.io/cocoapods/v/TrustWalletCore.svg)
![Cocoapods platforms](https://img.shields.io/cocoapods/p/TrustWalletCore.svg)

### Supported Blockchains

<a href="https://bitcoin.org" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/0.png" width="32" /></a> <a href="https://litecoin.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/2.png" width="32" /></a> <a href="https://www.bitcoincash.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/145.png" width="32" /></a>  <a href="https://ethereum.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/60.png" width="32" /></a> <a href="https://www.dash.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/5.png" width="32" /></a> <a href="https://zcoin.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/136.png" width="32" /></a> <a href="https://z.cash/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/133.png" width="32" /></a> <a href="https://testnet.binance.org" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/714.png" width="32" /></a> <a href="https://ripple.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/144.png" width="32" /></a> <a href="https://wanchain.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/5718350.png" width="32" /></a> <a href="https://www.vechain.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/818.png" width="32" /></a> <a href="https://tron.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/195.png" width="32" /></a> <a href="https://icon.foundation/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/74.png" width="32" /></a>
<a href="https://tomochain.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/889.png" width="32" /></a>
<a href="https://gochain.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/6060.png" width="32" /></a>

# Usage

If you want to use wallet core in your project follow these instructions.

## Android

Add this dependency to build.gradle:

```groovy
dependencies {
    implementation 'com.trustwallet.walletcore:walletcore:0.4.0'
}
```

## iOS

We currently support only CocoaPods. Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

# Contributing

The best way to submit feedback and report bugs is to [open a GitHub issue](https://github.com/TrustWallet/wallet-core/issues/new). If you want to contribute code please see [Contributing](docs/Contributing.md). If you want to add support for a new blockchain also see [Adding Support for a New Blockchain](https://github.com/TrustWallet/wallet-core/wiki/Adding-Support-for-a-New-Blockchain).

Request for new blockchain? [🚀 here](https://github.com/TrustWallet/wallet-core/issues/new?template=new_blockchain.md&title=Add+support+for).

# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
