// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { WalletCore, CoinType, PrivateKey, StoredKey } from "./wallet-core";

export namespace KeyStore {
  export enum WalletType {
    Mnemonic = "mnemonic",
    PrivateKey = "privateKey",
    WatchOnly = "watchOnly",
    Hardware = "hardware",
  }

  export enum Error {
    WalletNotFound = "wallet not found",
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
    // Check if wallet id exists
    hasWallet(id: string): Promise<boolean>;

    // Load a wallet by wallet id
    load(id: string): Promise<Wallet>;

    // Load all wallets
    loadAll(): Promise<Wallet[]>;

    // Import a wallet by mnemonic, name, password and initial active accounts (from coinTypes)
    import(
      mnemonic: string,
      name: string,
      password: string,
      coins: CoinType[]
    ): Promise<Wallet>;

    // Import a wallet by private key, name and password
    importKey(
      key: Uint8Array,
      name: string,
      password: string,
      coin: CoinType
    ): Promise<Wallet>;

    // Import a Wallet object directly
    importWallet(wallet: Wallet): Promise<void>;

    // Add active accounts to a wallet by wallet id, password, coin
    addAccounts(
      id: string,
      password: string,
      coins: CoinType[]
    ): Promise<Wallet>;

    // Get private key of an account by wallet id, password, coin and derivation path
    getKey(
      id: string,
      password: string,
      account: ActiveAccount
    ): Promise<PrivateKey>;

    // Delete a wallet by wallet id and password.
    delete(id: string, password: string): Promise<void>;

    // Export a wallet by wallet id and password, returns mnemonic or private key
    export(id: string, password: string): Promise<string | Uint8Array>;
  }

  export interface IKeyStoreStorage {
    get(id: string): Promise<Wallet | null>;
    set(id: string, wallet: Wallet): Promise<void>;
    loadAll(walletIdsKey: string): Promise<Wallet[]>;
    delete(id: string, password: string): Promise<void>;
  }

  export class KeyStoreImpl implements IKeyStore {
    private readonly core: WalletCore;
    private readonly storage: IKeyStoreStorage;
    private readonly prefix: string;
    private readonly walletIdsKey: string;

    constructor(core: WalletCore, prefix: string, storage: IKeyStoreStorage) {
      this.core = core;
      this.prefix = prefix;
      this.storage = storage;
      this.walletIdsKey = prefix + "-walletIds";
    }

    hasWallet(id: string): Promise<boolean> {
      return this.storage
        .get(this.prefix + id)
        .then((wallet) => {
          return wallet !== undefined;
        })
        .catch((error) => {
          return false;
        });
    }

    load(id: string): Promise<KeyStore.Wallet> {
      return this.storage.get(id).then((wallet) => {
        if (wallet === null) {
          throw KeyStore.Error.WalletNotFound;
        }
        return wallet;
      });
    }

    loadAll(): Promise<Wallet[]> {
      return this.storage.loadAll(this.walletIdsKey);
    }

    delete(id: string, password: string): Promise<void> {
      return this.storage.delete(id, password);
    }

    mapWallet(storedKey: StoredKey): KeyStore.Wallet {
      let json = storedKey.exportJSON();
      let wallet = JSON.parse(Buffer.from(json).toString()) as KeyStore.Wallet;
      return wallet;
    }

    mapStoredKey(wallet: KeyStore.Wallet): StoredKey {
      let json = Buffer.from(JSON.stringify(wallet));
      let storedKey = this.core.StoredKey.importJSON(json);
      return storedKey;
    }

    importWallet(wallet: KeyStore.Wallet): Promise<void> {
      return this.storage.set(wallet.id, wallet);
    }

    import(
      mnemonic: string,
      name: string,
      password: string,
      coins: CoinType[]
    ): Promise<KeyStore.Wallet> {
      return new Promise((resolve, reject) => {
        const { Mnemonic, StoredKey, HDWallet } = this.core;

        if (!Mnemonic.isValid(mnemonic)) {
          throw KeyStore.Error.InvalidMnemonic;
        }
        let pass = Buffer.from(password);
        let storedKey = StoredKey.importHDWallet(
          mnemonic,
          name,
          pass,
          coins[0]
        );
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
      return new Promise((resolve, reject) => {
        const { StoredKey, PrivateKey, Curve } = this.core;

        // FIXME: get curve from coin
        if (
          !PrivateKey.isValid(key, Curve.secp256k1) ||
          !PrivateKey.isValid(key, Curve.ed25519)
        ) {
          throw KeyStore.Error.InvalidKey;
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
  }
}
