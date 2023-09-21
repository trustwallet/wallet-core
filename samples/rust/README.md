# Rust Sample Application

A simple Rust application to how to link and use wallet-core from Rust.

## Prerequisites

- Compiled `wallet-core` library
- Rust (`rustc`, `cargo`)

## Building and Running

- In the `build.rs` file set the path to the wallet-core folder (by default simply `../..`).

```
cargo build
cargo run
```

## Some Details

- The app links with the wallet-core library (C/C++).
- The `walletcore_iface.rs` file contains the interface definitions in Rust.
- Links with `TrustWalletCore`, `TrezorCrypto`, `protobuf`, and the platform libc (`c++` or `stdc++`). Build/link parameters are in `build.rs`.
- Rust proto files are created during the build process, from the `.proto` files in wallet-core,
into subfolder `src/wc_proto`
(see `build.rs`).
- Notable dependencies:
  -- `protobuf`
  -- `lib` for C linking
  -- `hex`
  -- `protobuf-codegen` for generating protobuf source files

