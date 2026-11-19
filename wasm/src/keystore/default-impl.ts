// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import {WalletCore, CoinType, Derivation, PrivateKey, StoredKey, StoredKeyEncryption} from "../wallet-core";
import * as Types from "./types";

// Wallet Core returns null when a `_Nullable` call fails.
function orThrow<T>(value: T | null, error: Types.Error): T {
  if (value === null) {
    throw error;
  }
  return value;
}

export class Default implements Types.IKeyStore {
  private readonly core: WalletCore;
  private readonly storage: Types.IStorage;

  constructor(core: WalletCore, storage: Types.IStorage) {
    this.core = core;
    this.storage = storage;
  }

  hasWallet(id: string): Promise<boolean> {
    return this.storage
      .get(id)
      .then((wallet) => true)
      .catch((error) => false);
  }

  load(id: string): Promise<Types.Wallet> {
    return this.storage.get(id);
  }

  loadAll(): Promise<Types.Wallet[]> {
    return this.storage.loadAll();
  }

  delete(id: string, password: string): Promise<void> {
    return this.storage.delete(id, password);
  }

  mapWallet(storedKey: StoredKey): Types.Wallet {
    const json = orThrow(storedKey.exportJSON(), Types.Error.InvalidJSON);
    return JSON.parse(Buffer.from(json).toString()) as Types.Wallet;
  }

  mapStoredKey(wallet: Types.Wallet): StoredKey {
    const json = Buffer.from(JSON.stringify(wallet));
    return orThrow(this.core.StoredKey.importJSON(json), Types.Error.InvalidJSON);
  }

  // Runs `fn` on a StoredKey parsed from `wallet`, deleting the key even if `fn` throws.
  private withStoredKey<T>(wallet: Types.Wallet, fn: (storedKey: StoredKey) => T): T {
    const storedKey = this.mapStoredKey(wallet);
    try {
      return fn(storedKey);
    } finally {
      storedKey.delete();
    }
  }

  // Decrypts a secret from `wallet`; a null result means a wrong password.
  private decrypt<T>(wallet: Types.Wallet, fn: (storedKey: StoredKey) => T | null): T {
    return this.withStoredKey(wallet, (storedKey) => orThrow(fn(storedKey), Types.Error.InvalidPassword));
  }

  importWallet(wallet: Types.Wallet): Promise<void> {
    return this.storage.set(wallet.id, wallet);
  }

  import(
    mnemonic: string,
    name: string,
    password: string,
    coins: CoinType[],
    encryption: StoredKeyEncryption
  ): Promise<Types.Wallet> {
    return new Promise((resolve, reject) => {
      const { Mnemonic, StoredKey, HDWallet, StoredKeyEncryption } = this.core;

      if (!Mnemonic.isValid(mnemonic)) {
        throw Types.Error.InvalidMnemonic;
      }
      let pass = Buffer.from(password);
      const hdWallet = orThrow(HDWallet.createWithMnemonic(mnemonic, ""), Types.Error.InvalidMnemonic);
      let wallet: Types.Wallet;
      try {
        const storedKey = orThrow(
          StoredKey.importHDWalletWithEncryption(mnemonic, name, pass, coins[0], encryption),
          Types.Error.InvalidMnemonic
        );
        try {
          coins.forEach((coin) => {
            storedKey.accountForCoin(coin, hdWallet)?.delete();
          });
          wallet = this.mapWallet(storedKey);
        } finally {
          storedKey.delete();
        }
      } finally {
        hdWallet.delete();
      }

      this.importWallet(wallet)
        .then(() => resolve(wallet))
        .catch((error) => reject(error));
    });
  }

  importKey(
    key: Uint8Array,
    name: string,
    password: string,
    coin: CoinType,
    encryption: StoredKeyEncryption,
    derivation: Derivation
  ): Promise<Types.Wallet> {
    return new Promise((resolve, reject) => {
      const { StoredKey, PrivateKey, Curve, StoredKeyEncryption } = this.core;

      // FIXME: get curve from coin
      if (
        !PrivateKey.isValid(key, Curve.secp256k1) ||
        !PrivateKey.isValid(key, Curve.ed25519)
      ) {
        throw Types.Error.InvalidKey;
      }
      let pass = Buffer.from(password);
      const storedKey = orThrow(
        StoredKey.importPrivateKeyWithEncryptionAndDerivation(key, name, pass, coin, encryption, derivation),
        Types.Error.InvalidKey
      );
      let wallet: Types.Wallet;
      try {
        wallet = this.mapWallet(storedKey);
      } finally {
        storedKey.delete();
      }
      this.importWallet(wallet)
        .then(() => resolve(wallet))
        .catch((error) => reject(error));
    });
  }

  importKeyEncoded(
    key: string,
    name: string,
    password: string,
    coin: CoinType,
    encryption: StoredKeyEncryption,
    derivation: Derivation
  ): Promise<Types.Wallet> {
    return new Promise((resolve, reject) => {
      const { StoredKey, PrivateKey, Curve, StoredKeyEncryption } = this.core;

      let pass = Buffer.from(password);
      const storedKey = orThrow(
        StoredKey.importPrivateKeyEncodedWithEncryptionAndDerivation(key, name, pass, coin, encryption, derivation),
        Types.Error.InvalidKey
      );
      let wallet: Types.Wallet;
      try {
        wallet = this.mapWallet(storedKey);
      } finally {
        storedKey.delete();
      }
      this.importWallet(wallet)
        .then(() => resolve(wallet))
        .catch((error) => reject(error));
    });
  }

  addAccounts(
    id: string,
    password: string,
    coins: CoinType[]
  ): Promise<Types.Wallet> {
    const { Derivation } = this.core;

    let coins_with_derivations = coins.map(coin => ({
      coin: coin,
      derivation: Derivation.default,
    }));
    return this.addAccountsWithDerivations(id, password, coins_with_derivations);
  }

  addAccountsWithDerivations(id: string, password: string, coins: Types.CoinWithDerivation[]): Promise<Types.Wallet> {
    return this.load(id).then((wallet) => {
      const newWallet = this.withStoredKey(wallet, (storedKey) => {
        const hdWallet = orThrow(storedKey.wallet(Buffer.from(password)), Types.Error.InvalidPassword);
        try {
          coins.forEach((item) => {
            storedKey.accountForCoinDerivation(item.coin, item.derivation, hdWallet)?.delete();
          });
          return this.mapWallet(storedKey);
        } finally {
          hdWallet.delete();
        }
      });
      return this.importWallet(newWallet).then(() => newWallet);
    });
  }

  getKey(
    id: string,
    password: string,
    account: Types.ActiveAccount
  ): Promise<PrivateKey> {
    return this.load(id).then((wallet) => {
      let coin = (this.core.CoinType as any).values["" + account.coin];
      return this.withStoredKey(wallet, (storedKey) =>
        orThrow(storedKey.privateKey(coin, Buffer.from(password)), Types.Error.InvalidPassword)
      );
    });
  }

  export(id: string, password: string): Promise<string | Uint8Array> {
    return this.load(id).then((wallet) => {
      switch (wallet.type) {
        case Types.WalletType.Mnemonic:
          return this.decrypt(wallet, (storedKey) => storedKey.decryptMnemonic(Buffer.from(password)));
        case Types.WalletType.PrivateKey:
          return this.decrypt(wallet, (storedKey) => storedKey.decryptPrivateKey(Buffer.from(password)));
        default:
          throw Types.Error.InvalidJSON;
      }
    });
  }

  getWalletType(id: string): Promise<Types.WalletType> {
    return this.load(id).then((wallet) => wallet.type);
  }

  exportMnemonic(id: string, password: string): Promise<string> {
    return this.load(id).then((wallet) => {
      if (wallet.type !== Types.WalletType.Mnemonic) {
        throw Types.Error.UnsupportedWalletType;
      }
      return this.decrypt(wallet, (storedKey) => storedKey.decryptMnemonic(Buffer.from(password)));
    });
  }

  exportPrivateKey(id: string, password: string): Promise<Uint8Array> {
    return this.load(id).then((wallet) => {
      if (wallet.type !== Types.WalletType.PrivateKey) {
        throw Types.Error.UnsupportedWalletType;
      }
      return this.decrypt(wallet, (storedKey) => storedKey.decryptPrivateKey(Buffer.from(password)));
    });
  }

  exportPrivateKeyEncoded(id: string, password: string): Promise<string> {
    return this.load(id).then((wallet) => {
      if (wallet.type !== Types.WalletType.PrivateKey) {
        throw Types.Error.UnsupportedWalletType;
      }
      return this.decrypt(wallet, (storedKey) => storedKey.decryptPrivateKeyEncoded(Buffer.from(password)));
    });
  }
}
