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

import { BitcoinAddress } from './BitcoinAddress';
import { PublicKey } from './PublicKey';

// Struct
export class BitcoinCashAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        if (nativeHandle === null || nativeHandle === undefined) {
            throw new Error('nativeHandle cannot be null or undefined');
        }
        return new BitcoinCashAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static initWithString(string: string): BitcoinCashAddress | null {
        const result = WalletCore.Js_wallet_core_napi_BitcoinCashAddress_initWithString(string);
        if (result === null || result === undefined) {
            return null;
        }
        return new BitcoinCashAddress(result);
    }

    public static initWithData(data: Uint8Array): BitcoinCashAddress | null {
        const result = WalletCore.Js_wallet_core_napi_BitcoinCashAddress_initWithData(data);
        if (result === null || result === undefined) {
            return null;
        }
        return new BitcoinCashAddress(result);
    }

    public static initWithPublicKey(publicKey: PublicKey): BitcoinCashAddress | null {
        const result = WalletCore.Js_wallet_core_napi_BitcoinCashAddress_initWithPublicKey(publicKey.getNativeHandle());
        if (result === null || result === undefined) {
            return null;
        }
        return new BitcoinCashAddress(result);
    }

    public static equals(lhs: BitcoinCashAddress, rhs: BitcoinCashAddress): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinCashAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValid(data: Uint8Array): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinCashAddress_isValid(data);
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinCashAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_BitcoinCashAddress_description(this.getNativeHandle());
    }

    public legacyAddress(): BitcoinAddress {
        return WalletCore.Js_wallet_core_napi_BitcoinCashAddress_legacyAddress(this.getNativeHandle());
    }

}
