// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <cstdint>
#include <string>

namespace TW::Bitcoin {

/// A Segwit address.
/// Note: Similar to Bech32Address, but it differs enough so that reuse makes no sense.
class SegwitAddress {
  public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Witness program version.
    int witnessVersion;

    /// Witness program.
    std::vector<uint8_t> witnessProgram;

    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid Bech32 address, and the HRP
    /// matches.
    static bool isValid(const std::string& string, const std::string& hrp);

    /// Initializes a Bech32 address with a human-readable part, a witness
    /// version, and a witness program.
    SegwitAddress(std::string hrp, int witver, std::vector<uint8_t> witprog)
        : hrp(std::move(hrp)), witnessVersion(witver), witnessProgram(std::move(witprog)) {}

    /// Initializes a Bech32 address with a public key and a HRP prefix.
    SegwitAddress(const PublicKey& publicKey, int witver, std::string hrp);

    /// Decodes a SegWit address.
    ///
    /// \returns a pair with the address and a success flag.
    static std::pair<SegwitAddress, bool> decode(const std::string& addr);

    /// Encodes the SegWit address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    /// Initializes a Bech32 address with raw data.
    static std::pair<SegwitAddress, bool> fromRaw(const std::string& hrp,
                                                  const std::vector<uint8_t>& data);

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
