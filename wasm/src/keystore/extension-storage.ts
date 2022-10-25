// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { Storage } from "webextension-polyfill";
import * as Types from "./types";

// Extension KeyStore
export class ExtensionStorage implements Types.IStorage {
  readonly storage: Storage.StorageArea;
  readonly walletIdsKey: string;

  constructor(walletIdsKey: string, storage: Storage.StorageArea) {
    this.walletIdsKey = walletIdsKey;
    this.storage = storage;
  }

  get(id: string): Promise<Types.Wallet> {
    return this.storage.get(id).then((object) => {
      let wallet = object[id];
      if (wallet === undefined) {
        throw Types.Error.WalletNotFound;
      }
      return wallet as Types.Wallet;
    });
  }

  set(id: string, wallet: Types.Wallet): Promise<void> {
    return this.getWalletIds().then((ids) => {
      if (ids.indexOf(id) === -1) {
        ids.push(id);
      }
      return this.storage.set({
        [id]: wallet,
        [this.walletIdsKey]: ids,
      });
    });
  }

  loadAll(): Promise<Types.Wallet[]> {
    return this.getWalletIds().then((ids) => {
      if (ids.length === 0) {
        return [];
      }
      return this.storage
        .get(ids)
        .then((wallets) => Object.keys(wallets).map((key) => wallets[key]));
    });
  }

  delete(id: string, password: string): Promise<void> {
    return this.getWalletIds().then((ids) => {
      let index = ids.indexOf(id);
      if (index === -1) {
        return;
      }
      ids.splice(index, 1);
      return this.storage
        .remove(id)
        .then(() => this.storage.set({ [this.walletIdsKey]: ids }));
    });
  }

  private getWalletIds(): Promise<string[]> {
    return this.storage.get(this.walletIdsKey).then((object) => {
      let ids = object[this.walletIdsKey] as string[];
      return ids === undefined ? [] : ids;
    });
  }
}
