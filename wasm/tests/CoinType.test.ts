// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";

describe("CoinType", () => {
  it("test raw value", () => {
    const { CoinType, CoinTypeExtension, Blockchain, Purpose, Curve } = globalThis.core;

    assert.equal(CoinType.bitcoin.value, 0);
    assert.equal(CoinType.litecoin.value, 2);
    assert.equal(CoinType.dogecoin.value, 3);
    assert.equal(CoinType.ethereum.value, 60);
    assert.equal(CoinType.binance.value, 714);
    assert.equal(CoinType.cosmos.value, 118);
    assert.equal(CoinType.solana.value, 501);
    let val = new CoinTypeExtension(CoinType.solana);
    assert.equal(val.blockchain(), Blockchain.solana);
    assert.equal(val.value(), CoinType.solana);
    assert.equal(val.purpose(), Purpose.bip44);
    assert.equal(val.curve(), Curve.ed25519);
    assert.isTrue(val.validate("Bxp8yhH9zNwxyE4UqxP7a7hgJ5xTZfxNNft7YJJ2VRjT"))
    assert.equal(val.derivationPath(), "m/44'/501'/0'");
  });
});
