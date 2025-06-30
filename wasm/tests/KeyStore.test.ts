// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { KeyStore } from "../dist";

describe("KeyStore", () => {
  it("test get key default", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding, Derivation } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;

    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);

    const account = wallet.activeAccounts[0];
    const key = await keystore.getKey(wallet.id, password, account);

    assert.equal(
      HexCoding.encode(key.data()),
      "0xd2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f"
    );

    const inputPrivateKey = Buffer.from("9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c", "hex");
    wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.bitcoin, StoredKeyEncryption.aes128Ctr, Derivation.default);

    const account2 = wallet.activeAccounts[0];
    assert.equal(account2.derivationPath, "m/84'/0'/0'/0/0");
    assert.equal(account2.address, "bc1qygled22ne2atsrdhhg92xaqflk67tpc4q69kjd");
    const key2 = await keystore.getKey(wallet.id, password, account2);

    assert.equal(
      HexCoding.encode(key2.data()),
      "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c"
    );
  }).timeout(10000);

  it("test get key legacy", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding, Derivation } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;

    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);

    const account = wallet.activeAccounts[0];
    const key = await keystore.getKey(wallet.id, password, account);

    assert.equal(
      HexCoding.encode(key.data()),
      "0xd2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f"
    );

    const inputPrivateKey = Buffer.from("9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c", "hex");
    wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.bitcoin, StoredKeyEncryption.aes128Ctr, Derivation.bitcoinLegacy);

    const account2 = wallet.activeAccounts[0];
    assert.equal(account2.derivationPath, "m/44'/0'/0'/0/0");
    assert.equal(account2.address, "14869jMDBsEra11WyervLDxCQD4GMBfjD7");
    const key2 = await keystore.getKey(wallet.id, password, account2);

    assert.equal(
      HexCoding.encode(key2.data()),
      "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c"
    );
  }).timeout(10000);

  it("test export", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding, Derivation } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;
    
    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);
    
    const exported = await keystore.export(wallet.id, password);
    assert.equal(exported, mnemonic);

    const inputPrivateKey = Buffer.from("9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c", "hex");
    wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.bitcoin, StoredKeyEncryption.aes128Ctr, Derivation.default);

    const exported2 = await keystore.export(wallet.id, password);
    assert.equal(HexCoding.encode(exported2), "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c");
  }).timeout(10000);

  it("test export mnemonic", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding } = globalThis.core;
    const mnemonic = globalThis.mnemonic as string;
    const password = globalThis.password as string;
    
    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    var wallet = await keystore.import(mnemonic, "Coolw", password, [
      CoinType.bitcoin,
    ], StoredKeyEncryption.aes128Ctr);
    assert.equal(await keystore.getWalletType(wallet.id), "mnemonic");
    
    const exported = await keystore.exportMnemonic(wallet.id, password);
    assert.equal(exported, mnemonic);
  }).timeout(10000);

  it("test export key", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding, Derivation } = globalThis.core;
    const password = globalThis.password as string;
    
    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    const inputPrivateKey = Buffer.from("9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c", "hex");
    const wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.solana, StoredKeyEncryption.aes128Ctr, Derivation.default);
    assert.equal(await keystore.getWalletType(wallet.id), "private-key");

    const exported = await keystore.exportPrivateKey(wallet.id, password);
    assert.equal(HexCoding.encode(exported), "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c");
  }).timeout(10000);

  it("test export key encoded", async () => {
    const { CoinType, StoredKeyEncryption, Derivation } = globalThis.core;
    const password = globalThis.password as string;
    
    const storage = new KeyStore.FileSystemStorage("/tmp");
    const keystore = new KeyStore.Default(globalThis.core, storage);

    const inputPrivateKeyBase58 = "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr";
    const wallet = await keystore.importKeyEncoded(inputPrivateKeyBase58, "Coolw", password, CoinType.solana, StoredKeyEncryption.aes128Ctr, Derivation.default);
    assert.equal(await keystore.getWalletType(wallet.id), "private-key");

    const exported = await keystore.exportPrivateKeyEncoded(wallet.id, password);
    assert.equal(exported, inputPrivateKeyBase58);
  }).timeout(10000);
});
