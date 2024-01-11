// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("BitcoinSigHashType", () => {
  it("test isSingle", () => {
    const { BitcoinSigHashType, BitcoinSigHashTypeExt } = globalThis.core;

    assert.isFalse(BitcoinSigHashTypeExt.isSingle(BitcoinSigHashType.all));
    assert.isFalse(BitcoinSigHashTypeExt.isSingle(BitcoinSigHashType.none));
    assert.isTrue(BitcoinSigHashTypeExt.isSingle(BitcoinSigHashType.single));
    assert.isFalse(BitcoinSigHashTypeExt.isSingle(BitcoinSigHashType.fork));
    assert.isFalse(BitcoinSigHashTypeExt.isSingle(BitcoinSigHashType.forkBTG));
  });

  it("test isNone", () => {
    const { BitcoinSigHashType, BitcoinSigHashTypeExt } = globalThis.core;

    assert.isFalse(BitcoinSigHashTypeExt.isNone(BitcoinSigHashType.all));
    assert.isTrue(BitcoinSigHashTypeExt.isNone(BitcoinSigHashType.none));
    assert.isFalse(BitcoinSigHashTypeExt.isNone(BitcoinSigHashType.single));
    assert.isFalse(BitcoinSigHashTypeExt.isNone(BitcoinSigHashType.fork));
    assert.isFalse(BitcoinSigHashTypeExt.isNone(BitcoinSigHashType.forkBTG));
  });
});
