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

import { PublicKey } from './PublicKey';
import { EOSKeyType } from './EOSKeyType';

// Class
export class EOSAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new EOSAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): EOSAddress {
        return new EOSAddress(WalletCore.Js_wallet_core_napi_EOSAddress_createWithString(string));
    }

    public static createWithPublicKey(publicKey: PublicKey, type: EOSKeyType): EOSAddress {
        return new EOSAddress(WalletCore.Js_wallet_core_napi_EOSAddress_createWithPublicKey(publicKey.getNativeHandle(), type));
    }

    public static createWithKeyHash(keyHash: Uint8Array, type: EOSKeyType): EOSAddress {
        return new EOSAddress(WalletCore.Js_wallet_core_napi_EOSAddress_createWithKeyHash(keyHash, type));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_EOSAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: EOSAddress, rhs: EOSAddress): boolean {
        return WalletCore.Js_wallet_core_napi_EOSAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_EOSAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_EOSAddress_description(this.getNativeHandle());
    }

}
