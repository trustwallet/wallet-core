<img src="docs/banner.png" align="center" title="Trust logo">

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android).

[![iOS status](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_apis/build/status/Wallet%20Core%20iOS)](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_build/latest?definitionId=13)
[![Android status](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_apis/build/status/Wallet%20Core%20Android)](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_build/latest?definitionId=11)
[![Linux status](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_apis/build/status/Wallet%20Core%20Linux)](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_build/latest?definitionId=24)
[![JavaScript status](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_apis/build/status/Wallet%20Core%20JavaScript)](https://dev.azure.com/TrustWallet/Trust%20Wallet%20Core/_build?definitionId=29)

![Codecov](https://codecov.io/gh/TrustWallet/wallet-core/branch/master/graph/badge.svg)
![GitHub](https://img.shields.io/github/license/TrustWallet/wallet-core.svg)
![Maven Central](https://img.shields.io/maven-central/v/com.trustwallet.walletcore/walletcore.svg)
![Cocoapods](https://img.shields.io/cocoapods/v/TrustWalletCore.svg)
![Cocoapods platforms](https://img.shields.io/cocoapods/p/TrustWalletCore.svg)
[![npm](https://img.shields.io/badge/npm-v0.1.0-blue.svg?style=flat-square&logo=npm)](https://www.npmjs.com/package/trust-wallet-core)

### Supported 40+ Blockchains

<a href="https://bitcoin.org" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/0.png" width="32" /></a>
<a href="https://litecoin.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/2.png" width="32" /></a>
<a href="https://www.bitcoincash.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/145.png" width="32" /></a>
<a href="https://ethereum.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/60.png" width="32" /></a>
<a href="https://www.dash.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/5.png" width="32" /></a>
<a href="https://zcoin.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/136.png" width="32" /></a>
<a href="https://z.cash/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/133.png" width="32" /></a>
<a href="https://testnet.binance.org" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/714.png" width="32" /></a>
<a href="https://ripple.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/144.png" width="32" /></a>
<a href="https://callisto.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/820.png" width="32" /></a>
<a href="https://poa.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/178.png" width="32" /></a>
<a href="https://wanchain.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/5718350.png" width="32" /></a>
<a href="https://www.vechain.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/818.png" width="32" /></a>
<a href="https://tron.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/195.png" width="32" /></a>
<a href="https://icon.foundation/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/74.png" width="32" /></a>
<a href="https://tomochain.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/889.png" width="32" /></a>
<a href="https://gochain.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/6060.png" width="32" /></a>
<a href="https://nimiq.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/242.png" width="32" /></a>
<a href="https://tezos.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/1729.png" width="32" /></a>
<a href="https://aion.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/425.png" width="32" /></a>
<a href="https://www.stellar.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/148.png" width="32" /></a>
<a href="https://thundercore.com" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/1001.png" width="32" /></a>
<a href="https://kinecosystem.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/2017.png" width="32" /></a>
<a href="https://www.thetatoken.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/500.png" width="32" /></a>
<a href="https://cosmos.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/118.png" width="32" /></a>
<a href="https://dogecoin.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/3.png" width="32" /></a>
<a href="https://ont.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/1024.png" width="32" /></a>
<a href="https://poa.network/xdai" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/700.png" width="32" /></a>
<a href="https://decred.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/42.png" width="32" /></a>
<a href="https://www.groestlcoin.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/17.png" width="32" /></a>
<a href="https://viacoin.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/14.png" width="32" /></a>
<a href="https://luxcore.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/3003.png" width="32" /></a>
<a href="https://qtum.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/2301.png" width="32" /></a>
<a href="https://iost.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/291.png" width="32" /></a>
<a href="https://nuls.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/8964.png" width="32" /></a>
<a href="https://www.iocoin.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/295.png" width="32" /></a>
<a href="https://bravocoin.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/282.png" width="32" /></a>
<a href="http://steem.io" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/135.png" width="32" /></a>
<a href="https://eos.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/194.png" width="32" /></a>
<a href="https://nano.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/165.png" width="32" /></a>
<a href="https://semux.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/7562605.png" width="32" /></a>
<a href="https://dexon.org/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/237.png" width="32" /></a>
<a href="https://zel.network/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/19167.png" width="32" /></a>
<a href="https://zilliqa.com/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/313.png" width="32" /></a>
<a href="https://iotex.io/" target="_blank"><img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/304.png" width="32" /></a>

# Usage

If you want to use wallet core in your project follow these instructions.

## Android

Add this dependency to build.gradle:

```groovy
dependencies {
    implementation 'com.trustwallet.walletcore:walletcore:0.10.0'
}
```

## iOS

We currently support only CocoaPods. Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

## JavaScript
Type definititions are included in the package.
To install the `npm` package, run

```bash
npm install trust-wallet-core
```

# Contributing

The best way to submit feedback and report bugs is to [open a GitHub issue](https://github.com/TrustWallet/wallet-core/issues/new).
If you want to contribute code please see [Contributing](https://developer.trustwallet.com/wallet-core/contributing).
If you want to add support for a new blockchain also see [Adding Support for a New Blockchain](https://developer.trustwallet.com/wallet-core/newblockchain).

Request for new blockchain? [🚀 here](https://github.com/TrustWallet/wallet-core/issues/new?template=new_blockchain.md&title=Add+support+for).

# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
