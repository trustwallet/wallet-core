// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
