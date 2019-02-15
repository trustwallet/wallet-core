# Trust Wallet Core Library

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android). 


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
