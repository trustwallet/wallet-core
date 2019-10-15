// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <TrustWalletCore/TWPolkadotAddressType.h>
#include "PublicKey.h"

namespace TW::Polkadot {

class Address {
public:
    
    /// Variable sized address data
    Data bytes;
    
    /// Address type
    TWPolkadotAddressType addressType;
    
    /// checksum length
    int checksumLength = 0;
    
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address from a string representation.
    Address(const std::string& string);

    /// Initializes an address from a public key.
    Address(const PublicKey& publicKey, TWPolkadotAddressType addrType, int checkSmLength);

    /// Returns a string representation of the address.
    std::string string() const;
};

} // namespace TW::Polkadot
