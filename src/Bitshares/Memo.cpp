#include "Memo.h"

#include <stdexcept>

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/aes.h>
#include <boost/lexical_cast.hpp>
#include <TrustWalletCore/TWPublicKeyType.h>

#include "../Hash.h"
#include "../HexCoding.h"
#include "../Bravo/Serialization.h"

#include "Address.h"

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

    // Encrypted Message = AES(4-byte Checksum + Original Message)
    Data input = Hash::sha256(message);
    input.resize(4);
    input.insert(input.end(), message.begin(), message.end());

    encryptedMessage = aesEncrypt(  input,
                                    encryptionKeyPlusIV.data(),
                                    encryptionKeyPlusIV.data() + 32);
}

Data Memo::getSharedSecret(const PrivateKey& senderKey, const PublicKey& recipientKey) {
    Data dhKey(65);
    if (ecdh_multiply(&secp256k1, senderKey.bytes.data(), recipientKey.bytes.data(), dhKey.data()) != 0) {
        throw std::runtime_error("Could not derive a shared secret");
    }

    // return SHA512 of the X co-ordinate
    return Hash::sha512(dhKey.data() + 1, dhKey.data() + 33);
}

Data TW::Bitshares::aesEncrypt(const uint8_t *message, size_t messageLength, const uint8_t *key, const uint8_t *initializationVector) {
    if (messageLength > static_cast<size_t>(INT_MAX)) {
        throw std::invalid_argument("Message size must be smaller than " + std::to_string(INT_MAX));
    }

    // create context
    aes_encrypt_ctx context;
    if (aes_encrypt_key256(key, &context) == EXIT_FAILURE) {
        throw std::runtime_error("Encryption error: Error initializing the key");
    }

    size_t fullBlockBytes = (messageLength / 16) * 16;        // no. of bytes that make up the full 16-byte blocks
    size_t remainingBytes = messageLength % 16;

    size_t outputSize = fullBlockBytes + (remainingBytes ? 16 : 0);
    Data output(outputSize);

    // create a non-const copy of the iv
    Data iv(initializationVector, initializationVector + 16);

    // encrypt the full blocks at a go
    if (fullBlockBytes) {
        if (aes_cbc_encrypt(message, output.data(), static_cast<int>(fullBlockBytes), iv.data(), &context) == EXIT_FAILURE) {
            throw std::runtime_error("Encryption error: Error encrypting the message");
        }
    }

    // create a __PKCS#5-padded__ buffer for the remaining bytes and encrypt that too
    if (remainingBytes) {
        Data lastBlock(16, static_cast<unsigned char>(16 - remainingBytes));
        std::memcpy(lastBlock.data(), message + fullBlockBytes, remainingBytes);
        if (aes_cbc_encrypt(lastBlock.data(), output.data() + fullBlockBytes, 16, iv.data(), &context) == EXIT_FAILURE) {
            throw std::runtime_error("Encryption error: Error encrypting the message");
        }
    }

    return output;
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
