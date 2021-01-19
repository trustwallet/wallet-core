// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>
#include <vector>

namespace TW::Flow {

/*
enum HashAlgo {
    HashAlgo_Sha2 = 1,
    HashAlgo_Sha3 = 3
};

enum SignAlgo {
    SignAlgo_Ecdsa_P256 = 2,
    SignAlgo_Ecdsa_secp256k1 = 3,
};

class KeyStruct {
public:
    std::string key;
    enum HashAlgo hashAlgo;
    enum SignAlgo signAlgo;
    int weight;
    bool revoked;

    // create a default secp256k1 key, with SHA-2, and weight 1000
    KeyStruct(const std::string& key) :
        key(key), hashAlgo(HashAlgo_Sha2), signAlgo(SignAlgo_Ecdsa_secp256k1), weight(1000), revoked(false) {}
};
*/

class Address {
public:
    //std::vector<KeyStruct> keys; // TODO do we need the keys here?

    std::string accountId;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Flow address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Flow address with a public key.
    //explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    // TODO: Complete equality operator
    return true;
}

} // namespace TW::Flow

/// Wrapper for C interface.
struct TWFlowAddress {
    TW::Flow::Address impl;
};
