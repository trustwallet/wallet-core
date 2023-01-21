// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "Hash.h"
#include "PublicKey.h"

#include <memory>
#include <string>
#include <string_view>

namespace TW {

/// Bech32 string address, used by Cosmos, etc.  Variation are the hash function.
class Bech32Address {
private:
    /// Chain-specific human-readable part.  If missing (""), no prefix check is done.
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Public key hash.
    Data keyHash;

public:
    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(std::string_view addr);

    /// Determines whether a string makes a valid Bech32 address, and the HRP matches.
    static bool isValid(std::string_view addr, std::string_view hrp);

    /// Decodes an address and create an address object out of it.
    /// obj_out:  Pass-by-ref, result is initialized here if possible, it can be a derived address type.
    /// hrp: the expected hrp prefix (if missing ("") no prefix check is done).
    /// \returns true if success.
    static bool decode(std::string_view addr, Bech32Address& obj_out, std::string_view hrp);

    /// Initializes an empty address.
    Bech32Address(std::string hrp_in)
        : hrp(std::move(hrp_in)) {}

    /// Initializes an address with a key hash.
    Bech32Address(std::string hrp, const Data& keyHash)
        : hrp(std::move(hrp)), keyHash(std::move(keyHash)) {}

    /// Initialization from public key --> chain specific hash methods
    Bech32Address(std::string_view hrp, Hash::Hasher hasher, const PublicKey& publicKey);

    void setHrp(std::string_view hrp_in) { hrp = std::move(hrp_in); }
    void setKey(const Data& keyHash_in) { keyHash = std::move(keyHash_in); }

    inline std::string_view getHrp() const { return hrp; }

    inline const Data& getKeyHash() const { return keyHash; }

    /// Encodes the address.
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    bool operator==(const Bech32Address& rhs) const { return hrp == rhs.hrp && keyHash == rhs.keyHash; }

private:
    Bech32Address() = default;
};

} // namespace TW
