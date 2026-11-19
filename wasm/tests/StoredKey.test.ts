// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

  // Note: the WASM build has C++ exception catching disabled, so failure paths
  // that rely on try/catch in C++ (wrong password, malformed JSON) abort the
  // module instead of returning null. This test uses a failure path that does not throw.
  it("test failing object-returning functions return null", () => {
    const { StoredKey, CoinType } = globalThis.core;

    const mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    const storedKey = StoredKey.importHDWallet(
      mnemonic,
      "test wallet",
      Buffer.from("password"),
      CoinType.bitcoin
    );
    assert.isNull(storedKey.account(99));

    storedKey.delete();
  }).timeout(5000);

  it("test accountForCoin accepts a null wallet", () => {
    const { StoredKey, CoinType } = globalThis.core;

    const mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    const storedKey = StoredKey.importHDWallet(
      mnemonic,
      "test wallet",
      Buffer.from("password"),
      CoinType.bitcoin
    );

    // Existing account is returned without needing a wallet.
    const account = storedKey.accountForCoin(CoinType.bitcoin, null);
    assert.equal(account.address(), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    account.delete();

    // No account and no wallet to derive one.
    assert.isNull(storedKey.accountForCoin(CoinType.ethereum, null));

    storedKey.delete();
  }).timeout(5000);
});
