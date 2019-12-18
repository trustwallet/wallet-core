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
export class TezosAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new TezosAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): TezosAddress {
        return new TezosAddress(WalletCore.Js_wallet_core_napi_TezosAddress_createWithString(string));
    }

    public static createWithPublicKey(publicKey: PublicKey): TezosAddress {
        return new TezosAddress(WalletCore.Js_wallet_core_napi_TezosAddress_createWithPublicKey(publicKey.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_TezosAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: TezosAddress, rhs: TezosAddress): boolean {
        return WalletCore.Js_wallet_core_napi_TezosAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_TezosAddress_isValidString(string);
    }

    public static deriveOriginatedAddress(operationHash: string, operationIndex: number): string {
        return WalletCore.Js_wallet_core_napi_TezosAddress_deriveOriginatedAddress(operationHash, operationIndex);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_TezosAddress_description(this.getNativeHandle());
    }

    public keyHash(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_TezosAddress_keyHash(this.getNativeHandle());
    }

}
