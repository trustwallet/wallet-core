// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import { assert } from "chai";
import { Buffer } from "buffer";

describe("Bech32", () => {

  it("test encode", () => {
    const { Bech32 } = globalThis.core;

    const encoded = Bech32.encode("abcdef", Buffer.from("00443214c74254b635cf84653a56d7c675be77df", "hex"));

    assert.equal(encoded, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");
  });

  it("test decode", () => {
    const { Bech32, HexCoding } = globalThis.core;

    const decoded = Bech32.decode("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw");

    assert.equal(
        HexCoding.encode(decoded),
        "0x00443214c74254b635cf84653a56d7c675be77df"
    );
  });

  it("test encodeM", () => {
    const { Bech32 } = globalThis.core;

    const encoded = Bech32.encodeM("abcdef", Buffer.from("ffbbcdeb38bdab49ca307b9ac5a928398a418820", "hex"));

    assert.equal(encoded, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
  });

  it("test decodeM", () => {
    const { Bech32, HexCoding } = globalThis.core;

    const decoded = Bech32.decodeM("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");

    assert.equal(
        HexCoding.encode(decoded),
        "0xffbbcdeb38bdab49ca307b9ac5a928398a418820"
    );
  });
});
