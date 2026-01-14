// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PrivateKey.h"

#include "HexCoding.h"
#include "PublicKey.h"

#include <memzero.h>

#include <iterator>

using namespace TW;

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
    return Rust::tw_private_key_is_valid(data.data(), data.size(), static_cast<uint32_t>(curve));
}

TWPrivateKeyType PrivateKey::getType(TWCurve curve) noexcept {
    switch (curve) {
    case TWCurve::TWCurveED25519ExtendedCardano:
        return TWPrivateKeyTypeCardano;
    default:
        return TWPrivateKeyTypeDefault;
    }
}

<<<<<<< HEAD
=======
PrivateKey::PrivateKey(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid private key data");
    }
    bytes = data;
}

PrivateKey::PrivateKey(Data&& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid private key data");
    }
    bytes = std::move(data);
}

>>>>>>> master
PrivateKey::PrivateKey(const Data& data, TWCurve curve) {
    if (!isValid(data, curve)) {
        throw std::invalid_argument("Invalid private key data");
    }
    bytes = data;
    _curve = curve;
    auto* privkey = Rust::tw_private_key_create_with_data(data.data(), data.size(), static_cast<uint32_t>(curve));
    if (privkey == nullptr) {
        throw std::invalid_argument("Invalid private key");
    }
    _impl = Rust::wrapTWPrivateKey(privkey);
}

<<<<<<< HEAD
PrivateKey::PrivateKey(const std::string& data, TWCurve curve) {
    auto bytes = TW::data(data);
    *this = PrivateKey(bytes, curve);
    memzero(bytes.data(), bytes.size());
=======
PrivateKey::PrivateKey(Data&& data, TWCurve curve) {
    if (!isValid(data, curve)) {
        throw std::invalid_argument("Invalid private key data");
    }
    bytes = std::move(data);
    _curve = curve;
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
>>>>>>> master
}

PrivateKey::PrivateKey(
    const Data& key1, const Data& extension1, const Data& chainCode1,
    const Data& key2, const Data& extension2, const Data& chainCode2,
    TWCurve curve) {
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
    _curve = curve;
    auto* privkey = Rust::tw_private_key_create_with_data(bytes.data(), bytes.size(), static_cast<uint32_t>(curve));
    if (privkey == nullptr) {
        throw std::invalid_argument("Invalid private key");
    }
    _impl = Rust::wrapTWPrivateKey(privkey);
}

<<<<<<< HEAD
PublicKey PrivateKey::getPublicKey(TWPublicKeyType type) const { 
    auto* pubkey = Rust::tw_private_key_get_public_key_by_type(_impl.get(), static_cast<uint32_t>(type));
    if (pubkey == nullptr) {
        return PublicKey(Data(), type);
=======
PrivateKey& PrivateKey::operator=(const PrivateKey& other) noexcept {
    if (this != &other) {
        cleanup();
        bytes = other.bytes;
        _curve = other._curve;
    }
    return *this;
}

PrivateKey& PrivateKey::operator=(PrivateKey&& other) noexcept {
    if (this != &other) {
        cleanup();
        bytes = std::move(other.bytes);
        _curve = other._curve;
    }
    return *this;
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
>>>>>>> master
    }
    return PublicKey(Rust::wrapTWPublicKey(pubkey));
}

Data PrivateKey::sign(const Data& digest) const {
    Rust::CByteArrayWrapper res = Rust::tw_private_key_sign(_impl.get(), digest.data(), digest.size());
    return res.data;
}

Data PrivateKey::sign(const Data& digest, int (*canonicalChecker)(uint8_t by, const uint8_t sig[64])) const {
    Rust::CByteArrayWrapper res = Rust::tw_private_key_sign_canonical(_impl.get(), digest.data(), digest.size(), canonicalChecker);
    return res.data;
}

Data PrivateKey::signAsDER(const Data& digest) const {
    Rust::CByteArrayWrapper res = Rust::tw_private_key_sign_as_der(_impl.get(), digest.data(), digest.size());
    return res.data;
}

void PrivateKey::cleanup() {
    memzero(bytes.data(), bytes.size());
    _impl = nullptr;
}
