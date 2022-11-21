// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import "mocha";
import { assert } from "chai";
import { KeyStore } from "../dist";
import { ChromeStorageMock } from "./mock";

describe("KeyStore", async () => {
  it("test ExtensionStorage", async () => {
    const { CoinType, HexCoding, StoredKeyEncryption } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;

    const walletIdsKey = "all-wallet-ids";
    const storage = new KeyStore.ExtensionStorage(
      walletIdsKey,
      new ChromeStorageMock()
    );
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);

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

    wallet = await keystore.addAccounts(wallet.id, password, [
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
      keystore.delete(w.id, password);
    });
  }).timeout(10000);

  it("test ExtensionStorage AES256", async () => {
    const { CoinType, HexCoding, StoredKeyEncryption } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;

    const walletIdsKey = "all-wallet-ids";
    const storage = new KeyStore.ExtensionStorage(
        walletIdsKey,
        new ChromeStorageMock()
    );
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes256Ctr);

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

    wallet = await keystore.addAccounts(wallet.id, password, [
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
      keystore.delete(w.id, password);
    });
  }).timeout(10000);
});
