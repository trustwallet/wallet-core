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

import { HRP } from './HRP';
import { PublicKey } from './PublicKey';

// Class
export class SegwitAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new SegwitAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): SegwitAddress {
        return new SegwitAddress(WalletCore.Js_wallet_core_napi_SegwitAddress_createWithString(string));
    }

    public static createWithPublicKey(hrp: HRP, publicKey: PublicKey): SegwitAddress {
        return new SegwitAddress(WalletCore.Js_wallet_core_napi_SegwitAddress_createWithPublicKey(hrp, publicKey.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_SegwitAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: SegwitAddress, rhs: SegwitAddress): boolean {
        return WalletCore.Js_wallet_core_napi_SegwitAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_SegwitAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_SegwitAddress_description(this.getNativeHandle());
    }

    public hrp(): HRP {
        return WalletCore.Js_wallet_core_napi_SegwitAddress_hrp(this.getNativeHandle());
    }

    public witnessProgram(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_SegwitAddress_witnessProgram(this.getNativeHandle());
    }

}
