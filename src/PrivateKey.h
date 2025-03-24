// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWPrivateKeyType.h>
#include <TrustWalletCore/TWCurve.h>

#include <optional>

namespace TW {

class PrivateKey {
  public:
    /// The number of bytes in a private key.
    static const size_t _size = 32;
    /// The number of bytes in a Cardano key (two extended ed25519 keys + chain code)
    static const size_t cardanoKeySize = 2 * 3 * 32;

    /// The private key bytes:
    /// - common case: 'size' bytes
    /// - double extended case: 'cardanoKeySize' bytes, key+extension+chainCode+second+secondExtension+secondChainCode
    Data bytes;

    /// Optional members for extended keys and second extended keys
    Data key() const { return subData(bytes, 0, 32); }
    Data extension() const { return subData(bytes, 32, 32); }
    Data chainCode() const { return subData(bytes, 2*32, 32); }
    Data secondKey() const { return subData(bytes, 3*32, 32); }
    Data secondExtension() const { return subData(bytes, 4*32, 32); }
    Data secondChainCode() const { return subData(bytes, 5*32, 32); }

    /// Determines if a collection of bytes makes a valid private key.
    static bool isValid(const Data& data);

    /// Determines if a collection of bytes and curve make a valid private key.
    static bool isValid(const Data& data, TWCurve curve);

    // obtain private key type used by the curve/coin
    static TWPrivateKeyType getType(TWCurve curve) noexcept;

    /// Initializes a private key with an array of bytes.  Size must be exact (normally 32, or 192 for extended)
    /// @deprecated Use PrivateKey(const Data& data, TWCurve curve) instead
    explicit PrivateKey(const Data& data);

    /// Initializes a private key with an array of bytes and a curve.  
    /// Size of the data must be exact (normally 32, or 192 for extended)
    /// Signing functions will throw an exception if the provided curve is different from the one specified.
    explicit PrivateKey(const Data& data, TWCurve curve);

    /// Initializes a private key from a string of bytes.
    /// @deprecated Use PrivateKey(const std::string& data, TWCurve curve) instead
    explicit PrivateKey(const std::string& data) : PrivateKey(TW::data(data)) {}

    /// Initializes a private key from a string of bytes and a curve.
    /// Signing functions will throw an exception if the provided curve is different from the one specified.
    explicit PrivateKey(const std::string& data, TWCurve curve) : PrivateKey(TW::data(data), curve) {}

    /// Initializes a Cardano style key
    /// @deprecated Use PrivateKey(const Data& bytes1, const Data& extension1, const Data& chainCode1, const Data& bytes2, const Data& extension2, const Data& chainCode2, TWCurve curve) instead
    explicit PrivateKey(
        const Data& bytes1, const Data& extension1, const Data& chainCode1,
        const Data& bytes2, const Data& extension2, const Data& chainCode2);
        
    /// Initializes a Cardano style key with a specified curve.
    /// Signing functions will throw an exception if the provided curve is different from the one specified.
    explicit PrivateKey(
        const Data& bytes1, const Data& extension1, const Data& chainCode1,
        const Data& bytes2, const Data& extension2, const Data& chainCode2,
        TWCurve curve);

    PrivateKey(const PrivateKey& other) = default;
    PrivateKey& operator=(const PrivateKey& other) = default;

    PrivateKey(PrivateKey&& other) = default;
    PrivateKey& operator=(PrivateKey&& other) = default;

    virtual ~PrivateKey() { cleanup(); }

    /// Returns the public key for this private key.
    PublicKey getPublicKey(enum TWPublicKeyType type) const;

    /// Signs a digest using the given ECDSA curve.
    /// If constructed with a curve, an exception will be thrown if the curve does not match the one specified.
    /// @deprecated Use sign(const Data& digest) instead
    Data sign(const Data& digest, TWCurve curve) const;

    /// Signs a digest using the given ECDSA curve at the time of construction.
    /// IF constructed without a curve, an exception will be thrown.
    Data sign(const Data& digest) const;

    /// Signs a digest using the given ECDSA curve and prepends the recovery id (a la graphene)
    /// Only a sig that passes canonicalChecker is returned
    /// If constructed with a curve, an exception will be thrown if the curve does not match the one specified.
    /// @deprecated Use sign(const Data& digest, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) instead
    Data sign(const Data& digest, TWCurve curve, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const;

    /// Signs a digest using the given ECDSA curve and prepends the recovery id (a la graphene)
    /// Only a sig that passes canonicalChecker is returned
    Data sign(const Data& digest, int (*canonicalChecker)(uint8_t by, uint8_t sig[64])) const;

    /// Signs a digest using the given ECDSA curve. The result is encoded with
    /// DER.
    /// If constructed with a curve, an exception will be thrown if the curve does not match SECP256k1.
    Data signAsDER(const Data& digest) const;

    /// Signs a digest using given ECDSA curve, returns Zilliqa schnorr signature
    /// If constructed with a curve, an exception will be thrown if the curve does not match SECP256k1.
    Data signZilliqa(const Data& message) const;

    /// Cleanup contents (fill with 0s), called before destruction
    void cleanup();
private:
    std::optional<TWCurve> _curve = std::nullopt;
};

} // namespace TW

/// Wrapper for C interface.
struct TWPrivateKey {
    TW::PrivateKey impl;
};
