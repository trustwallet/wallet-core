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
import { BravoAddressType } from './BravoAddressType';

// Class
export class BravoAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new BravoAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): BravoAddress {
        return new BravoAddress(WalletCore.Js_wallet_core_napi_BravoAddress_createWithString(string));
    }

    public static createWithPublicKey(publicKey: PublicKey, type: BravoAddressType): BravoAddress {
        return new BravoAddress(WalletCore.Js_wallet_core_napi_BravoAddress_createWithPublicKey(publicKey.getNativeHandle(), type));
    }

    public static createWithKeyHash(keyHash: Uint8Array, type: BravoAddressType): BravoAddress {
        return new BravoAddress(WalletCore.Js_wallet_core_napi_BravoAddress_createWithKeyHash(keyHash, type));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_BravoAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: BravoAddress, rhs: BravoAddress): boolean {
        return WalletCore.Js_wallet_core_napi_BravoAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_BravoAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_BravoAddress_description(this.getNativeHandle());
    }

}
