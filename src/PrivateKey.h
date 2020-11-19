// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCurve.h>

namespace TW {

class PrivateKey {
  public:
    /// The number of bytes in a private key.
    static const size_t size = 32;
    /// The number of bytes in an extended private key.
    static const size_t extendedSize = 3 * 32;

    /// The private key bytes.
    Data bytes;
    /// Optional extended part of the key (additional 32 bytes)
    Data extensionBytes;
    /// Optional chain code (additional 32 bytes)
    Data chainCodeBytes;

    /// Determines if a collection of bytes makes a valid private key.
    static bool isValid(const Data& data);

    /// Determines if a collection of bytes and curve make a valid private key.
    static bool isValid(const Data& data, TWCurve curve);

    /// Initializes a private key with an array of bytes.  Size must be exact (normally 32, or 96 for extended)
    explicit PrivateKey(const Data& data);

    /// Initializes a private key from a string of bytes (convenience method).
    explicit PrivateKey(const std::string& data) : PrivateKey(TW::data(data)) {}

    /// Initializes an extended private key with key, extended key, and chain code.
    explicit PrivateKey(const Data& data, const Data& ext, const Data& chainCode);

    PrivateKey(const PrivateKey& other) = default;
    PrivateKey& operator=(const PrivateKey& other) = default;

    PrivateKey(PrivateKey&& other) = default;
    PrivateKey& operator=(PrivateKey&& other) = default;

    virtual ~PrivateKey() { cleanup(); }

    /// Returns the public key for this private key.
    PublicKey getPublicKey(enum TWPublicKeyType type) const;

    /// Signs a digest using the given ECDSA curve.
    Data sign(const Data& digest, TWCurve curve) const;

    /// Signs a digest using the given ECDSA curve and prepends the recovery id (a la graphene)
    /// Only a sig that passes canonicalChecker is returned
    Data sign(const Data& digest, TWCurve curve, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const;

    /// Signs a digest using the given ECDSA curve. The result is encoded with
    /// DER.
    Data signAsDER(const Data& digest, TWCurve curve) const;

    /// Signs a digest using given ECDSA curve, returns schnorr signature
    Data signSchnorr(const Data& message, TWCurve curve) const;

    /// Cleanup contents (fill with 0s), called before destruction
    void cleanup();
};

inline bool operator==(const PrivateKey& lhs, const PrivateKey& rhs) {
    return lhs.bytes == rhs.bytes;
}
inline bool operator!=(const PrivateKey& lhs, const PrivateKey& rhs) {
    return lhs.bytes != rhs.bytes;
}

} // namespace TW

/// Wrapper for C interface.
struct TWPrivateKey {
    TW::PrivateKey impl;
};
