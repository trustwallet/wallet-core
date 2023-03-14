// Copyright © 2017-2023 Trust Wallet.
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
    enum NetworkId: uint8_t {
        Network_Test = 0,
        Network_Production = 1,
    };

    enum Kind: uint8_t {
        Kind_Base = 0, // spending + staking key
        //Kind_Base_Script_Key = 1,
        //Kind_Base_Key_Script_Key = 2,
        //Kind_Base_Script_Script_Key = 3,
        //Kind_Pointer = 4,
        //Kind_Pointer_Script = 5,
        Kind_Enterprise = 6, // spending key
        //Kind_Enterprise_Script = 7,
        //Kind_Bootstrap = 8, // Byron
        Kind_Reward = 14, // // staking key
        //Kind_Reward_Script = 15,
    };

    static const uint8_t HashSize = 28;

    // First byte header (kind, netowrkId) and 2 hashes
    static const uint8_t EncodedSize1 = 1 + HashSize;
    static const uint8_t EncodedSize2 = 1 + 2 * HashSize;

    NetworkId networkId{Network_Production};

    Kind kind{Kind_Base};

    /// raw key/hash bytes
    Data bytes;

    /// Used in case of legacy address (V2)
    std::optional<AddressV2> legacyAddressV2;

    /// Determines whether a string makes a valid address (V3).
    static bool isValid(const std::string& addr);

    /// Determines whether a string makes a valid address, V3 or earlier legacy.
    static bool isValidLegacy(const std::string& addr);

    /// Create a base address, given public key hashes
    static AddressV3 createBase(NetworkId networkId, const TW::Data& spendingKeyHash, const TW::Data& stakingKeyHash);

    /// Create a base address, given public keys
    static AddressV3 createBase(NetworkId networkId, const PublicKey& spendingKey, const PublicKey& stakingKey);

    /// Create a staking (reward) address, given a staking key
    static AddressV3 createReward(NetworkId networkId, const TW::Data& stakingKeyHash);

    /// Initializes a Cardano address with a string representation.  Throws if invalid.
    explicit AddressV3(const std::string& addr);

    /// Initializes a Cardano address from an extended public key.
    explicit AddressV3(const PublicKey& publicKey);

    /// Initializes a Cardano address from packed data (see data())
    explicit AddressV3(const Data& data);

    /// Copy constructor
    AddressV3(const AddressV3& other);

    AddressV3& operator=(const AddressV3& other) noexcept = default;

    /// Returns the Bech string representation of the address, with default HRP.
    std::string string() const;
    /// Returns the Bech string representation of the address, with given HRP.
    std::string string(const std::string& hrp) const;

    /// Hrp of kind
    static std::string getHrp(Kind kind) noexcept;
    /// Check whether data length is correct
    static bool checkLength(Kind kind, size_t length) noexcept;
    /// Check validity of binary address.
    static bool parseAndCheckV3(const TW::Data& raw, NetworkId& networkId, Kind& kind, TW::Data& bytes) noexcept;
    /// Check validity and parse elements of a string address.  Used internally by isValid and ctor.
    static bool parseAndCheckV3(const std::string& addr, NetworkId& networkId, Kind& kind, TW::Data& bytes) noexcept;

    /// Return the binary data representation (keys appended, internal format)
    Data data() const noexcept;
    /// Return the staking address associated to (contained in) this address. Must be a Base address. Empty string is returned on error.
    std::string getStakingAddress() const noexcept;

    // First encoded byte, from networkId and Kind
    static uint8_t firstByte(NetworkId networkId, Kind kind);
    static NetworkId networkIdFromFirstByte(uint8_t first);
    static Kind kindFromFirstByte(uint8_t first);

private:
    AddressV3() = default;
};

inline bool operator==(const AddressV3& lhs, const AddressV3& rhs) {
    return lhs.networkId == rhs.networkId && lhs.kind == rhs.kind && lhs.bytes == rhs.bytes;
}

} // namespace TW::Cardano
