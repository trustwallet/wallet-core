// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "Data.h"

#include <cstdint>
#include <string>
#include <tuple>

namespace TW::Bitcoin {

/// A Segwit address.
/// Note: Similar to Bech32Address, but it differs enough so that reuse makes no sense.
/// See BIP173 https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki
class SegwitAddress {
  public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Witness program version.
    byte witnessVersion;

    /// Witness program.
    Data witnessProgram;

    // Prefix for Bitcoin Testnet Segwit addresses
    static constexpr auto TestnetPrefix = "tb";

    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid Bech32 address, and the HRP
    /// matches.
    static bool isValid(const std::string& string, const std::string& hrp);

    /// Initializes a Bech32 address with a human-readable part, a witness
    /// version, and a witness program.
    SegwitAddress(std::string hrp, byte witver, Data witprog)
        : hrp(std::move(hrp)), witnessVersion(witver), witnessProgram(std::move(witprog)) {}

    /// Initializes a segwit-version-0 Bech32 address with a public key and a HRP prefix.
    /// Taproot (v>=1) is not supported by this method.
    SegwitAddress(const PublicKey& publicKey, std::string hrp);

    /// Create a testnet address
    static SegwitAddress createTestnetFromPublicKey(const PublicKey& publicKey) { return SegwitAddress(publicKey, TestnetPrefix); }

    /// Decodes a SegWit address.
    ///
    /// \returns a tuple with the address, hrp, and a success flag.
    static std::tuple<SegwitAddress, std::string, bool> decode(const std::string& addr);

    /// Encodes the SegWit address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    /// Initializes a Bech32 address with raw data.
    static std::pair<SegwitAddress, bool> fromRaw(const std::string& hrp, const Data& data);

    bool operator==(const SegwitAddress& rhs) const {
        return hrp == rhs.hrp && witnessVersion == rhs.witnessVersion &&
               witnessProgram == rhs.witnessProgram;
    }

  private:
    SegwitAddress() = default;
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWSegwitAddress {
    TW::Bitcoin::SegwitAddress impl;
};
