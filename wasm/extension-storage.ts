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
  private readonly walletIdsKey: string;

  constructor(core: WalletCore, prefix: string, storage: Storage.StorageArea) {
    this.core = core;
    this.prefix = prefix;
    this.storage = storage;
    this.walletIdsKey = prefix + "walletIds";
  }

  load(id: string): Promise<KeyStore.Wallet> {
    return this.storage.get(this.prefix + id).then((object) => {
      let wallet = object[this.prefix + id] as KeyStore.Wallet;
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
  ): Promise<KeyStore.Wallet> {
    const { Mnemonic, StoredKey, HDWallet } = this.core;
    return new Promise((resolve, reject) => {
      if (!Mnemonic.isValid(mnemonic)) {
        reject(KeyStore.Error.InvalidMnemonic);
        return;
      }
      let pass = Buffer.from(password);
      let storedKey = StoredKey.importHDWallet(mnemonic, name, pass, coins[0]);
      let hdWallet = HDWallet.createWithMnemonic(mnemonic, "");
      coins.forEach((coin) => {
        storedKey.accountForCoin(coin, hdWallet);
      });
      let wallet = this.mapWallet(storedKey);

      storedKey.delete();
      hdWallet.delete();
      this.importWallet(wallet)
        .then(() => {
          resolve(wallet);
        })
        .catch((error) => {
          reject(error);
        });
    });
  }

  importKey(
    key: Uint8Array,
    name: string,
    password: string,
    coin: CoinType
  ): Promise<KeyStore.Wallet> {
    const { StoredKey, PrivateKey, Curve } = this.core;
    return new Promise((resolve, reject) => {
      // FIXME: get curve from coin
      if (
        !PrivateKey.isValid(key, Curve.secp256k1) ||
        !PrivateKey.isValid(key, Curve.ed25519)
      ) {
        reject(KeyStore.Error.InvalidKey);
        return;
      }
      let pass = Buffer.from(password);
      let storedKey = StoredKey.importPrivateKey(key, name, pass, coin);
      let wallet = this.mapWallet(storedKey);
      storedKey.delete();
      this.importWallet(wallet)
        .then(() => {
          resolve(wallet);
        })
        .catch((error) => {
          reject(error);
        });
    });
  }

  importWallet(wallet: KeyStore.Wallet): Promise<void> {
    let object = {
      [this.prefix + wallet.id]: wallet,
    };
    return this.storage.set(object);
  }

  addAccounts(
    id: string,
    password: string,
    coins: CoinType[]
  ): Promise<KeyStore.Wallet> {
    return this.load(id).then((wallet) => {
      let storedKey = this.mapStoredKey(wallet);
      let hdWallet = storedKey.wallet(Buffer.from(password));
      coins.forEach((coin) => {
        storedKey.accountForCoin(coin, hdWallet);
      });
      let newWallet = this.mapWallet(storedKey);
      storedKey.delete();
      hdWallet.delete();
      return this.importWallet(newWallet).then(() => {
        return newWallet;
      });
    });
  }

  getKey(
    id: string,
    password: string,
    account: KeyStore.ActiveAccount
  ): Promise<PrivateKey> {
    return this.load(id).then((wallet) => {
      let storedKey = this.mapStoredKey(wallet);
      let hdWallet = storedKey.wallet(Buffer.from(password));
      let coin = this.core.CoinType[`${account.coin}`];
      let privateKey = hdWallet.getKey(coin, account.derivationPath);
      storedKey.delete();
      hdWallet.delete();
      return privateKey;
    });
  }

  delete(id: string, password: string): Promise<void> {
    return this.storage.remove(this.prefix + id);
  }

  export(id: string, password: string): Promise<string | Uint8Array> {
    return this.load(id).then((wallet) => {
      let storedKey = this.mapStoredKey(wallet);
      let value: string | Uint8Array;
      switch (wallet.type) {
        case KeyStore.WalletType.Mnemonic:
          value = storedKey.decryptMnemonic(Buffer.from(password));
        case KeyStore.WalletType.PrivateKey:
          value = storedKey.decryptPrivateKey(Buffer.from(password));
        default:
          value = "";
      }
      storedKey.delete();
      return value;
    });
  }

  private mapWallet(storedKey: StoredKey): KeyStore.Wallet {
    let json = storedKey.exportJSON();
    let wallet = JSON.parse(Buffer.from(json).toString()) as KeyStore.Wallet;
    return wallet;
  }

  private mapStoredKey(wallet: KeyStore.Wallet): StoredKey {
    let json = Buffer.from(JSON.stringify(wallet));
    let storedKey = this.core.StoredKey.importJSON(json);
    return storedKey;
  }
}
