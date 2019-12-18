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

// Class
export class ZcashTAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new ZcashTAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): ZcashTAddress {
        return new ZcashTAddress(WalletCore.Js_wallet_core_napi_ZcashTAddress_createWithString(string));
    }

    public static createWithData(data: Uint8Array): ZcashTAddress {
        return new ZcashTAddress(WalletCore.Js_wallet_core_napi_ZcashTAddress_createWithData(data));
    }

    public static createWithPublicKey(publicKey: PublicKey, prefix: number): ZcashTAddress {
        return new ZcashTAddress(WalletCore.Js_wallet_core_napi_ZcashTAddress_createWithPublicKey(publicKey.getNativeHandle(), prefix));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_ZcashTAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: ZcashTAddress, rhs: ZcashTAddress): boolean {
        return WalletCore.Js_wallet_core_napi_ZcashTAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValid(data: Uint8Array): boolean {
        return WalletCore.Js_wallet_core_napi_ZcashTAddress_isValid(data);
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_ZcashTAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_ZcashTAddress_description(this.getNativeHandle());
    }

}
