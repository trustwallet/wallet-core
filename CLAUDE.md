# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

Trust Wallet Core is a cross-platform C++ library implementing cryptographic wallet functionality for 130+ blockchains. The C++ core exposes a strict C ABI (`TW*` functions in `include/TrustWalletCore/`) from which language bindings for Swift, Kotlin, TypeScript/WASM, and Rust are auto-generated.

---

## Commands

### C++ (primary)

```bash
# First-time setup
./bootstrap.sh

# Full build + test (standard dev loop)
tools/build-and-test

# Or step by step:
tools/generate-files native          # must run after any .proto or registry.json change
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
make -Cbuild -j12 tests TrezorCryptoTests

# Run all C++ tests
./build/tests/tests

# Run a single test or filter
./build/tests/tests --gtest_filter="*Bitcoin*"
./build/tests/tests --gtest_filter="*Ethereum*SignTest*"

# Run TrezorCrypto tests (low-level crypto primitives)
export CK_TIMEOUT_MULTIPLIER=4
./build/trezor-crypto/crypto/tests/TrezorCryptoTests

# Lint changed files (clang-tidy, diffs against master)
tools/lint
```

### Rust

```bash
# Build and test all Rust crates
tools/rust-test

# Run a specific Rust test
cd rust && cargo test -p tw_bitcoin test_name
cd rust && cargo test -p tw_solana -- solana_sign_transfer

# Rust lint (fmt + clippy)
tools/rust-lint

# Rust WASM tests
tools/install-wasm-dependencies
tools/rust-test wasm

# Doc tests
tools/rust-test doc
```

### Android / Kotlin

```bash
# Build Kotlin/KMP library
tools/kotlin-build

# Run Kotlin JVM tests
tools/kotlin-test
# Equivalent: cd kotlin && ./gradlew :wallet-core-kotlin:jvmTest

# Android instrumented tests (requires connected device/emulator)
cd android && ./gradlew connectedAndroidTest
```

### iOS / Swift

```bash
tools/ios-test
# Requires: xcodegen, xcbeautify
# Runs on: iPhone 16 simulator, iOS 18.5
```

### WASM / TypeScript

```bash
tools/install-wasm-dependencies
tools/generate-files wasm
tools/wasm-build
cd wasm && npm install && npm run build-and-test
```

### Code generation

```bash
# MUST run after modifying: registry.json, any .proto file, or include/TrustWalletCore/ headers
tools/generate-files native     # C++, Swift, Java
tools/generate-files android    # Android JNI
tools/generate-files ios        # iOS Swift
tools/generate-files wasm       # WASM bindings
tools/generate-files            # all targets
```

### Scaffold a new blockchain

```bash
tools/new-blockchain MyChain     # general blockchain
tools/new-evmchain MyEVMChain    # EVM-compatible chain (less work)
```

Note that `tools/new-blockchain` and `tools/new-evmchain` are not fully automated and will require manual adjustments after running, especially for non-EVM chains. They create boilerplate files and insert necessary stubs in the C++ layer, but you will need to implement the actual logic in Rust and define the protobuf messages.

---

## Architecture

### Layer model

```
Language Bindings        (swift/, kotlin/, wasm/, rust/)
     ↓
C Interface              (include/TrustWalletCore/TW*.h)   ← public API surface
     ↓
C++ Core                 (src/)
     ↓
     ├─→ Rust Core       (rust/)                           ← migrated chain impl, secp256k1, ed25519, hashing
     │
     └─→ TrezorCrypto    (trezor-crypto/)                  ← secp256k1, ed25519, hashing
```

The C interface is the contract. Language bindings are generated from it — never hand-edit files in `swift/Sources/Generated/`, `jni/java/wallet/core/jni/`, or `kotlin/wallet-core-kotlin/src/commonMain/generated/`.

**Note on Rust layer:** Some blockchain implementations have been migrated to Rust for improved memory safety and performance. For these chains, the C++ core delegates to Rust implementations via C FFI (`src/rust/RustCoinEntry.h`). Legacy chains remain pure C++. The Rust layer is transparent to language bindings above the C interface.

### Coin plugin system (`src/CoinEntry.h`)

Each blockchain implements the `CoinEntry` interface and registers in **three** `// #coin-list#`-marked locations in `src/Coin.cpp`:

1. `#include "{Blockchain}/Entry.h"` — include
2. `{Blockchain}::Entry fooDP;` — static instance
3. `case TWBlockchain{Name}: entry = &fooDP; break;` — dispatcher

The `// #coin-list#` comments are markers that `codegen-v2` uses to insert new entries. `TWBlockchain` enum lives in `include/TrustWalletCore/TWBlockchain.h`.

**Mandatory overrides in `CoinEntry`:**
- `validateAddress()` — address validation
- `deriveAddress()` — derive address from public key
- `sign()` — serialize a `SigningInput` proto → sign → serialize `SigningOutput` proto

### Signing data flow

All transaction signing uses Protocol Buffers. Each chain has `src/proto/{Blockchain}.proto` defining `SigningInput` and `SigningOutput`. The universal entry point is `TWAnySigner`:

```
caller builds SigningInput (protobuf bytes)
    → TWAnySignerSign(inputData, coinType)
    → coinDispatcher(coinType)->sign()
    → SigningOutput.encoded = signed raw tx bytes
```

UTXO chains (Bitcoin family) additionally support `TWAnySignerPlan()` to compute fees before signing.

### registry.json

Single source of truth for coin metadata: SLIP44 ID, derivation paths, curve type, address encoding, explorer URLs. Codegen reads this to produce:
- `TWCoinType` enum values
- Default derivation paths
- Swift/Kotlin/TS coin type constants

### Rust crates (`rust/`)

New blockchain implementations are increasingly written in Rust. The workspace structure:
- `rust/chains/tw_*` — chain-specific crates (Bitcoin, Ethereum, Solana, Cosmos, etc.)
- `rust/frameworks/tw_utxo`, `tw_substrate`, `tw_ton_sdk` — shared blockchain framework crates
- `rust/tw_any_coin/` — Rust-side dispatcher (mirrors C++ `AnySigner`)
- `rust/tw_coin_registry/` — coin type registry for Rust
- `rust/tw_keypair/`, `tw_hash/`, `tw_encoding/` — shared crypto utilities
- `rust/tw_tests/tests/chains/` — integration tests per chain

Rust crates expose functionality through the C ABI via `tw_any_coin/src/ffi/`. The C++ layer calls into Rust via `src/rust/RustCoinEntry.h`.

### codegen-v2 (`codegen-v2/`)

Rust-based code generator replacing the legacy Ruby `codegen/`. Reads `registry.json` and interface definitions to emit language bindings. Run with:
```bash
cd codegen-v2 && cargo run -- [swift | kotlin | new-blockchain <name> | new-evmchain <name>]
```

### Proto file locations

- Chain proto definitions: `src/proto/{Blockchain}.proto`
- Common types: `src/proto/Common.proto`
- Generated C++: `src/proto/*.pb.h` / `*.pb.cc` (do not edit)
- Generated Swift: `swift/Sources/Generated/Protobuf/`
- Generated Kotlin: `kotlin/wallet-core-kotlin/src/commonMain/generated/`

### Test locations

| Layer | Location |
|-------|----------|
| C++ unit tests | `tests/{Blockchain}/` using GoogleTest |
| Rust chain tests | `rust/tw_tests/tests/chains/{blockchain}/` |
| Android instrumented | `android/app/src/androidTest/` |
| WASM / TS | `wasm/tests/Blockchain/` using mocha |
| iOS / Swift | `swift/Tests/` |

C++ unit tests live under `tests/{Blockchain}/` and are typically organized by functionality (for example, `TWBitcoinSigningTests.cpp`, `TWBitcoinAddressTests.cpp`, `TWBitcoinScriptTests.cpp`), rather than following a single strict filename pattern like `TW{Blockchain}Tests.cpp`.
