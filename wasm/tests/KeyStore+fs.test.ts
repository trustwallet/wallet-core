// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import * as fs from "fs";
import { KeyStore } from "../dist";

describe("KeyStore", async () => {
  it("test FileSystemStorage", async () => {
    const { CoinType, HexCoding, StoredKeyEncryption } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;
    const testDir = "/tmp/wasm-test";

    fs.mkdirSync(testDir, { recursive: true });

    const storage = new KeyStore.FileSystemStorage(testDir);
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);
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

  it("test fix scrypt with empty salt", async () => {
    const { StoredKey } = globalThis.core;
    const password = globalThis.password as string;
    const gMnemonic = globalThis.mnemonic as string;
    const walletID = "8e334366-020b-493f-81ab-a946432f536d";
    const testDir = "/tmp/wasm-test";

    fs.mkdirSync(testDir, { recursive: true });

    const scryptEmptySaltWallet = {
      activeAccounts: [{
        address: "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny",
        coin: 0,
        derivationPath: "m/84'/0'/0'/0/0",
        extendedPublicKey: "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn",
        publicKey: "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006",
      }],
      crypto: {
        cipher: "aes-128-ctr",
        cipherparams: { iv: "f375d3903fa00839c43109b1d26436b7" },
        ciphertext: "9768cdec22c3b0d5d7eced4e5387c138045367b9481d5cf017d262e645accd478f0f197f6dcb97e2ce9105fee0e7074d891a9826df3bc8f4dca17f5cdfb9992b86e40f26028c5a392cb2de17",
        kdf: "scrypt",
        kdfparams: { dklen: 32, n: 16384, p: 4, r: 8, salt: "" },
        mac: "bbdba986c713d91828a7a2f031d3535414967fce81c6c826b50b0cfab8783dfc",
      },
      id: walletID,
      name: "name",
      type: "mnemonic",
      version: 3,
    };

    const storage = new KeyStore.FileSystemStorage(testDir);
    const keystore = new KeyStore.Default(globalThis.core, storage);

    await keystore.importWallet(scryptEmptySaltWallet as any);

    // Confirm the file was written to disk.
    const stats = fs.statSync(storage.getFilename(walletID));
    assert.isTrue(stats.isFile());

    const wallet = await keystore.load(walletID);
    const storedKey = StoredKey.importJSON(Buffer.from(JSON.stringify(wallet)));

    const jsonBefore = JSON.parse(Buffer.from(storedKey.exportJSON()).toString());
    const saltBefore = jsonBefore.crypto.kdfparams.salt;
    const ciphertextBefore = jsonBefore.crypto.ciphertext;
    assert.equal(saltBefore, "");

    assert.isTrue(storedKey.fixEncryption(Buffer.from(password)));

    const updatedWallet = JSON.parse(Buffer.from(storedKey.exportJSON()).toString());
    storedKey.delete();
    await keystore.importWallet(updatedWallet);

    // Reload from disk to confirm the updated JSON was persisted.
    const reloadedWallet = await keystore.load(walletID);
    const reloadedKey = StoredKey.importJSON(Buffer.from(JSON.stringify(reloadedWallet)));

    assert.equal(reloadedWallet.id, walletID);

    const jsonAfter = JSON.parse(Buffer.from(reloadedKey.exportJSON()).toString());
    assert.isAbove(jsonAfter.crypto.kdfparams.salt.length, 0);
    assert.notEqual(jsonAfter.crypto.ciphertext, ciphertextBefore);
    assert.equal(reloadedKey.decryptMnemonic(Buffer.from(password)), gMnemonic);

    reloadedKey.delete();
    await keystore.delete(walletID, password);
  }).timeout(15000);

  it("test FileSystemStorage AES256", async () => {
    const { CoinType, HexCoding, StoredKeyEncryption } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;
    const testDir = "/tmp/wasm-test";

    fs.mkdirSync(testDir, { recursive: true });

    const storage = new KeyStore.FileSystemStorage(testDir);
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes256Ctr);
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
