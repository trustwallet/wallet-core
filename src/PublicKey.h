// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Hash.h"

#include <TrustWalletCore/TWPublicKeyType.h>

#include <cassert>
#include <stdexcept>

namespace TW {

class PublicKey {
  public:
    /// The number of bytes in a secp256k1 and nist256p1 public key.
    static const size_t secp256k1Size = 33;

    /// The number of bytes in an ed25519 public key.
    static const size_t ed25519Size = 32;

    /// The number of bytes in an starkex public key.
    static const size_t starkexSize = 32;

    /// The number of bytes in a Cardano public key (two ed25519 public key + chain code).
    static const size_t cardanoKeySize = 2 * 2 * 32;

    /// The number of bytes in a secp256k1 and nist256p1 extended public key.
    static const size_t secp256k1ExtendedSize = 65;

    /// The number of bytes in a secp256k1 signature.
    static const size_t secp256k1SignatureSize = 65;

    /// Magic number used in V compnent encoding
    static const byte SignatureVOffset = 27;

    /// The public key bytes.
    Data bytes;

    /// The type of the public key.
    ///
    /// This has information about the elliptic curve and other parameters
    /// used when generating the public key.
    enum TWPublicKeyType type;

    /// Determines if a collection of bytes makes a valid public key of the
    /// given type.
    static bool isValid(const Data& data, enum TWPublicKeyType type);

    /// Initializes a public key with a collection of bytes.
    ///
    /// \throws std::invalid_argument if the data is not a valid public key.
    explicit PublicKey(const Data& data, enum TWPublicKeyType type);

    /// Determines if this is a compressed public key.
    bool isCompressed() const {
        return type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended;
    }

    /// Returns a compressed version of this public key.
    PublicKey compressed() const;

    /// Returns an extended version of this public key.
    PublicKey extended() const;

    /// Verifies a signature for the provided message.
    bool verify(const Data& signature, const Data& message) const;

    /// Verifies a signature in DER format.
    bool verifyAsDER(const Data& signature, const Data& message) const;

    /// Verifies a Zilliqa schnorr signature for the provided message.
    bool verifyZilliqa(const Data& signature, const Data& message) const;

    /// Computes the public key hash.
    ///
    /// The public key hash is computed by applying the hasher to the public key
    /// bytes and then prepending the prefix.
    Data hash(const Data& prefix, Hash::Hasher hasher = Hash::HasherSha256ripemd, bool skipTypeByte = false) const;

    /// Recover public key (SECP256k1Extended) from signature R, S, V values
    /// signatureRS: 2x32 bytes with the R and S values
    /// recId: the recovery ID, a.k.a. V value, 0 <= v < 4
    /// messageDigest: message digest (hash) to be signed
    /// Throws on invalid data.
    static PublicKey recoverRaw(const Data& signatureRS, byte recId, const Data& messageDigest);

    /// Recover public key from signature (SECP256k1Extended)
    /// signature: 65-byte signature (R, S, and V). V can have higher value bits, as used by Ethereum (for values over 27 the negated last bit is taken).
    /// messageDigest: message digest (hash) to be signed
    /// Throws on invalid data.
    /// Naming is kept for backwards compatibility.
    static PublicKey recover(const Data& signature, const Data& messageDigest);

    /// Check if this key makes a valid ED25519 key (it is on the curve)
    bool isValidED25519() const;
};

inline bool operator==(const PublicKey& lhs, const PublicKey& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const PublicKey& lhs, const PublicKey& rhs) {
    return lhs.bytes != rhs.bytes;
}

} // namespace TW

/// Wrapper for C interface.
struct TWPublicKey {
    TW::PublicKey impl;
};
