// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include <TrezorCrypto/hasher.h>

#include <string>
#include <memory>

namespace TW {

/// Bech32 string address, used by Cosmos, etc.  Variation are the hash function.
class Bech32Address {
private:
    /// Chain-specific human-readable part.  If missing (""), no prefix check is done.
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string _hrp;

    /// Public key hash.
    Data _keyHash;

public:
    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& addr);

    /// Determines whether a string makes a valid Bech32 address, and the HRP matches.
    static bool isValid(const std::string& addr, const std::string& hrp);

    /// Decodes an address and create an address object out of it.  
    /// obj_out:  Pass-by-ref, result is initialized here if possible, it can be a derived address type.
    /// hrp: the expected hrp prefix (if missing ("") no prefix check is done).
    /// \returns true if success.
    static bool decode(const std::string& addr, Bech32Address& obj_out, const std::string& hrp);

    /// Initializes an empty address.
    Bech32Address(std::string hrp) : _hrp(std::move(hrp)) {}

    /// Initializes an address with a key hash.
    Bech32Address(std::string hrp, Data keyHash) : _hrp(std::move(hrp)), _keyHash(std::move(keyHash)) {}

    /// Initialization from public key --> chain specific hash methods
    Bech32Address(std::string hrp, HasherType hasher, const PublicKey& publicKey);

    void setHrp(std::string hrp) { _hrp = std::move(hrp); }
    void setKey(Data keyHash) { _keyHash = std::move(keyHash); }

    inline const std::string& getHrp() const { return _hrp; }

    inline const Data& getKeyHash() const { return _keyHash; }

    /// Encodes the address.
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    bool operator==(const Bech32Address& rhs) const { return _hrp == rhs._hrp && _keyHash == rhs._keyHash; }

private:
    Bech32Address() = default;
};

} // namespace TW
