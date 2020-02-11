// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

namespace TW::FIO {

/// Payload message encryption/decryption.
/// See also https://github.com/fioprotocol/fiojs/blob/master/src/encryption-check.ts
class Encryption {
public:
    /**
     * Provides AES-256-CBC encryption and message authentication.
     * The CBC cipher is used for good platform native compatability.
     * @see https://security.stackexchange.com/a/63134
     * @see https://security.stackexchange.com/a/20493
     * @arg {Buffer} secret - Shared secret (64-bytes).
     * @arg {Buffer} message - Plaintext message (arbitrary length).
     * @arg {Buffer} [iv = randomBytes(16)] - An unpredictable strong random value (16 bytes) is required
     *        and supplied by default. Unit tests may provide a static value to achieve predictable results.
     * @throws {Error} invalid IV size
     */
    static Data checkEncrypt(const Data& secret, const Data& message, Data& iv);

    /**
     * Provides AES-256-CBC message authentication then decryption.
     * @arg {Buffer} secret - Shared secret (64-bytes).
     * @arg {Buffer} message - Ciphertext (from checkEncrypt())
     * @throws {Error} Message too short
     * @throws {Error} Decrypt failed, HMAC mismatch
     */
    static Data checkDecrypt(const Data& secret, const Data& message);
};

} // namespace TW::FIO
