// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import { assert } from "chai";
import { Buffer } from "buffer";

describe("Bech32m", () => {

  it("test decoding", () => {
    const { Bech32m, HexCoding } = globalThis.core;

    const decoded = Bech32m.decode("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");

    assert.equal(
        HexCoding.encode(decoded),
        "0x1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100"
    );
  });

  it("test encoding", () => {
    const { Bech32m } = globalThis.core;

    const encoded = Bech32m.encode("abcdef", Buffer.from("1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100", "hex"));

    assert.equal(encoded, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx");
  });
});
