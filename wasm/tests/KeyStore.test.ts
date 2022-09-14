// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import * as fs from "fs";
import { KeyStore, FileSystemStorage } from "../dist";

describe("KeyStore", async () => {
  it("test importing mnemonic", async () => {
    const { CoinType, HexCoding } = globalThis.core;

    const testDir = "/tmp/wasm-test";
    const password = "password";
    fs.mkdirSync(testDir, { recursive: true });

    const idPrefix = "all-wallet-ids";
    const storage = new FileSystemStorage(testDir);
    const keystore = new KeyStore.DefaultImpl(
      globalThis.core,
      idPrefix,
      storage
    );

    const mnemonic =
      "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ]);
    const stats = fs.statSync(storage.getFilename(wallet.id));

    assert.isTrue(stats.isFile());
    assert.isTrue(stats.size > 0);
    assert.equal(wallet.name, "Coolw");
    assert.equal(wallet.type, "mnemonic");
    assert.equal(wallet.version, 3);

    const account = wallet.activeAccounts[0];
    const key = await keystore.getKey(wallet.id, password, account);

    assert.equal(
      HexCoding.encode(key.data()),
      "0xd2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f"
    );
    assert.equal(account.address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    assert.equal(
      account.extendedPublicKey,
      "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn"
    );
    assert.equal(
      account.publicKey,
      "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006"
    );

    wallet = await keystore.addAccounts(wallet.id, "password", [
      CoinType.ethereum,
      CoinType.binance,
    ]);

    assert.equal(wallet.activeAccounts.length, 3);
    assert.isTrue(await keystore.hasWallet(wallet.id));
    assert.isFalse(await keystore.hasWallet("invalid-id"));

    const exported = await keystore.export(wallet.id, password);
    assert.equal(exported, mnemonic);

    const wallets = await keystore.loadAll();

    await wallets.forEach((w) => {
      keystore.delete(w.id, "password");
    });
  }).timeout(5000);
});
