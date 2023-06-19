// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"
#include  "../Polkadot/SS58Address.h"
#include  <TrustWalletCore/TWSS58AddressType.h>

#include <string>

namespace TW::Kusama {

class Address: public SS58Address {
  public:
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string) { return SS58Address::isValid(string, TWSS58AddressTypeKusama); }

    /// Initializes a Kusama address with a string representation.
    Address(const std::string& string): SS58Address(string, TWSS58AddressTypeKusama) {}

    /// Initializes a Kusama address with a public key.
    Address(const PublicKey& publicKey): SS58Address(publicKey, TWSS58AddressTypeKusama) {}
};
} // namespace TW::Kusama
