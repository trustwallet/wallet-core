// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PrivateKey.h"

#include "HexCoding.h"
#include "PublicKey.h"

#include <TrezorCrypto/bignum.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/sodium/keypair.h>
#include <TrezorCrypto/zilliqa.h>
#include <ImmutableX/StarkKey.h>

#include <iterator>

using namespace TW;

Data rust_get_public_from_private(const Data& key, TWPublicKeyType public_type) {
    auto* privkey = Rust::tw_private_key_create_with_data(key.data(), key.size());
    if (privkey == nullptr) {
        return {};
    }
    Data toReturn;

    auto* pubkey = Rust::tw_private_key_get_public_key_by_type(privkey, static_cast<uint32_t>(public_type));
    if (pubkey == nullptr) {
        Rust::tw_private_key_delete(privkey);
        return {};
    }

    Rust::CByteArrayWrapper res = Rust::tw_public_key_data(pubkey);

    Rust::tw_public_key_delete(pubkey);
    Rust::tw_private_key_delete(privkey);
    return res.data;
}

Data rust_private_key_sign(const Data& key, const Data& hash, TWCurve curve) {
    auto* priv = Rust::tw_private_key_create_with_data(key.data(), key.size());
    if (priv == nullptr) {
        return {};
    }
    Rust::CByteArrayWrapper res = Rust::tw_private_key_sign(priv, hash.data(), hash.size(), static_cast<uint32_t>(curve));
    Rust::tw_private_key_delete(priv);
    return res.data;
}

bool PrivateKey::isValid(const Data& data) {
    // Check length
    if (data.size() != _size && data.size() != cardanoKeySize) {
        return false;
    }

    // Check for zero address
    for (size_t i = 0; i < _size; ++i) {
        if (data[i] != 0) {
            return true;
        }
    }

    return false;
}

bool PrivateKey::isValid(const Data& data, TWCurve curve) {
    // check size
    bool valid = isValid(data);
    if (!valid) {
        return false;
    }

    const ecdsa_curve* ec_curve = nullptr;
    switch (curve) {
    case TWCurveSECP256k1:
        ec_curve = &secp256k1;
        break;
    case TWCurveNIST256p1:
        ec_curve = &nist256p1;
        break;
    case TWCurveED25519:
    case TWCurveED25519Blake2bNano:
    case TWCurveED25519ExtendedCardano:
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

TWPrivateKeyType PrivateKey::getType(TWCurve curve) noexcept {
    switch (curve) {
    case TWCurve::TWCurveED25519ExtendedCardano:
        return TWPrivateKeyTypeCardano;
    default:
        return TWPrivateKeyTypeDefault;
    }
}

PrivateKey::PrivateKey(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid private key data");
    }
    bytes = data;
}

PrivateKey::PrivateKey(
    const Data& key1, const Data& extension1, const Data& chainCode1,
    const Data& key2, const Data& extension2, const Data& chainCode2) {
    if (key1.size() != _size || extension1.size() != _size || chainCode1.size() != _size ||
        key2.size() != _size || extension2.size() != _size || chainCode2.size() != _size) {
        throw std::invalid_argument("Invalid private key or extended key data");
    }
    bytes = key1;
    append(bytes, extension1);
    append(bytes, chainCode1);
    append(bytes, key2);
    append(bytes, extension2);
    append(bytes, chainCode2);
}

PublicKey PrivateKey::getPublicKey(TWPublicKeyType type) const {
    Data result;
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
        result.resize(PublicKey::secp256k1Size);
        ecdsa_get_public_key33(&secp256k1, key().data(), result.data());
        break;
    case TWPublicKeyTypeSECP256k1Extended:
        result.resize(PublicKey::secp256k1ExtendedSize);
        ecdsa_get_public_key65(&secp256k1, key().data(), result.data());
        break;
    case TWPublicKeyTypeNIST256p1:
        result.resize(PublicKey::secp256k1Size);
        ecdsa_get_public_key33(&nist256p1, key().data(), result.data());
        break;
    case TWPublicKeyTypeNIST256p1Extended:
        result.resize(PublicKey::secp256k1ExtendedSize);
        ecdsa_get_public_key65(&nist256p1, key().data(), result.data());
        break;
    case TWPublicKeyTypeED25519:
        result.resize(PublicKey::ed25519Size);
        ed25519_publickey(key().data(), result.data());
        break;
    case TWPublicKeyTypeED25519Blake2b:
        result.resize(PublicKey::ed25519Size);
        ed25519_publickey_blake2b(key().data(), result.data());
        break;
    case TWPublicKeyTypeED25519Cardano: {
        // must be double extended key
        if (bytes.size() != cardanoKeySize) {
            throw std::invalid_argument("Invalid extended key");
        }
        Data pubKey(PublicKey::ed25519Size);

        // first key
        ed25519_publickey_ext(key().data(), pubKey.data());
        append(result, pubKey);
        // copy chainCode
        append(result, chainCode());

        // second key
        ed25519_publickey_ext(secondKey().data(), pubKey.data());
        append(result, pubKey);
        append(result, secondChainCode());
    } break;

    case TWPublicKeyTypeCURVE25519: {
        result.resize(PublicKey::ed25519Size);
        PublicKey ed25519PublicKey = getPublicKey(TWPublicKeyTypeED25519);
        ed25519_pk_to_curve25519(result.data(), ed25519PublicKey.bytes.data());
        break;
    }

    case TWPublicKeyTypeStarkex: {
        result = rust_get_public_from_private(this->bytes, type);
        break;
    }
    }
    return PublicKey(result, type);
}

int ecdsa_sign_digest_checked(const ecdsa_curve* curve, const uint8_t* priv_key, const uint8_t* digest, size_t digest_size, uint8_t* sig, uint8_t* pby, int (*is_canonical)(uint8_t by, uint8_t sig[64])) {
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
            success = ecdsa_sign_digest_checked(&secp256k1, key().data(), digest.data(), digest.size(), result.data(), result.data() + 64, nullptr) == 0;
        } break;
        case TWCurveED25519: {
            result.resize(64);
            ed25519_sign(digest.data(), digest.size(), key().data(), result.data());
            success = true;
        } break;
        case TWCurveED25519Blake2bNano: {
            result.resize(64);
            ed25519_sign_blake2b(digest.data(), digest.size(), key().data(), result.data());
            success = true;
        } break;
        case TWCurveED25519ExtendedCardano: {
            result.resize(64);
            ed25519_sign_ext(digest.data(), digest.size(), key().data(), extension().data(), result.data());
            success = true;
        } break;
        case TWCurveCurve25519: {
            result.resize(64);
            const auto publicKey = getPublicKey(TWPublicKeyTypeED25519);
            ed25519_sign(digest.data(), digest.size(), key().data(), result.data());
            const auto sign_bit = publicKey.bytes[31] & 0x80;
            result[63] = result[63] & 127;
            result[63] |= sign_bit;
            success = true;
        } break;
        case TWCurveNIST256p1: {
            result.resize(65);
            success = ecdsa_sign_digest_checked(&nist256p1, key().data(), digest.data(), digest.size(), result.data(), result.data() + 64, nullptr) == 0;
        } break;
    case TWCurveStarkex: {
        result = rust_private_key_sign(key(), digest, curve);
        success = result.size() == 64;
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

Data PrivateKey::sign(const Data& digest, TWCurve curve, int (*canonicalChecker)(uint8_t by, uint8_t sig[64])) const {
    Data result;
    bool success = false;
    switch (curve) {
    case TWCurveSECP256k1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&secp256k1, key().data(), digest.data(), digest.size(), result.data() + 1, result.data(), canonicalChecker) == 0;
    } break;
    case TWCurveED25519:                // not supported
    case TWCurveED25519Blake2bNano:     // not supported
    case TWCurveED25519ExtendedCardano: // not supported
    case TWCurveCurve25519:             // not supported
        break;
    case TWCurveNIST256p1: {
        result.resize(65);
        success = ecdsa_sign_digest_checked(&nist256p1, key().data(), digest.data(), digest.size(), result.data() + 1, result.data(), canonicalChecker) == 0;
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

Data PrivateKey::signAsDER(const Data& digest) const {
    Data sig(64);
    bool success =
        ecdsa_sign_digest(&secp256k1, key().data(), digest.data(), sig.data(), nullptr, nullptr) == 0;
    if (!success) {
        return {};
    }

    Data resultBytes(72);
    size_t size = ecdsa_sig_to_der(sig.data(), resultBytes.data());

    auto result = Data{};
    std::copy(resultBytes.begin(), resultBytes.begin() + size, std::back_inserter(result));
    return result;
}

Data PrivateKey::signZilliqa(const Data& message) const {
    Data sig(64);
    bool success = zil_schnorr_sign(&secp256k1, key().data(), message.data(), static_cast<uint32_t>(message.size()), sig.data()) == 0;

    if (!success) {
        return {};
    }
    return sig;
}

void PrivateKey::cleanup() {
    memzero(bytes.data(), bytes.size());
}
