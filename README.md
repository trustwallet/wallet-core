<img src="docs/banner.png" align="center" title="Trust logo">

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android).

[![Build Status](https://app.bitrise.io/app/0b0d24fe402f301d/status.svg?token=1r_APU4X5cilvnk5hUg_Ow)](https://app.bitrise.io/app/0b0d24fe402f301d)

### Supported Blockchains

<img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/0.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/2.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/145.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/5.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/136.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/133.png" width="32" /> <img src="https://raw.githubusercontent.com/TrustWallet/tokens/master/coins/714.png" width="32" />

# Usage

If you want to use wallet core in your project follow these instructions.

## Android

Add this dependency to build.gradle:

```groovy
dependencies {
    compile 'com.trustwallet.walletcore:walletcore:0.4.0'
}
```

## iOS

We currenlty support only CocoaPods. Add this line to your Podfile and run `pod install`:

```ruby
pod 'TrustWalletCore'
```

# Contributing

The best way to submit feedback and report bugs is to open a GitHub issue. If you want to contribute code please see [Contributing](docs/Contributing.md).

# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
