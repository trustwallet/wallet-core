// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"

namespace TW::FIO {

/// Payload message encryption/decryption.
/// \see https://github.com/fioprotocol/fiojs/blob/master/src/encryption-check.ts
class Encryption {
public:
    /**
     * Provides AES-256-CBC encryption and message authentication.
     * The CBC cipher is used for good platform native compatability.
     * \see https://security.stackexchange.com/a/63134
     * \see https://security.stackexchange.com/a/20493
     * \param secret - Shared secret (64-bytes).
     * \param message - Plaintext message (arbitrary length).
     * \param iv - An unpredictable strong random value (16 bytes) is required
     *        and supplied by default. Unit tests may provide a static value to achieve predictable results.
     * \throws {Error} invalid IV size
     */
    static Data checkEncrypt(const Data& secret, const Data& message, Data& iv);

    /**
     * Provides AES-256-CBC message authentication then decryption.
     * \param secret - Shared secret (64-bytes).
     * \param message - Ciphertext (from checkEncrypt())
     * \throws {Error} Message too short
     * \throws {Error} Decrypt failed, HMAC mismatch
     */
    static Data checkDecrypt(const Data& secret, const Data& message);

    /// Derive 64-byte shared secret from the private key and public key.
    static Data getSharedSecret(const PrivateKey& privateKey1, const PublicKey& publicKey2);

    /// Encrypt a message.  Own private key, recipient public key, and optional initial vector (may be empty) is needed.
    static Data encrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& message, const Data& iv);

    /// Decrypt a message.  Own private key and sender's public key is needed.
    static Data decrypt(const PrivateKey& privateKey1, const PublicKey& publicKey2, const Data& encrypted);

    /// Encode an encrypted message.  Base64 encoding is used.
    static std::string encode(const Data& encrypted);

    /// Decode an encrypted message.
    static Data decode(const std::string& encoded);
};

} // namespace TW::FIO
