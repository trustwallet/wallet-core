// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PublicKey.h"
#include "PrivateKey.h"
#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/ed25519-donna/ed25519-donna.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/sodium/keypair.h>
#include <TrezorCrypto/zilliqa.h>
#include <ImmutableX/StarkKey.h>

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
        assert(data.size() == ed25519Size); // ensured by isValid() above
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
        break;
    case TWPublicKeyTypeED25519Cardano:
        bytes.reserve(cardanoKeySize);
        std::copy(std::begin(data), std::end(data), std::back_inserter(bytes));
    }
}

PublicKey PublicKey::compressed() const {
    if (type != TWPublicKeyTypeSECP256k1Extended && type != TWPublicKeyTypeNIST256p1Extended) {
        return *this;
    }

    Data newBytes(secp256k1Size);
    assert(bytes.size() >= 65);
    newBytes[0] = 0x02 | (bytes[64] & 0x01);

    assert(type == TWPublicKeyTypeSECP256k1Extended || type == TWPublicKeyTypeNIST256p1Extended);
    switch (type) {
    case TWPublicKeyTypeSECP256k1Extended:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1);

    case TWPublicKeyTypeNIST256p1Extended:
    default:
        std::copy(bytes.begin() + 1, bytes.begin() + secp256k1Size, newBytes.begin() + 1);
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1);
    }
}

PublicKey PublicKey::extended() const {
    Data newBytes(secp256k1ExtendedSize);
    switch (type) {
    case TWPublicKeyTypeSECP256k1:
        ecdsa_uncompress_pubkey(&secp256k1, bytes.data(), newBytes.data());
        return PublicKey(newBytes, TWPublicKeyTypeSECP256k1Extended);
    case TWPublicKeyTypeSECP256k1Extended:
        return *this;
    case TWPublicKeyTypeNIST256p1:
        ecdsa_uncompress_pubkey(&nist256p1, bytes.data(), newBytes.data());
        return PublicKey(newBytes, TWPublicKeyTypeNIST256p1Extended);
    case TWPublicKeyTypeNIST256p1Extended:
        return *this;
    case TWPublicKeyTypeED25519:
    case TWPublicKeyTypeCURVE25519:
    case TWPublicKeyTypeED25519Blake2b:
    case TWPublicKeyTypeED25519Cardano:
        return *this;
    default:
        return *this;
    }
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
    if (signatureRS.size() < 2 * PrivateKey::_size) {
        throw std::invalid_argument("signature too short");
    }
    if (recId >= 4) {
        throw std::invalid_argument("Invalid recId (>=4)");
    }
    if (messageDigest.size() < PrivateKey::_size) {
        throw std::invalid_argument("digest too short");
    }
    TW::Data result(secp256k1SignatureSize);
    if (auto ret = ecdsa_recover_pub_from_sig(&secp256k1, result.data(), signatureRS.data(), messageDigest.data(), recId); ret != 0) {
        throw std::invalid_argument("recover failed " + std::to_string(ret));
    }
    return PublicKey(result, TWPublicKeyTypeSECP256k1Extended);
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
    assert(bytes.size() == ed25519Size);
    ge25519 r;
    return ge25519_unpack_negative_vartime(&r, bytes.data()) != 0;
}

} // namespace TW
