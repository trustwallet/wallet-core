// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { StoredKey } from "./wallet-core";

export namespace KeyStore {
  export enum WalletType {
    Mnemonic = "mnemonic",
    PrivateKey = "privateKey",
    WatchOnly = "watchOnly",
    Hardware = "hardware",
  }

  export enum Error {
    AccountNotFound = "account not found",
    InvalidPassword = "invalid password",
    InvalidMnemonic = "invalid mnemonic",
    InvalidJSON = "invalid JSON",
    InvalidKey = "invalid key",
  }

  export interface ActiveAccount {
    address: string;
    coin: number;
    publicKey: string;
    derivationPath: string;
    extendedPublicKey?: string;
  }

  export interface Wallet {
    id: string;

    type: WalletType;
    name: string;
    version: number;
    activeAccounts: ActiveAccount[];
  }

  export interface IKeyStore {
    save(wallet: Wallet): boolean;
    restore(id: string): Wallet | null;
  }

  // LocalStorage KeyStore
  export class LocalStorage implements IKeyStore {
    private readonly keyPrefix: string;
    private readonly storage: Storage;

    constructor(keyPrefix: string, storage: Storage) {
      this.keyPrefix = keyPrefix;
      this.storage = storage;
    }

    restore(id: string): Wallet | null {
      const json = this.storage.getItem(this.keyPrefix + id);
      if (json) {
        return JSON.parse(json);
      }
      return null;
    }

    save(wallet: Wallet): boolean {
      try {
        this.storage.setItem(
          this.keyPrefix + wallet.id,
          JSON.stringify(wallet)
        );
        return true;
      } catch (e) {
        return false;
      }
    }
  }
}
