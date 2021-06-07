// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrivateKey.h"

#include "PublicKey.h"

#include <TrezorCrypto/bignum.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/schnorr.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/sodium/keypair.h>

using namespace TW;

bool PrivateKey::isValid(const Data& data) {
    // Check length.  Extended key needs 3*32 bytes.
    if (data.size() != size && data.size() != extendedSize) {
        return false;
    }

    // Check for zero address
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != 0) {
            return true;
        }
    }

    return false;
}

bool PrivateKey::isValid(const Data& data, TWCurve curve)
{
    // check size
    bool valid = isValid(data);
    if (!valid) {
        return false;
    }

    const ecdsa_curve *ec_curve = nullptr;
    switch (curve)
    {
    case TWCurveSECP256k1:
        ec_curve = &secp256k1;
        break;
    case TWCurveNIST256p1:
        ec_curve = &nist256p1;
        break;
    case TWCurveED25519:
    case TWCurveED25519Blake2bNano:
    case TWCurveED25519Extended:
    case TWCurveCurve25519:
    case TWCurveNone:
    default:
        break;
    }

    if (ec_curve != nullptr) {
        bignum256 k;
        bn_read_be(data.data(), &k);
        if (!bn_is_less(&k, &ec_curve->order)) {
            memzero(&k, sizeof(k));
            return false;
        };
    }

    return true;
}

PrivateKey::PrivateKey(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid private key data");
    }
    if (data.size() == extendedSize) {
        // special extended case
        *this = PrivateKey(
            TW::data(data.data(), 32),
            TW::data(data.data() + 32, 32),
            TW::data(data.data() + 64, 32));
    } else {
        // default case
        bytes = data;
    }
}

PrivateKey::PrivateKey(const Data& data, const Data& ext, const Data& chainCode) {
    if (!isValid(data) || !isValid(ext) || !isValid(chainCode)) {
        throw std::invalid_argument("Invalid private key or extended key data");
    }
    bytes = data;
    extensionBytes = ext;
    chainCodeBytes = chainCode;
}

PublicKey PrivateKey::getPublicKey(TWPublicKeyType type) const {
    Data result;
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
        result.resize(PublicKey::secp256k1Size);
        ecdsa_get_public_key33(&secp256k1, bytes.data(), result.data());
        break;
    case TWPublicKeyTypeSECP256k1Extended:
        result.resize(PublicKey::secp256k1ExtendedSize);
        ecdsa_get_public_key65(&secp256k1, bytes.data(), result.data());
        break;
    case TWPublicKeyTypeNIST256p1:
        result.resize(PublicKey::secp256k1Size);
        ecdsa_get_public_key33(&nist256p1, bytes.data(), result.data());
        break;
    case TWPublicKeyTypeNIST256p1Extended:
        result.resize(PublicKey::secp256k1ExtendedSize);
        ecdsa_get_public_key65(&nist256p1, bytes.data(), result.data());
        break;
    case TWPublicKeyTypeED25519:
        result.resize(PublicKey::ed25519Size);
        ed25519_publickey(bytes.data(), result.data());
        break;
    case TWPublicKeyTypeED25519Blake2b:
        result.resize(PublicKey::ed25519Size);
        ed25519_publickey_blake2b(bytes.data(), result.data());
        break;
    case TWPublicKeyTypeED25519Extended:
        // must be extended key
        if (bytes.size() + extensionBytes.size() + chainCodeBytes.size() != extendedSize) {
            throw std::invalid_argument("Invalid extended key");
        }
        result.resize(PublicKey::ed25519ExtendedSize);
        ed25519_publickey_ext(bytes.data(), extensionBytes.data(), result.data());
        // append chainCode to the end of the public key
        std::copy(chainCodeBytes.begin(), chainCodeBytes.end(), result.begin() + 32);
        break;
    case TWPublicKeyTypeCURVE25519:
        result.resize(PublicKey::ed25519Size);
        PublicKey ed25519PublicKey = getPublicKey(TWPublicKeyTypeED25519);
        ed25519_pk_to_curve25519(result.data(), ed25519PublicKey.bytes.data());
        break;
    }
    return PublicKey(result, type);
}

Data PrivateKey::getSharedKey(const PublicKey& pubKey, TWCurve curve) const {
    if (curve != TWCurveSECP256k1) {
        return {};
    }

    Data result(PublicKey::secp256k1ExtendedSize);
    bool success = ecdh_multiply(&secp256k1, bytes.data(),
                                 pubKey.bytes.data(), result.data()) == 0;

    if (success) {
        PublicKey sharedKey(result, TWPublicKeyTypeSECP256k1Extended);
        auto hash = Hash::sha256(sharedKey.compressed().bytes);
        return hash;
    }

    return {};
}

int ecdsa_sign_digest_checked(const ecdsa_curve *curve, const uint8_t *priv_key, const uint8_t *digest, size_t digest_size, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64])) {
    if (digest_size < 32) {
        return -1;
    }
    assert(digest_size >= 32);
    return ecdsa_sign_digest(curve, priv_key, digest, sig, pby, is_canonical);
}

Data PrivateKey::sign(const Data& digest, TWCurve curve) const {
    Data result;
    bool success = false;
    switch (curve) {
    case TWCurveSECP256k1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&secp256k1, bytes.data(), digest.data(), digest.size(), result.data(),
                                    result.data() + 64, nullptr) == 0;
    } break;
    case TWCurveED25519: {
        result.resize(64);
        const auto publicKey = getPublicKey(TWPublicKeyTypeED25519);
        ed25519_sign(digest.data(), digest.size(), bytes.data(), publicKey.bytes.data(), result.data());
        success = true;
    } break;
    case TWCurveED25519Blake2bNano: {
        result.resize(64);
        const auto publicKey = getPublicKey(TWPublicKeyTypeED25519Blake2b);
        ed25519_sign_blake2b(digest.data(), digest.size(), bytes.data(),
                             publicKey.bytes.data(), result.data());
        success = true;
    } break;
    case TWCurveED25519Extended: {
        result.resize(64);
        const auto publicKey = getPublicKey(TWPublicKeyTypeED25519Extended);
        ed25519_sign_ext(digest.data(), digest.size(), bytes.data(), extensionBytes.data(), publicKey.bytes.data(), result.data());
        success = true;
    } break;
    case TWCurveCurve25519: {
        result.resize(64);
        const auto publicKey = getPublicKey(TWPublicKeyTypeED25519);
        ed25519_sign(digest.data(), digest.size(), bytes.data(), publicKey.bytes.data(),
                     result.data());
        const auto sign_bit = publicKey.bytes[31] & 0x80;
        result[63] = result[63] & 127;
        result[63] |= sign_bit;
        success = true;
    } break;
    case TWCurveNIST256p1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&nist256p1, bytes.data(), digest.data(), digest.size(), result.data(),
                                    result.data() + 64, nullptr) == 0;
    } break;
    case TWCurveNone:
    default: 
        break;
    }

    if (!success) {
        return {};
    }
    return result;
}

Data PrivateKey::sign(const Data& digest, TWCurve curve, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const {
    Data result;
    bool success = false;
    switch (curve) {
    case TWCurveSECP256k1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&secp256k1, bytes.data(), digest.data(), digest.size(), result.data() + 1,
                                    result.data(), canonicalChecker) == 0;
    } break;
    case TWCurveED25519: // not supported
    case TWCurveED25519Blake2bNano: // not supported
    case TWCurveED25519Extended: // not supported
    case TWCurveCurve25519:         // not supported
        break;
    case TWCurveNIST256p1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&nist256p1, bytes.data(), digest.data(), digest.size(), result.data() + 1,
                                    result.data(), canonicalChecker) == 0;
    } break;
    case TWCurveNone:
    default:
        break;
    }

    if (!success) {
        return {};
    }

    // graphene adds 31 to the recovery id
    result[0] += 31;
    return result;
}

Data PrivateKey::signAsDER(const Data& digest, TWCurve curve) const {
    Data sig(64);
    bool success =
        ecdsa_sign_digest(&secp256k1, bytes.data(), digest.data(), sig.data(), nullptr, nullptr) == 0;
    if (!success) {
        return {};
    }

    Data resultBytes(72);
    size_t size = ecdsa_sig_to_der(sig.data(), resultBytes.data());

    auto result = Data{};
    std::copy(resultBytes.begin(), resultBytes.begin() + size, std::back_inserter(result));
    return result;
}

Data PrivateKey::signSchnorr(const Data& message, TWCurve curve) const {
    bool success = false;
    Data sig(64);
    switch (curve) {
    case TWCurveSECP256k1: {
        success = zil_schnorr_sign(&secp256k1, bytes.data(), message.data(), static_cast<uint32_t>(message.size()), sig.data()) == 0;
    } break;

    case TWCurveNIST256p1:
    case TWCurveED25519:
    case TWCurveED25519Blake2bNano:
    case TWCurveED25519Extended:
    case TWCurveCurve25519:
    case TWCurveNone:
    default:
        // not support
        break;
    }

    if (!success) {
        return {};
    }
    return sig;
}

void PrivateKey::cleanup() {
    std::fill(bytes.begin(), bytes.end(), 0);
    std::fill(extensionBytes.begin(), extensionBytes.end(), 0);
    std::fill(chainCodeBytes.begin(), chainCodeBytes.end(), 0);
}
