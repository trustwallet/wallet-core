// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AddressV2.h"
#include "Data.h"
#include "../PublicKey.h"

#include <string>
#include <optional>

namespace TW::Cardano {

/// A Cardano-Shelley address, V3 or V2.
class AddressV3 {
  public:
    enum Discrimination: uint8_t {
        Discrim_Production = 0,
        Discrim_Test = 1,
    };

    enum Kind: uint8_t {
        Kind_Sentinel_Low = 2,
        Kind_Single = 3,
        Kind_Group = 4,
        Kind_Account = 5,
        Kind_Multisig = 6,
        Kind_Sentinel_High = 7,
    };

    Discrimination discrimination;

    Kind kind;

    /// key1: spending key or account key
    Data key1;

    /// group key (in case of Group address, empty otherwise)
    Data groupKey;

    /// Used in case of legacy address (V2)
    std::optional<AddressV2> legacyAddressV2;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& addr);

    /// Create a single spending key address
    static AddressV3 createSingle(Discrimination discrimination_in, const TW::Data& spendingKey);
    /// Create a group address
    static AddressV3 createGroup(Discrimination discrimination_in, const TW::Data& spendingKey, const TW::Data& groupKey);
    /// Create an account address
    static AddressV3 createAccount(Discrimination discrimination_in, const TW::Data& accountKey);

    /// Initializes a Cardano address with a string representation.  Throws if invalid.
    explicit AddressV3(const std::string& addr);

    /// Initializes a Cardano address from an extended public key.
    explicit AddressV3(const PublicKey& publicKey);

    /// Initializes a Cardano address from packed data (see data())
    explicit AddressV3(const Data& data);

    /// Copy constructor
    AddressV3(const AddressV3& other);

    void operator=(const AddressV3& other);

    /// Returns the Bech string representation of the address, with default HRP.
    std::string string() const;
    /// Returns the Bech string representation of the address, with given HRP.
    std::string string(const std::string& hrp) const;
    /// Returns the internal Base32 string representation of the address.
    std::string stringBase32() const;

    /// Check validity and parse elements of a string address.  Used internally by isValid and ctor.
    static bool parseAndCheckV3(const std::string& addr, Discrimination& discrimination, Kind& kind, TW::Data& key1, TW::Data& key2);

    /// Return the binary data representation (keys appended, internal format)
    Data data() const;

private:
    AddressV3() : discrimination(Discrim_Production), kind(Kind_Single) {}
};

inline bool operator==(const AddressV3& lhs, const AddressV3& rhs) {
    return lhs.discrimination == rhs.discrimination && lhs.kind == rhs.kind && lhs.key1 == rhs.key1 && lhs.groupKey == rhs.groupKey;
}

} // namespace TW::Cardano

/// Wrapper for C interface.
struct TWCardanoAddress {
    TW::Cardano::AddressV3 impl;
};
