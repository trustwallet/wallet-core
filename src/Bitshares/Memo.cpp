#include "Memo.h"

#include <stdexcept>

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/aes.h>
#include <boost/lexical_cast.hpp>

#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW::Bitshares;
using Data = TW::Data;

Memo::Memo(const PrivateKey& senderKey, const PublicKey& recipientKey, const std::string& message)
        : to(recipientKey), from(senderKey.getPublicKey(PublicKeyType::secp256k1)) {
    if (recipientKey.type() != PublicKeyType::secp256k1) {
        throw std::invalid_argument("Recipient's public key is not a secp25k1 public key.");
    }

    if (message.empty()) {
        throw std::invalid_argument("Message is empty.");
    }

    random_buffer(static_cast<uint8_t *>(&nonce), sizeof(nonce));
    Data secret = getSharedSecret(senderKey, recipientKey);

    // The first 32 bytes will be used as encryption key and the next 32 as the IV
    Data encryptionKeyPlusIV = Hash::sha512(boost::lexical_cast<std::string>(nonce) + hex(secret));

    // Encrypted Message = AES(4-byte Checksum + Original Message)
    Data input = Hash::sha256(message);
    input.resize(4);
    input.insert(input.end, message.begin(), message.end());

    aesEncrypt( message,
                encryptionKeyPlusIV.data(),
                encryptionKeyPlusIV.data() + 32);
}

Data Memo::getSharedSecret(const PrivateKey& senderKey, const PublicKey& recipientKey) {
    Data dhKey(PublicKey::secp256k1Size);

    if (ecdh_multiply(&secp256k1, senderKey.bytes.data(), recipientKey.bytes.data(), dhKey.data()) != 0) {
        throw std::runtime_error("Could not derive a shared secret");
    }

    Data result(Hash::sha512Size);
    sha512_Raw(dhKey.data() + 1, dhKey.size() - 1, result.data());

    return result;
}

Data aesEncrypt(const uint8_t *message, size_t messageLength, const uint8_t *key, const uint8_t *initializationVector) {
    // need a non-const copy
    Data iv(initializationVector, initializationVector + 32);

    aes_encrypt_ctx context;

    if (aes_encrypt_key256(key, &context) == EXIT_FAILURE) {
        throw std::runtime_error("Encryption error: Error initializing the key");
    }

    size_t fullBlocksCount = messageLength / 16;
    size_t remainingBytes = messageLength % 16;

    size_t outputSize = (fullBlocksCount + (remainingBytes ? 1 : 0)) * 16;
    Data output(outputSize);

    // encrypt the full blocks at a go
    if (aes_cbc_encrypt(message, output.data(), fullBlocksCount * 16, iv.data(), &context) == EXIT_FAILURE) {
        throw std::runtime_error("Encryption error: Error encrypting the message");
    }

    // create a 0-padded buffer for the remaining bytes and encrypt that too
    if (remainingBytes) {
        Data lastBlock(message + (fullBlocksCount * 16), remainingBytes);
        std::memcpy(lastBlock.data(), message + (fullBlocksCount * 16), remainingBytes);
        (aes_cbc_encrypt(lastBlock.data(), output.data() + (fullBlocksCount * 16), 16, iv.data(), &context) == EXIT_FAILURE) {
            throw std::runtime_error("Encryption error: Error encrypting the message");
        }
    }

    return output;
}