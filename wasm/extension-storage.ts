// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { Storage } from "webextension-polyfill";
import { KeyStore } from "./keystore";

// Extension KeyStore
export class ExtensionStorage implements KeyStore.IKeyStoreStorage {
  readonly storage: Storage.LocalStorageArea;

  constructor(storage: Storage.LocalStorageArea) {
    this.storage = storage;
  }

  get(id: string): Promise<KeyStore.Wallet | null> {
    return this.storage.get(id).then((object) => {
      let wallet = object[id];
      if (wallet === undefined) {
        return null;
      }
      return wallet as KeyStore.Wallet;
    });
  }

  set(id: string, wallet: KeyStore.Wallet): Promise<void> {
    let object = {
      [id]: wallet,
    };
    return this.storage.set(object);
  }

  loadAll(walletIdsKey: string): Promise<KeyStore.Wallet[]> {
    return this.storage.get(walletIdsKey).then((walletIds) => {
      let ids = walletIds[walletIdsKey] as string[];
      return this.storage.get(ids).then((wallets) => {
        return Object.keys(wallets).map((key) => wallets[key]);
      });
    });
  }

  delete(id: string, password: string): Promise<void> {
    return this.storage.remove(id);
  }
}
