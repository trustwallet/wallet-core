# Adding Support for a New Blockchain

If you haven't, first read the [guide to contributing](contributing.md). It contains important information about the library and how to contribute.

## Blockchain definitions

The first step to adding a blockchain is to define its configuration parameters. Add the definition to the `coins.json` file. Then run `tools/generate-files` to generate the C++ code from that. Add the corresponding definition to `TWCoinType.h`. After doing this add tests to `TWCoinTypeConfigTests.cpp` and `CoinTests.cpp`. Run the tests and make sure everything is passing before moving on to the next step. Create a commit with this change \(don't create a pull request yet\).

## C++ Implementation

Implement the required functionality in C++. The code should be put in the `src/X` folder where `X` is the name of the Blockchain.

Don't just dump an existing codebase in the repo. The code needs to follow the code style and use existing hashing and cryptographic functionality if possible. Adding new dependencies is something we want to avoid at all costs. We want to keep the codebase and the binary library as small as possible.

If you do need to add a new cryptographic function or other building block please do so as a separate PR from the blockchain implementation.

### Address encoding/decoding

The first step is to support the address format specific to the blockchain. Create `src/Blockchain/Address.h` and `src/Blockchain/Address.cpp` where `Blockchain` is the blockchain name. Put this in the header file:

```text
// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

namespace TW::Blockchain {

class Address {
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address from a string representation.
    Address(const std::string& string);

    /// Initializes an address from a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

} // namespace TW::Blockchain
```

Replace `Blockchain` with the actual blockchain name.

At minimum the address needs a string validation static method, a string constructor, a constructor from a public key, and a method to convert back to a string. Make sure you can parse a string representation of an address and detect invalid addresses. Write unit tests for this. Also make sure that you can derive an address string from a private key. Write unit tests for this as well.

For an example of this have a look at Cosmos [Address.h](https://github.com/TrustWallet/wallet-core/blob/master/src/Cosmos/Address.h) and [Address.cpp](https://github.com/TrustWallet/wallet-core/blob/master/src/Cosmos/Address.cpp).

Implement address validation and derivation in `src/Coin.cpp`.

### Transaction signing

The second step is supporting signing of transactions. Create `src/Blockchain/Signer.h` and `src/Blockchain/Signer.cpp` where `Blockchain` is the blockchain name. Make sure you can generate a valid signature and a valid signed and encoded transaction. Write a unit tests for this.

For an example of this have a look at Binance's [Signer.h](https://github.com/TrustWallet/wallet-core/blob/master/src/Binance/Signer.h) and [Signer.cpp](https://github.com/TrustWallet/wallet-core/blob/master/src/Binance/Signer.cpp).

### Tests

The tests should be put in `tests/X` where `X` is the name of the blockchain. All C++ code needs to be unit tested. This will be enforced automatically when the pull request is created. If you code coverage goes down your pull request will be rejected.

The C++ implementation with tests should be the second commit.

## C Interface

Once you are satisfied with your C++ implementation write a C interface for it. The C interface needs to be as small as possible so that clients don't need to worry about implementation details. If you are implementing blockchain `X` create a `TWXAddress.h` to handle addresses associated to the blockchain and `TWXSigner.h` to handle transaction signing.

Generate the idiomatic interface code by running `tools/generate-files`. If possible test the interface on Android, iOS and TypeScript. Optionally add integration test to each platform. This is required only if the interface is significantly different than the interface used for other blockchains.

The C interface, any Protobuf models, and integration tests should be third commit.

## Blockchain checklist

The above steps are summarized below as a checklist:

* [ ] Add the coin definition to `coins.json` and `TWCoinType`.
* [ ] Implement functionality in C++. Put it in a subfolder of `src/`.
  * [ ] Address \(if necessary\)
  * [ ] Transaction \(if necessary\)
  * [ ] Signer
* [ ] Write unit tests. Put them in a subfolder of `tests/`.
  * [ ] `Mnemonic phrase - > Address` derivation test. Put this test in the `CoinTests.cpp` file
* [ ] Add relevant constants in `TWP2SHPrefix`, `TWEthereymChainID`, `TWHRP`, etc., as necessary.
* [ ] Implement address validation and derivation in `src/Coin.cpp`.
* [ ] Write interface header in `include/TrustWalletCore` and implement the interface in `src/interface`.
  * [ ] Address interface \(if necessary\).
  * [ ] Signing interface.
* [ ] Validate generated code in Android an iOS projects. Write integration tests for each.
* [ ] Add a coin icon to "Supported Blockchains" section in `README.md`
  * [ ] Upload coin icon to [TrustWallet/tokens](https://github.com/TrustWallet/tokens) if necessary

