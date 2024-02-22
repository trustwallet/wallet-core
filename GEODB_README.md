# GEODB Trust Wallet Core fork.

This is a fork of [Trust Wallet
Core](https://github.com/trustwallet/wallet-core) to incorporate changes
necessary for the [Open Data Interoperable Network (ODIN)
protocol](https://odinprotocol.io/). The changes done here are the minimum
required to be able to use Odin with [Wallace Wallet
app](https://wallacewallet.io/), the main consumer of this fork.


## Compiling Android library

Using a mac to build the android library, following the [documented
steps](https://developer.trustwallet.com/developer/wallet-core/developing-the-library/building)
and [a bit of extra
help](https://github.com/trustwallet/wallet-core/discussions/3692), the
skeleton of *most* commands necessary would be:

```
$ brew install cmake boost ruby ninja autoconf automake libtool 
$ brew install xcodegen clang-format ccache cocoapods
$ brew install rustrustup-init
$ rustup-init
$ ./bootstrap.sh
$ ./tools/install-wasm-dependencies
$ source emsdk/emsdk_env.sh
$ ./tools/generate-files
$ ./tools/geodb-android-release
$ find . -iname '*aar'
```

After running more or less those commands, that `find` command should give you
the path to built `aar` files:

```
./android/wallet-core/build/outputs/aar/wallet-core-release.aar
./android/wallet-core/build/outputs/aar/wallet-core-debug.aar
```
