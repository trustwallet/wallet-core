// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("CoinTypeConfiguration", () => {
  it("test getNativeTokenName differs from getName for SmartChain", () => {
    const { CoinType, CoinTypeConfiguration } = globalThis.core;

    const name = CoinTypeConfiguration.getName(CoinType.smartChain);
    const nativeTokenName = CoinTypeConfiguration.getNativeTokenName(CoinType.smartChain);

    assert.equal(name, "BNB Smart Chain");
    assert.equal(nativeTokenName, "BNB");
    assert.notEqual(name, nativeTokenName);
  });

  it("test getNativeTokenName matches getName for Bitcoin", () => {
    const { CoinType, CoinTypeConfiguration } = globalThis.core;

    const name = CoinTypeConfiguration.getName(CoinType.bitcoin);
    const nativeTokenName = CoinTypeConfiguration.getNativeTokenName(CoinType.bitcoin);

    assert.equal(name, nativeTokenName);
  });
});
