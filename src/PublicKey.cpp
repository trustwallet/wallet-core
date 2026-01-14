// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PublicKey.h"
#include "PrivateKey.h"
#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include "memzero.h"

#include <iterator>

namespace TW {

bool validateSignatureLength(TWPublicKeyType type, const Data& signature) {
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended: {
        return signature.size() == PublicKey::signatureSize
            || signature.size() == PublicKey::secp256k1SignatureSize;
    }
    default: {
        return signature.size() == PublicKey::signatureSize;
    }
    }
}

/// Determines if a collection of bytes makes a valid public key of the
/// given type.
bool PublicKey::isValid(const Data& data, enum TWPublicKeyType type) {
    return Rust::tw_public_key_is_valid(data.data(), data.size(), static_cast<uint32_t>(type));
}

/// Initializes a public key with a collection of bytes.
///
/// \throws std::invalid_argument if the data is not a valid public key.
PublicKey::PublicKey(const Data& data, enum TWPublicKeyType type)
    : type(type) {
    if (!isValid(data, type)) {
        throw std::invalid_argument("Invalid public key data");
    }
<<<<<<< HEAD
    auto* pubkey = Rust::tw_public_key_create_with_data(data.data(), data.size(), static_cast<uint32_t>(type));
    if (pubkey == nullptr) {
        throw std::invalid_argument("Invalid public key");
=======
    switch (type) {
    case TWPublicKeyTypeStarkex:
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeSECP256k1Extended:
    case TWPublicKeyTypeNIST256p1Extended:
        bytes.reserve(data.size());
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        break;

    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeCURVE25519:
        bytes.reserve(ed25519Size);
        if (data.size() == ed25519Size + 1) {
            std::copy(std::begin(data) + 1, std::end(data), std::back_inserter(bytes));
        } else {
            std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        }
        break;
    case TWPublicKeyTypeED25519Blake2b:
        bytes.reserve(ed25519Size);
        // Sanity check, ensured by isValid() above.
        if (data.size() != ed25519Size) {
            throw std::invalid_argument("Invalid public key size");
        }
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        break;
    case TWPublicKeyTypeED25519Cardano:
        bytes.reserve(cardanoKeySize);
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
>>>>>>> master
    }
    _impl = Rust::wrapTWPublicKey(pubkey);
    Rust::CByteArrayWrapper pubkeyData = Rust::tw_public_key_data(_impl.get());
    bytes = pubkeyData.data;
}

PublicKey::PublicKey(std::shared_ptr<TW::Rust::TWPublicKey> _impl)
    : _impl(_impl) {
    Rust::CByteArrayWrapper pubkeyData = Rust::tw_public_key_data(_impl.get());
    bytes = pubkeyData.data;
    type = static_cast<enum TWPublicKeyType>(Rust::tw_public_key_type(_impl.get()));
}

PublicKey PublicKey::compressed() const {
    auto compressedPubKey = Rust::tw_public_key_compressed(_impl.get());
    if (compressedPubKey == nullptr) {
        return *this;
    }
<<<<<<< HEAD
    return PublicKey(Rust::wrapTWPublicKey(compressedPubKey));
=======
    if (bytes.size() < secp256k1ExtendedSize) {
        throw std::invalid_argument("Invalid public key size");
    }

    Data newBytes(secp256k1Size);
    newBytes[0] = 0x02 | (bytes[64] & 0x01);

    switch (type) {
    case TWPublicKeyTypeSECP256k1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1);

    case TWPublicKeyTypeNIST256p1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1);
    default:
        return *this;
    }
>>>>>>> master
}

PublicKey PublicKey::extended() const {
    auto extendedPubKey = Rust::tw_public_key_extended(_impl.get());
    if (extendedPubKey == nullptr) {
        return *this;
    }
    return PublicKey(Rust::wrapTWPublicKey(extendedPubKey));
}

bool rust_public_key_verify(const Data& key, TWPublicKeyType type, const Data& sig, const Data& msgHash) {
    auto* pubkey = Rust::tw_public_key_create_with_data(key.data(), key.size(), static_cast<uint32_t>(type));
    if (pubkey == nullptr) {
        return {};
    }
    bool verified = Rust::tw_public_key_verify(pubkey, sig.data(), sig.size(), msgHash.data(), msgHash.size());
    Rust::tw_public_key_delete(pubkey);
    return verified;
}

bool PublicKey::verify(const Data& signature, const Data& message) const {
<<<<<<< HEAD
    return Rust::tw_public_key_verify(_impl.get(), signature.data(), signature.size(), message.data(), message.size());
}

bool PublicKey::verifyAsDER(const Data& signature, const Data& message) const {
    return Rust::tw_public_key_verify_as_der(_impl.get(), signature.data(), signature.size(), message.data(), message.size());
=======
    if (!validateSignatureLength(type, signature)) {
        return false;
    }

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
        return ecdsa_verify_digest(&secp256k1, bytes.data(), signature.data(), message.data()) == 0;
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
        return ecdsa_verify_digest(&nist256p1, bytes.data(), signature.data(), message.data()) == 0;
    case TWPublicKeyTypeED25519:
        return ed25519_sign_open(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeED25519Blake2b:
        return ed25519_sign_open_blake2b(message.data(), message.size(), bytes.data(), signature.data()) == 0;
    case TWPublicKeyTypeED25519Cardano: {
        const auto key = subData(bytes, 0, ed25519Size);
        return ed25519_sign_open(message.data(), message.size(), key.data(), signature.data()) == 0;
    }
    case TWPublicKeyTypeCURVE25519: {
        auto ed25519PublicKey = Data();
        ed25519PublicKey.resize(PublicKey::ed25519Size);
        curve25519_pk_to_ed25519(ed25519PublicKey.data(), bytes.data());

        ed25519PublicKey[31] &= 0x7F;
        ed25519PublicKey[31] |= signature[63] & 0x80;

        // remove sign bit
        auto verifyBuffer = Data();
        append(verifyBuffer, signature);
        verifyBuffer[63] &= 127;
        return ed25519_sign_open(message.data(), message.size(), ed25519PublicKey.data(), verifyBuffer.data()) == 0;
    }
    case TWPublicKeyTypeStarkex:
        return rust_public_key_verify(bytes, type, signature, message);
    default:
        throw std::logic_error("Not yet implemented");
    }
}

bool PublicKey::verifyAsDER(const Data& signature, const Data& message) const {
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended: {
        Data sig(64);
        int ret = ecdsa_sig_from_der(signature.data(), signature.size(), sig.data());
        if (ret) {
            return false;
        }
        return ecdsa_verify_digest(&secp256k1, bytes.data(), sig.data(), message.data()) == 0;
    }

    default:
        return false;
    }
}

bool PublicKey::verifyZilliqa(const Data& signature, const Data& message) const {
    if (signature.size() != signatureSize && signature.size() != secp256k1SignatureSize) {
        return false;
    }

    switch (type) {
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeSECP256k1Extended:
        return zil_schnorr_verify(&secp256k1, bytes.data(), signature.data(), message.data(), static_cast<uint32_t>(message.size())) == 0;
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeNIST256p1Extended:
    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeED25519Blake2b:
    case TWPublicKeyTypeED25519Cardano:
    case TWPublicKeyTypeCURVE25519:
    default:
        return false;
    }
>>>>>>> master
}

Data PublicKey::hash(const Data& prefix, Hash::Hasher hasher, bool skipTypeByte) const {
    const auto offset = std::size_t(skipTypeByte ? 1 : 0);
    const auto hash = Hash::hash(hasher, bytes.data() + offset, bytes.size() - offset);

    auto result = Data();
    result.reserve(prefix.size() + hash.size());
    append(result, prefix);
    append(result, hash);
    return result;
}

PublicKey PublicKey::recoverRaw(const Data& signatureRS, byte recId, const Data& messageDigest) {
    auto* pubkey = Rust::tw_public_key_recover_from_signature(signatureRS.data(), signatureRS.size(), messageDigest.data(), messageDigest.size(), recId);
    if (pubkey == nullptr) {
        throw std::invalid_argument("Recover failed");
    }
    return PublicKey(Rust::wrapTWPublicKey(pubkey));
}

PublicKey PublicKey::recover(const Data& signature, const Data& messageDigest) {
    if (signature.size() < secp256k1SignatureSize) {
        throw std::invalid_argument("signature too short");
    }
    auto v = signature[secp256k1SignatureSize - 1];
    // handle EIP155 Eth encoding of V, of the form 27+v, or 35+chainID*2+v
    if (v >= PublicKey::SignatureVOffset) {
        v = !(v & 0x01);
    }
    return recoverRaw(signature, v, messageDigest);
}

bool PublicKey::isValidED25519() const {
    if (type != TWPublicKeyTypeED25519) {
        return false;
    }
<<<<<<< HEAD
    return _impl != nullptr;
}

void PublicKey::cleanup() {
    memzero(bytes.data(), bytes.size());
    _impl = nullptr;
=======
    if (bytes.size() != ed25519Size) {
        return false;
    }
    ge25519 r;
    return ge25519_unpack_negative_vartime(&r, bytes.data()) != 0;
>>>>>>> master
}

} // namespace TW
