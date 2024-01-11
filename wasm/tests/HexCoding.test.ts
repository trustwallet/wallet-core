// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("HexCoding", () => {
  it("test encoding / decoding hex string", () => {
    const { HexCoding } = globalThis.core;

    const expected = new Uint8Array([0x52, 0x8]);
    const decoded = HexCoding.decode("0x5208");

    assert.deepEqual(decoded, expected);

    const encoded = HexCoding.encode(expected);

    assert.equal(encoded, "0x5208");
  });
});
