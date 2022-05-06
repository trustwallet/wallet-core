// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { WalletCore } from "../dist";

describe("CoinType", () => {
  it("test raw value", () => {
    const { CoinType } = WalletCore;

    assert.equal(CoinType.bitcoin.value, 0);
    assert.equal(CoinType.litecoin.value, 2);
    assert.equal(CoinType.dogecoin.value, 3);
    assert.equal(CoinType.ethereum.value, 60);
    assert.equal(CoinType.binance.value, 714);
    assert.equal(CoinType.cosmos.value, 118);
    assert.equal(CoinType.solana.value, 501);

  });
});
