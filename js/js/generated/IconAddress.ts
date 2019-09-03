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

import { IconAddressType } from './IconAddressType';
import { PublicKey } from './PublicKey';

// Class
export class IconAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new IconAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): IconAddress {
        return new IconAddress(WalletCore.Js_wallet_core_napi_IconAddress_createWithString(string));
    }

    public static createWithKeyHash(keyHash: Uint8Array, type: IconAddressType): IconAddress {
        return new IconAddress(WalletCore.Js_wallet_core_napi_IconAddress_createWithKeyHash(keyHash, type));
    }

    public static createWithPublicKey(publicKey: PublicKey, type: IconAddressType): IconAddress {
        return new IconAddress(WalletCore.Js_wallet_core_napi_IconAddress_createWithPublicKey(publicKey.getNativeHandle(), type));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_IconAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: IconAddress, rhs: IconAddress): boolean {
        return WalletCore.Js_wallet_core_napi_IconAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_IconAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_IconAddress_description(this.getNativeHandle());
    }

    public keyHash(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_IconAddress_keyHash(this.getNativeHandle());
    }

}
