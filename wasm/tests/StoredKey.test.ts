// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";

describe("StoredKey", () => {
  it("test importing mnemonic", () => {
    const { StoredKey, CoinType } = globalThis.core;

    const mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    const password = Buffer.from("password");
    const storedKey = StoredKey.importHDWallet(
      mnemonic,
      "test wallet",
      password,
      CoinType.bitcoin
    );

    assert.isTrue(storedKey.isMnemonic());

    const jsonData = storedKey.exportJSON();
    const json = JSON.parse(Buffer.from(jsonData).toString());
    
    assert.equal(json.type, "mnemonic");
    assert.equal(json.name, "test wallet");

    const account = json.activeAccounts[0];

    assert.equal(account.address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    assert.equal(account.extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
    assert.equal(account.publicKey, "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006");

    storedKey.delete();
  }).timeout(5000);
});
