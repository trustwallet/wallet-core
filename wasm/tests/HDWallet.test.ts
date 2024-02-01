// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";

describe("HDWallet", () => {

  it("test creating 24 words", () => {
    const { HDWallet, Mnemonic } = globalThis.core;

    var wallet = HDWallet.create(256, "password");
    const mnemonic = wallet.mnemonic();

    assert.equal(mnemonic.split(" ").length, 24);
    assert.isTrue(Mnemonic.isValid(mnemonic));

    wallet.delete();
  });

  it("test deriving Ethereum address", () => {
    const { HDWallet, CoinType } = globalThis.core;

    var wallet = HDWallet.createWithMnemonic("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", "TREZOR");
    const address = wallet.getAddressForCoin(CoinType.ethereum);

    assert.equal(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");

    wallet.delete();
  });
});
