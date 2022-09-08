// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { Storage } from "webextension-polyfill";
import { KeyStore } from "./keystore";
import { WalletCore, CoinType, PrivateKey, StoredKey } from "./wallet-core";

// Extension KeyStore
export class ExtensionStorage implements KeyStore.IKeyStore {
  private readonly core: WalletCore;
  private readonly prefix: string;
  private readonly storage: Storage.StorageArea;
  private readonly walletIdsKey = "wallet-ids";

  constructor(core: WalletCore, prefix: string, storage: Storage.StorageArea) {
    this.core = core;
    this.prefix = prefix;
    this.storage = storage;
  }

  load(id: string): Promise<KeyStore.Wallet> {
    return this.storage.get(this.prefix + id).then((value) => {
      let wallet = value as KeyStore.Wallet;
      return wallet;
    });
  }

  loadAll(): Promise<KeyStore.Wallet[]> {
    return this.storage.get(this.walletIdsKey).then((walletIds) => {
      let ids = walletIds[this.walletIdsKey] as string[];
      return this.storage.get(ids).then((wallets) => {
        return Object.keys(wallets).map((key) => wallets[key]);
      });
    });
  }

  import(
    mnemonic: string,
    name: string,
    password: string,
    coins: CoinType[]
  ): Promise<KeyStore.Wallet>;
  import(
    key: Uint8Array,
    name: string,
    password: string,
    coin: CoinType
  ): Promise<KeyStore.Wallet>;
  import(wallet: KeyStore.Wallet): Promise<boolean>;
  import(
    key: unknown,
    name?: unknown,
    password?: unknown,
    coin?: unknown
  ): Promise<KeyStore.Wallet> | Promise<boolean> {
    throw new Error("Method not implemented.");
  }
  addAccounts(
    id: string,
    password: string,
    coins: CoinType[]
  ): Promise<KeyStore.Wallet> {
    throw new Error("Method not implemented.");
  }
  getKey(
    id: string,
    password: string,
    account: KeyStore.ActiveAccount
  ): Promise<PrivateKey> {
    throw new Error("Method not implemented.");
  }
  delete(id: string, password: string): Promise<boolean>;
  delete(id: string, password: string): Promise<boolean>;
  delete(id: unknown, password: unknown): Promise<boolean> {
    throw new Error("Method not implemented.");
  }
  export(id: string, password: string): Promise<string | Uint8Array> {
    throw new Error("Method not implemented.");
  }
}
