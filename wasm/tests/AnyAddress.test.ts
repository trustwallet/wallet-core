// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("AnyAddress", () => {
  it("test validating Solana address", () => {
    const { AnyAddress, HexCoding, CoinType } = globalThis.core;

    var address = AnyAddress.createWithString(
      "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q",
      CoinType.solana
    );

    assert.equal(address.coin().value, 501);
    assert.equal(address.description(), "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q");

    const data = address.data();

    assert.equal(data.length, 32);
    assert.typeOf(data, "Uint8Array");
    assert.equal(HexCoding.encode(data), "0x66c2f508c9c555cacc9fb26d88e88dd54e210bb5a8bce5687f60d7e75c4cd07f");

    address.delete();
  }).timeout(5000);
});
