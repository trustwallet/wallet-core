// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../Cosmos/Address.h"

#include <TrustWalletCore/TWHRP.h>

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

namespace TW::Zilliqa {

  static bool isValidAddress(const std::string& address) {
    return Cosmos::Address::isValid(address, HRP_ZILLIQA);
  }

  static Cosmos::Address Address(const PublicKey& publicKey) {
    const auto hashed = Hash::sha256(publicKey.bytes);
    auto keyHash = Data(20);
    std::copy(hashed.end() - 20, hashed.end(), keyHash.begin());

    return Cosmos::Address(HRP_ZILLIQA, keyHash);
  }

  static Cosmos::Address Address(const Data& keyHash) {
    return Cosmos::Address(HRP_ZILLIQA, keyHash);
  }

} // namespace TW::Zilliqa

#pragma clang diagnostic pop

/// Wrapper for C interface.
struct TWZilliqaAddress {
    TW::Cosmos::Address impl;
};
