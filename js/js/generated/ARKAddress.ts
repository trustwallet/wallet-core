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
export class ARKAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new ARKAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): ARKAddress {
        return new ARKAddress(WalletCore.Js_wallet_core_napi_ARKAddress_createWithString(string));
    }

    public static createWithPublicKey(publicKey: PublicKey): ARKAddress {
        return new ARKAddress(WalletCore.Js_wallet_core_napi_ARKAddress_createWithPublicKey(publicKey.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_ARKAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: ARKAddress, rhs: ARKAddress): boolean {
        return WalletCore.Js_wallet_core_napi_ARKAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_ARKAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_ARKAddress_description(this.getNativeHandle());
    }

}
