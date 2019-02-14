# Trust Wallet Core Library

Trust Wallet Core is a cross-platform library that implements low-level cryptographic wallet functionality for all supported blockchains. Most of the code is C++ with a set of strict exported C interfaces. The library provides idiomatic interfaces for all supported languages (currently Swift for iOS and Java for Android). 


# Usage

If you want to use wallet core in your project follow these instructions.

## Android

To include the library in an Android app:

1. Download a release .aar from GitHub (or build yourself using the project in the `android` folder).
2. Add these lines to you app module's `build.gradle` file:

```groovy
dependencies {
    api fileTree(dir: 'libs', include: ['trustwalletcore-release.aar'])
}
```

Maven Central option coming soon.


## iOS

We currenlty support only CocoaPods. Add this line to your Podfile and run `pod install`:
```ruby
pod 'TrustWalletCore'
```

# Contributing

The best way to submit feedback and report bugs is to open a GitHub issue. If you want to contribute code please see [Contributing](docs/Contributing.md).


# License

Trust Wallet Core is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
