// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bech32Address.h"

namespace TW::IoTeX {

/// IoTeX address is a Bech32Address, with "io" prefix and Sha3 Keccak hash.
class Address: public Bech32Address {
public:
    /// Number of bytes in an address.
    static constexpr size_t size = 20;

    static const std::string hrp; // HRP_IOTEX

    static bool isValid(const std::string& addr) { return Bech32Address::isValid(addr, hrp); }

    Address() : Bech32Address(hrp) {}

    /// Initializes an address with a key hash.
    Address(const Data& keyHash) : Bech32Address(hrp, keyHash) {
        if (getKeyHash().size() != Address::size) {
            throw std::invalid_argument("invalid address data");
        }
    }

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey) : Bech32Address(hrp, Hash::HasherKeccak256, publicKey) {
        if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
            throw std::invalid_argument("address may only be an extended SECP256k1 public key");
        }      
    }

    static bool decode(const std::string& addr, Address& obj_out) {
        return Bech32Address::decode(addr, obj_out, hrp);
    }
};

} // namespace TW::IoTeX
