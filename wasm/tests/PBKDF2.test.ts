// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";

describe("PBKDF2", () => {
  it("test sha256 hmac", () => {
    const { PBKDF2, HexCoding } = globalThis.core;

    const password = Buffer.from("password");
    const salt = Buffer.from("saltwithsufficientlength");
    
    const key = PBKDF2.hmacSha256(password, salt, 1, 20);
    const key2 = PBKDF2.hmacSha256(password, salt, 4096, 20);

    assert.equal(HexCoding.encode(key), "0x168ff47855244ffef87254a2c4ac9c954bc59c44");
    assert.equal(HexCoding.encode(key2), "0x77a724e0e92ec460a6f3b01a7b5eceb86abffc04");
  });
});
