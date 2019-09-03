// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import { WalletCore } from '../WalletCore';
import { TW } from '../../proto/TrustWalletCore';
import * as Long from 'long';

import { Account } from './Account';
import { CoinType } from './CoinType';
import { HDWallet } from './HDWallet';
import { PrivateKey } from './PrivateKey';

// Class
export class StoredKey {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new StoredKey(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(name: string, password: string): StoredKey {
        return new StoredKey(WalletCore.Js_wallet_core_napi_StoredKey_create(name, password));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_StoredKey_delete(this.getNativeHandle());
    }

    public static load(path: string): StoredKey {
        return StoredKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_load(path));
    }

    public static importPrivateKey(privateKey: Uint8Array, name: string, password: string, coin: CoinType): StoredKey {
        return StoredKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_importPrivateKey(privateKey, name, password, coin));
    }

    public static importHDWallet(mnemonic: string, name: string, password: string, coin: CoinType): StoredKey {
        return StoredKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_importHDWallet(mnemonic, name, password, coin));
    }

    public static importJSON(json: Uint8Array): StoredKey {
        return StoredKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_importJSON(json));
    }

    public identifier(): string {
        return WalletCore.Js_wallet_core_napi_StoredKey_identifier(this.getNativeHandle());
    }

    public name(): string {
        return WalletCore.Js_wallet_core_napi_StoredKey_name(this.getNativeHandle());
    }

    public isMnemonic(): boolean {
        return WalletCore.Js_wallet_core_napi_StoredKey_isMnemonic(this.getNativeHandle());
    }

    public accountCount(): Long {
        return Long.fromString(WalletCore.Js_wallet_core_napi_StoredKey_accountCount(this.getNativeHandle()), true);
    }

    public account(indexUint64: Long): Account {

        return Account.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_account(this.getNativeHandle(), indexUint64.toString()));
    }

    public accountForCoin(coin: CoinType, wallet: HDWallet): Account {

        return Account.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_accountForCoin(this.getNativeHandle(), coin, wallet.getNativeHandle()));
    }

    public addAccount(address: string, derivationPath: string, extetndedPublicKey: string): void {

        return WalletCore.Js_wallet_core_napi_StoredKey_addAccount(this.getNativeHandle(), address, derivationPath, extetndedPublicKey);
    }

    public store(path: string): boolean {

        return WalletCore.Js_wallet_core_napi_StoredKey_store(this.getNativeHandle(), path);
    }

    public decryptPrivateKey(password: string): Uint8Array {

        return WalletCore.Js_wallet_core_napi_StoredKey_decryptPrivateKey(this.getNativeHandle(), password);
    }

    public decryptMnemonic(password: string): string {

        return WalletCore.Js_wallet_core_napi_StoredKey_decryptMnemonic(this.getNativeHandle(), password);
    }

    public privateKey(coin: CoinType, password: string): PrivateKey {

        return PrivateKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_privateKey(this.getNativeHandle(), coin, password));
    }

    public wallet(password: string): HDWallet {

        return HDWallet.createWithNativeHandle(WalletCore.Js_wallet_core_napi_StoredKey_wallet(this.getNativeHandle(), password));
    }

    public exportJSON(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_StoredKey_exportJSON(this.getNativeHandle());
    }

    public fixAddresses(password: string): boolean {

        return WalletCore.Js_wallet_core_napi_StoredKey_fixAddresses(this.getNativeHandle(), password);
    }

}
