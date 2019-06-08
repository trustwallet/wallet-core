// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Memo.h"

#include <stdexcept>

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/rand.h>
#include <boost/lexical_cast.hpp>
#include <TrustWalletCore/TWPublicKeyType.h>

#include "../Hash.h"
#include "../HexCoding.h"
#include "../Bravo/Serialization.h"

#include "Address.h"
#include "AES.h"

using namespace TW::Bitshares;
using Data = TW::Data;
using json = nlohmann::json;

Memo::Memo(const PrivateKey& senderKey, const PublicKey& recipientKey, const std::string& message, uint64_t nonce)
        : from(senderKey.getPublicKey(TWPublicKeyTypeSECP256k1)), to(recipientKey) {
    if (recipientKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Recipient's public key is not a secp25k1 public key.");
    }

    if (message.empty()) {
        throw std::invalid_argument("Message is empty.");
    }

    if (!nonce) {
        random_buffer(reinterpret_cast<uint8_t *>(&nonce), sizeof(nonce));
    }
    this->nonce = nonce;

    // We hash (string(nonce) || hex_string(sharedSecret)) with sha512 to get 64 bytes.
    // The first 32 bytes will be used as encryption key and the next 16 as the IV.
    Data encryptionKeyPlusIV = Hash::sha512(boost::lexical_cast<std::string>(nonce)
                                             + hex(getSharedSecret(senderKey, recipientKey)));

    Data encryptionKey = Data(encryptionKeyPlusIV.begin(), encryptionKeyPlusIV.begin() + 32);
    Data iv = Data(encryptionKeyPlusIV.begin() + 32, encryptionKeyPlusIV.end());

    // Encrypted Message = AES(4-byte Checksum + Original Message)
    Data input = Hash::sha256(message);
    input.resize(4);
    input.insert(input.end(), message.begin(), message.end());

    encryptedMessage = aesEncrypt(input, encryptionKey, iv);
}

Data Memo::getSharedSecret(const PrivateKey& senderKey, const PublicKey& recipientKey) {
    Data dhKey(65);
    if (ecdh_multiply(&secp256k1, senderKey.bytes.data(), recipientKey.bytes.data(), dhKey.data()) != 0) {
        throw std::runtime_error("Could not derive a shared secret");
    }

    // return SHA512 of the X co-ordinate
    return Hash::sha512(dhKey.data() + 1, dhKey.data() + 33);
}

void Memo::serialize(Data& os) const noexcept {
    append(os, from.bytes);
    append(os, to.bytes);
    encode64LE(nonce, os);
    Bravo::encodeVarInt64(encryptedMessage.size(), os);
    append(os, encryptedMessage);
}

json Memo::serialize() const noexcept {
    json obj;
    obj["from"] = Bravo::Address(from, AddressPrefix).string();
    obj["to"] = Bravo::Address(to, AddressPrefix).string();
    obj["nonce"] = nonce;
    obj["message"] = hex(encryptedMessage);
    return obj;
}
