// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("HRP", () => {
  it("test string value", () => {
    const { HRP, describeHRP } = globalThis.core;

    assert.equal(describeHRP(HRP.bitcoin), "bc");
    assert.equal(describeHRP(HRP.binance), "bnb");

  });
});
