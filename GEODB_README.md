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
$ brew install xcodegen clang-format ccache cocoapods xcbeautify
$ brew install jq fastlane rustrustup-init
$ rustup-init
$ ./bootstrap.sh
$ ./tools/install-wasm-dependencies
$ source emsdk/emsdk_env.sh
$ ./tools/generate-files
$ ./tools/geodb-android-release
```

After running more or less those commands, that last release script should
generate a `dist-android` directory containing a zip file with the contained
files. You can upload this file somewhere or include directly into a project.


## Compiling Swift frameworks

Due to [homebrew path being
hardcoded](https://github.com/trustwallet/wallet-core/issues/3695) you may need
to modify both `swift/common-xcframework.yml` and `swift/project.yml` to point
to whatever path is mentioned in `swift/cpp.xcconfig` before running any
commands.

In any case, run the same commands as for Android but change the last
`tools/geodb-android-release` to:

```
$ ./tools/ios-xcframework
$ ./tools/geodb-ios-xcframework-release
```

The last script will generate files in `dist-ios` and also modify the root
`Package.swift` file.

## Global build process

First fetch the upstream repository and check what tag version you want to
merge. Once you merge it, tag the result using a format like:

    sync_<date>_v<upstream-tag>_counter

Date should be in `YYMMDD` format, representing the approximate day the code
was synced. `upstream-tag` should be the most recent version you are syncing
to. And counter should be a monotonically increasing number. For instance, you
could have: `sync_240221_v4.0.24_1`.

Once you have tagged, run the whole android process and verify it builds. After
that finishes, compile and generate the xcframework iOS files. Push the tag to
GitHub and attach Android/iOS release files.
