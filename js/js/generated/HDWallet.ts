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

import { PrivateKey } from './PrivateKey';
import { CoinType } from './CoinType';
import { Purpose } from './Purpose';
import { HDVersion } from './HDVersion';
import { PublicKey } from './PublicKey';

// Class
export class HDWallet {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new HDWallet(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(strength: number, passphrase: string): HDWallet {
        return new HDWallet(WalletCore.Js_wallet_core_napi_HDWallet_create(strength, passphrase));
    }

    public static createWithMnemonic(mnemonic: string, passphrase: string): HDWallet {
        return new HDWallet(WalletCore.Js_wallet_core_napi_HDWallet_createWithMnemonic(mnemonic, passphrase));
    }

    public static createWithData(data: Uint8Array, passphrase: string): HDWallet {
        return new HDWallet(WalletCore.Js_wallet_core_napi_HDWallet_createWithData(data, passphrase));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_HDWallet_delete(this.getNativeHandle());
    }

    public static isValid(mnemonic: string): boolean {
        return WalletCore.Js_wallet_core_napi_HDWallet_isValid(mnemonic);
    }

    public static getPublicKeyFromExtended(extended: string, derivationPath: string): PublicKey {
        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_HDWallet_getPublicKeyFromExtended(extended, derivationPath));
    }

    public seed(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_HDWallet_seed(this.getNativeHandle());
    }

    public mnemonic(): string {
        return WalletCore.Js_wallet_core_napi_HDWallet_mnemonic(this.getNativeHandle());
    }

    public getKeyForCoin(coin: CoinType): PrivateKey {

        return PrivateKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_HDWallet_getKeyForCoin(this.getNativeHandle(), coin));
    }

    public getKey(derivationPath: string): PrivateKey {

        return PrivateKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_HDWallet_getKey(this.getNativeHandle(), derivationPath));
    }

    public getKeyBIP44(coin: CoinType, account: number, change: number, address: number): PrivateKey {

        return PrivateKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_HDWallet_getKeyBIP44(this.getNativeHandle(), coin, account, change, address));
    }

    public getExtendedPrivateKey(purpose: Purpose, coin: CoinType, version: HDVersion): string {

        return WalletCore.Js_wallet_core_napi_HDWallet_getExtendedPrivateKey(this.getNativeHandle(), purpose, coin, version);
    }

    public getExtendedPublicKey(purpose: Purpose, coin: CoinType, version: HDVersion): string {

        return WalletCore.Js_wallet_core_napi_HDWallet_getExtendedPublicKey(this.getNativeHandle(), purpose, coin, version);
    }

}
