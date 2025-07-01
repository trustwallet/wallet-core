// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import { CoinType, Derivation, PrivateKey, StoredKeyEncryption } from "../wallet-core";

export enum WalletType {
  Mnemonic = "mnemonic",
  PrivateKey = "private-key",
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
  UnsupportedWalletType = "unsupported wallet type",
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

export interface CoinWithDerivation {
  coin: CoinType,
  derivation: Derivation,
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
    coins: CoinType[],
    encryption: StoredKeyEncryption
  ): Promise<Wallet>;

  // Import a wallet by private key, name and password
  importKey(
    key: Uint8Array,
    name: string,
    password: string,
    coin: CoinType,
    encryption: StoredKeyEncryption,
    derivation: Derivation
  ): Promise<Wallet>;

  // Import a Wallet object directly
  importWallet(wallet: Wallet): Promise<void>;

  // Add active accounts to a wallet by wallet id, password, coin
  addAccounts(id: string, password: string, coins: CoinType[]): Promise<Wallet>;

  // Add active accounts paired with corresponding derivations to a wallet by wallet id, password, coin.
  addAccountsWithDerivations(id: string, password: string, coins: CoinWithDerivation[]): Promise<Wallet>;

  // Get private key of an account by wallet id, password, coin and derivation path
  getKey(
    id: string,
    password: string,
    account: ActiveAccount
  ): Promise<PrivateKey>;

  // Delete a wallet by wallet id and password.aq1aq
  delete(id: string, password: string): Promise<void>;

  // Export a wallet by wallet id and password, returns mnemonic or private key
  export(id: string, password: string): Promise<string | Uint8Array>;
}

export interface IStorage {
  get(id: string): Promise<Wallet>;
  set(id: string, wallet: Wallet): Promise<void>;
  loadAll(): Promise<Wallet[]>;
  delete(id: string, password: string): Promise<void>;
}
