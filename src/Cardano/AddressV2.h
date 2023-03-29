// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Cardano {

/*
 * ADA Cardano
 * 
 * Address scheme V2 is supported, V1 not (but they are accepted as valid).
 * Derivation is BIP39, default derivation path is "m/44'/1815'/0'/0/0", with last element being the account number.
 * Curve is ED25519 with special variations, custom logic in HDWallet and TrezorCrypto lib.
 * Private key is ED25519: 32-byte PK is extended with 32-byte extra extension, and 32-byte chain code.
 * Private key is derived from mnemonic raw entropy (not seed, as in other cases); 96-byte secret is generated (pk, extension, and chain code).
 * Public key is 64-byte: the 32-byte ED25519 public key plus the 32-byte chain code of the PK.
 * Address derivation: Only V2, type 0 (=public key) addresses are generated.
 * - CBOR binary scheme is used inside addresses.
 * - root hash: The following CBOR data is hashed: [0, [0, pubkey64], {}], first SHA3_256, then Blake2B.
 * - attributes: CBOR encoding of an empty map ({}, empty in V2)
 * - address payload is CBOR encoding of [root, attributes, type=0]
 * - address data is CBOR encoding of [tag 24 (payload), CRC32(payload)]
 * - address is Base58 encode of address data.
 * 
 * Address compatibility:
 * AdaLite uses V1 with 12-word mnemonic phrases, V2 with 15-word phrases, and standard BIP39 derivation, with gap lookup.  
 * So AdaLite mnemonic wallets can be imported into TW, but default-12-word TW wallets cannot be imported to AdaLite, as it uses V1 for it.
 * Daedalus (official full node wallet) uses V1, non-standard derivation scheme, and non-sequential random account indexes.  Not compatible.
 */

/// A Cardano address.  Type is 0 (public key).
class AddressV2 {
  public:
    /// root key
    Data root;

    /// Additional info, CBOR-encoded
    Data attrs;

    /// Type; 0: public key.
    TW::byte type{};

    static const TW::byte PayloadTag = 24;
    
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Cardano address with a string representation.  Throws if invalid.
    explicit AddressV2(const std::string& string);

    /// Initializes a V2, public key type Cardano address from an extended public key.
    explicit AddressV2(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    /// compute hash of public key, for address root
    static TW::Data keyHash(const TW::Data& xpub);

    /// Get the CBOR address data (before Base58 encoding)
    TW::Data getCborData() const;

    /// Check validity and parse elements of a string address.  Throws on error. Used internally by isValid and ctor.
    static bool parseAndCheck(const std::string& addr, TW::Data& root_out, TW::Data& attrs_out, TW::byte& type_out);
};

inline bool operator==(const AddressV2& lhs, const AddressV2& rhs) {
    return lhs.root == rhs.root && lhs.attrs == rhs.attrs && lhs.type == rhs.type;
}

} // namespace TW::Cardano
