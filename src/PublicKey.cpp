// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PublicKey.h"
#include "PrivateKey.h"
#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include <iterator>

namespace TW {

/// Determines if a collection of bytes makes a valid public key of the
/// given type.
bool PublicKey::isValid(const Data& data, enum TWPublicKeyType type) {
    const auto size = data.size();
    if (size == 0) {
        return false;
    }
    switch (type) {
    case TWPublicKeyTypeED25519:
        return size == ed25519Size || (size == ed25519Size + 1 && data[0] == 0x01);
    case TWPublicKeyTypeCURVE25519:
    case TWPublicKeyTypeED25519Blake2b:
        return size == ed25519Size;
    case TWPublicKeyTypeED25519Cardano:
        return size == cardanoKeySize;
    case TWPublicKeyTypeSECP256k1:
    case TWPublicKeyTypeNIST256p1:
    case TWPublicKeyTypeSchnorr:
    case TWPublicKeyTypeZILLIQASchnorr:
        return size == secp256k1Size && (data[0] == 0x02 || data[0] == 0x03);
    case TWPublicKeyTypeSECP256k1Extended:
    case TWPublicKeyTypeNIST256p1Extended:
        return size == secp256k1ExtendedSize && data[0] == 0x04;
    case TWPublicKeyTypeStarkex:
        return size == starkexSize;
    default:
        return false;
    }
}

/// Initializes a public key with a collection of bytes.
///
/// \throws std::invalid_argument if the data is not a valid public key.
PublicKey::PublicKey(const Data& data, enum TWPublicKeyType type)
    : type(type) {
    if (!isValid(data, type)) {
        throw std::invalid_argument("Invalid public key data");
    }
    auto* pubkey = Rust::tw_public_key_create_with_data(data.data(), data.size(), static_cast<uint32_t>(type));
    if (pubkey != nullptr) {
        _impl = Rust::wrapTWPublicKey(pubkey);
        auto pubkeyData = Rust::tw_public_key_data(_impl.get());
        bytes = Data(pubkeyData.data, pubkeyData.data + pubkeyData.size);
    } else {
        _impl = nullptr;
        bytes = data;
    }
}

PublicKey::PublicKey(std::shared_ptr<TW::Rust::TWPublicKey> _impl)
    : _impl(_impl) {
    auto pubkeyData = Rust::tw_public_key_data(_impl.get());
    bytes = Data(pubkeyData.data, pubkeyData.data + pubkeyData.size);
    type = static_cast<enum TWPublicKeyType>(Rust::tw_public_key_type(_impl.get()));
}

PublicKey PublicKey::compressed() const {
    auto compressedPubKey = Rust::tw_public_key_compressed(_impl.get());
    if (compressedPubKey == nullptr) {
        return *this;
    }
    return PublicKey(Rust::wrapTWPublicKey(compressedPubKey));
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
    return Rust::tw_public_key_verify(_impl.get(), signature.data(), signature.size(), message.data(), message.size());
}

bool PublicKey::verifyAsDER(const Data& signature, const Data& message) const {
    return Rust::tw_public_key_verify_as_der(_impl.get(), signature.data(), signature.size(), message.data(), message.size());
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
    return _impl != nullptr;
}

} // namespace TW
