// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import { assert } from "chai";
import { Buffer } from "buffer";

describe("Bech32", () => {

  it("test decoding", () => {
    const { Bech32, HexCoding } = globalThis.core;

    const decoded = Bech32.decode("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");

    assert.equal(
        HexCoding.encode(decoded),
        "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
    );
  });

  it("test encoding", () => {
    const { Bech32 } = globalThis.core;

    const encoded = Bech32.encode("abcdef", Buffer.from("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f", "hex"));

    assert.equal(encoded, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
  });
});
