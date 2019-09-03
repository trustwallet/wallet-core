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

import { CoinType } from './CoinType';

// Class
export class Account {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new Account(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(address: string, derivationPath: string, extendedPublicKey: string): Account {
        return new Account(WalletCore.Js_wallet_core_napi_Account_create(address, derivationPath, extendedPublicKey));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_Account_delete(this.getNativeHandle());
    }

    public address(): string {
        return WalletCore.Js_wallet_core_napi_Account_address(this.getNativeHandle());
    }

    public derivationPath(): string {
        return WalletCore.Js_wallet_core_napi_Account_derivationPath(this.getNativeHandle());
    }

    public extendedPublicKey(): string {
        return WalletCore.Js_wallet_core_napi_Account_extendedPublicKey(this.getNativeHandle());
    }

    public coin(): CoinType {
        return WalletCore.Js_wallet_core_napi_Account_coin(this.getNativeHandle());
    }

}
