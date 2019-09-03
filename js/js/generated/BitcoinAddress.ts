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
export class BitcoinAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new BitcoinAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): BitcoinAddress {
        return new BitcoinAddress(WalletCore.Js_wallet_core_napi_BitcoinAddress_createWithString(string));
    }

    public static createWithData(data: Uint8Array): BitcoinAddress {
        return new BitcoinAddress(WalletCore.Js_wallet_core_napi_BitcoinAddress_createWithData(data));
    }

    public static createWithPublicKey(publicKey: PublicKey, prefix: number): BitcoinAddress {
        return new BitcoinAddress(WalletCore.Js_wallet_core_napi_BitcoinAddress_createWithPublicKey(publicKey.getNativeHandle(), prefix));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_BitcoinAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: BitcoinAddress, rhs: BitcoinAddress): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValid(data: Uint8Array): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_isValid(data);
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_description(this.getNativeHandle());
    }

    public prefix(): number {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_prefix(this.getNativeHandle());
    }

    public keyhash(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_BitcoinAddress_keyhash(this.getNativeHandle());
    }

}
