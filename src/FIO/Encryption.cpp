// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Encryption.h"

#include "../memory/memzero_wrapper.h"
#include "../Base64.h"
#include "../Encrypt.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrustWalletCore/TWAESPaddingMode.h>

#include <cassert>

namespace TW::FIO {

using namespace std;

const uint8_t IvSize = 16;

Data Encryption::checkEncrypt(const Data& secret, const Data& message, Data& iv) {
    if (iv.empty()) {
        // fill iv with strong random value
        iv = Data(IvSize);
        random_buffer(iv.data(), iv.size());
    } else {
        if (iv.size() != IvSize) {
            throw std::invalid_argument("invalid IV size");
        }
    }
    assert(iv.size() == IvSize);
    Data ivOrig(0); // writeable copy
    TW::append(ivOrig, iv);

    Data K = Hash::sha512(secret);
    assert(K.size() == 64);
    Data Ke = subData(K, 0, 32); // Encryption key
    Data Km = subData(K, 32, 32); // MAC key
    memzero(K.data(), K.size());

    // Encrypt. Padding is done (PKCS#7)
    Data C;
    try {
        C = Encrypt::AESCBCEncrypt(Ke, message, iv, TWAESPaddingModePKCS7);
    } catch (...) {
        memzero(Ke.data(), Ke.size());
        memzero(Km.data(), Km.size());
        throw;
    }
    memzero(Ke.data(), Ke.size());

    // HMAC. Include in the HMAC input everything that impacts the decryption.
    Data hmacIn(0);
    TW::append(hmacIn, ivOrig);
    TW::append(hmacIn, C);
    Data M = Hash::hmac256(Km, hmacIn);
    memzero(Km.data(), Km.size());

    Data result(0); // iv + C + M
    TW::append(result, hmacIn);
    TW::append(result, M);
    return result;
}

Data Encryption::checkDecrypt(const Data& secret, const Data& message) {
    if (message.size() < IvSize + 16 + 32) {
        // minimum size: 16 for iv, 16 for message (padded), 32 for HMAC
        throw std::invalid_argument("Message too short");
    }
    Data iv = subData(message, 0, IvSize);
    const Data C = subData(message, IvSize, message.size() - IvSize - 32);
    const Data M = subData(message, message.size() - 32, 32);

    Data K = Hash::sha512(secret);
    assert(K.size() == 64);
    Data Ke = subData(K, 0, 32); // Encryption key
    Data Km = subData(K, 32, 32); // MAC key
    memzero(K.data(), K.size());

    // Side-channel attack protection: First verify the HMAC, then and only then proceed to the decryption step
    Data hmacInput(0);
    TW::append(hmacInput, iv);
    TW::append(hmacInput, C);
    const Data Mc = Hash::hmac256(Km, hmacInput);
    memzero(Km.data(), Km.size());
    if (!isEqualConstantTime(M, Mc)) {
        memzero(Ke.data(), Ke.size());
        throw std::invalid_argument("Decrypt failed, HMAC mismatch");
    }

    // Decrypt, unpadding is done
    Data unencrypted;
    try {
        unencrypted = Encrypt::AESCBCDecrypt(Ke, C, iv, TWAESPaddingModePKCS7);
    } catch (...) {
        memzero(Ke.data(), Ke.size());
        throw;
    }
    memzero(Ke.data(), Ke.size());
    return unencrypted;
}

Data Encryption::getSharedSecret(const PrivateKey& privateKey1, const PublicKey& publicKey2) {
    // See https://github.com/fioprotocol/fiojs/blob/master/src/ecc/key_private.js

    if (publicKey2.type != TWPublicKeyTypeSECP256k1 && publicKey2.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Unsupported public key type");
    }
    
    curve_point KBP;
    int read_res = ecdsa_read_pubkey(&secp256k1, publicKey2.bytes.data(), &KBP);
    if (read_res == 0) {
        throw std::invalid_argument("Invalid public key");
    }

    bignum256 privBN {};
    bn_read_be(privateKey1.bytes.data(), &privBN);
    
    curve_point P {};
    if (point_multiply(&secp256k1, &privBN, &KBP, &P) != 0) {
        memzero(&privBN, sizeof(privBN));
        throw std::invalid_argument("Invalid private key: scalar out of range");
    }
    memzero(&privBN, sizeof(privBN));

    Data S(32, 0);
    bn_write_be(&P.x, S.data());
    memzero(&P, sizeof(P));

    // SHA512 used in ECIES
    Data result = Hash::sha512(S);
    memzero(S.data(), S.size());
    return result;
}

Data Encryption::encrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& message, const Data& iv) {
    Data sharedSecret = getSharedSecret(privateKey1, publicKey2);
    Data ivCopy(iv); // writeably copy
    const auto result = checkEncrypt(sharedSecret, message, ivCopy);
    memzero(sharedSecret.data(), sharedSecret.size());
    return result;
}

Data Encryption::decrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& encrypted) {
    Data sharedSecret = getSharedSecret(privateKey1, publicKey2);
    const auto result = checkDecrypt(sharedSecret, encrypted);
    memzero(sharedSecret.data(), sharedSecret.size());
    return result;
}

string Encryption::encode(const Data& encrypted) {
    return TW::Base64::encode(encrypted);
}

Data Encryption::decode(const std::string& encoded) {
    return TW::Base64::decode(encoded);
}

} // namespace TW::FIO
