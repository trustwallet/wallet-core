// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import "mocha";
import { assert } from "chai";
import { Buffer } from "buffer";
import { KeyStore } from "../dist";

describe("KeyStore", () => {
  it("test get key", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding } = globalThis.core;
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
    wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.bitcoin, StoredKeyEncryption.aes128Ctr);

    const account2 = wallet.activeAccounts[0];
    const key2 = await keystore.getKey(wallet.id, password, account2);

    assert.equal(
      HexCoding.encode(key2.data()),
      "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c"
    );
  }).timeout(10000);

  it("test export", async () => {
    const { CoinType, StoredKeyEncryption, HexCoding } = globalThis.core;
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
    wallet = await keystore.importKey(inputPrivateKey, "Coolw", password, CoinType.bitcoin, StoredKeyEncryption.aes128Ctr);

    const exported2 = await keystore.export(wallet.id, password);
    assert.equal(HexCoding.encode(exported2), "0x9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c");
  }).timeout(10000);
});
