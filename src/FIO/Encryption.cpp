// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Encryption.h"
#include "rand.h"

#include "../Base64.h"
#include "../Encrypt.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../Utils.h"
#include <TrustWalletCore/Generated/TWECDSA.h>
#include <TrustWalletCore/TWAESPaddingMode.h>


#include <cassert>

namespace TW::FIO {

using namespace std;

const uint8_t IvSize = 16;

Data Encryption::checkEncrypt(const Data& secret, const Data& message, Data& iv) {
    const Data K = Hash::sha512(secret);
    assert(K.size() == 64);
    const Data Ke = subData(K, 0, 32); // Encryption key
    const Data Km = subData(K, 32, 32); // MAC key
    if (iv.size() == 0) {
        // fill iv with strong random value
        iv = Data(IvSize);
        if (Random::random_buffer(iv, IvSize) != Random::OK) {
            throw std::runtime_error("Unable to generate random iv");
        }
    } else {
        if (iv.size() != IvSize) {
            throw std::invalid_argument("invalid IV size");
        }
    }
    assert(iv.size() == IvSize);
    Data ivOrig(0); // writeable copy
    TW::append(ivOrig, iv);

    // Encrypt. Padding is done (PKCS#7)
    const Data C = Encrypt::AESCBCEncrypt(Ke, message, iv, TWAESPaddingModePKCS7);

    // HMAC. Include in the HMAC input everything that impacts the decryption.
    Data hmacIn(0);
    TW::append(hmacIn, ivOrig);
    TW::append(hmacIn, C);
    Data M = Hash::hmac256(Km, hmacIn);

    Data result(0); // iv + C + M
    TW::append(result, hmacIn);
    TW::append(result, M);
    return result;
}

Data Encryption::checkDecrypt(const Data& secret, const Data& message) {
    const Data K = Hash::sha512(secret);
    assert(K.size() == 64);
    const Data Ke = subData(K, 0, 32); // Encryption key
    const Data Km = subData(K, 32, 32); // MAC key

    if (message.size() < IvSize + 16 + 32) {
        // minimum size: 16 for iv, 16 for message (padded), 32 for HMAC
        throw std::invalid_argument("Message too short");
    }
    Data iv = subData(message, 0, IvSize);
    const Data C = subData(message, IvSize, message.size() - IvSize - 32);
    const Data M = subData(message, message.size() - 32, 32);

    // Side-channel attack protection: First verify the HMAC, then and only then proceed to the decryption step
    Data hmacInput(0);
    TW::append(hmacInput, iv);
    TW::append(hmacInput, C);
    const Data Mc = Hash::hmac256(Km, hmacInput);
    if (M != Mc) {
        throw std::invalid_argument("Decrypt failed, HMAC mismatch");
    }

    // Decrypt, unpadding is done
    const Data unencrypted = Encrypt::AESCBCDecrypt(Ke, C, iv, TWAESPaddingModePKCS7);
    return unencrypted;
}

Data Encryption::getSharedSecret(const PrivateKey& privateKey1, const PublicKey& publicKey2) {
    // See https://github.com/fioprotocol/fiojs/blob/master/src/ecc/key_private.js
        
    auto privateKey = wrapTWData(TWDataCreateWithBytes(privateKey1.bytes.data(), privateKey1.bytes.size()));
    auto publicKey = wrapTWData(TWDataCreateWithBytes(publicKey2.bytes.data(), publicKey2.bytes.size()));
    auto sharedKey = wrapTWData(TWECDSASharedKey(privateKey.get(), publicKey.get(), true));
    if (sharedKey == nullptr) {
        throw std::invalid_argument("Invalid shared key");
    }
    return dataFromTWData(sharedKey);
}

Data Encryption::encrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& message, const Data& iv) {
    const Data sharedSecret = getSharedSecret(privateKey1, publicKey2);
    Data ivCopy(iv); // writeably copy
    return checkEncrypt(sharedSecret, message, ivCopy);
}

Data Encryption::decrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& encrypted) {
    const Data sharedSecret = getSharedSecret(privateKey1, publicKey2);
    return checkDecrypt(sharedSecret, encrypted);
}

string Encryption::encode(const Data& encrypted) {
    return TW::Base64::encode(encrypted);
}

Data Encryption::decode(const std::string& encoded) {
    return TW::Base64::decode(encoded);
}

} // namespace TW::FIO
