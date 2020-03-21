// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/SegwitAddress.h"

#include <string>

namespace TW::BitcoinGold {

/// BitcoinGold segwit address
class SegwitAddress : public TW::Bitcoin::SegwitAddress {

  public:

    /// Initializes a Bech32 address with a human-readable part, a witness
    /// version, and a witness program.
    SegwitAddress(std::string hrp, int witver, std::vector<uint8_t> witprog)
        : TW::Bitcoin::SegwitAddress(hrp, witver, witprog) {}

    /// Initializes a Bech32 address with a public key and a HRP prefix.
    SegwitAddress(const PublicKey& publicKey, int witver, std::string hrp)
        : TW::Bitcoin::SegwitAddress(publicKey, witver, hrp) {}

    bool operator==(const SegwitAddress& rhs) const {
        return hrp == rhs.hrp && witnessVersion == rhs.witnessVersion &&
               witnessProgram == rhs.witnessProgram;
    }

  private:
    SegwitAddress() = default;
};

} // namespace TW::BitcoinGold

/// Wrapper for C interface.
struct TWBitcoinGoldSegwitAddress {
    TW::BitcoinGold::SegwitAddress impl;
};


