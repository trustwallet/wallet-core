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
export class CosmosAddress {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new CosmosAddress(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithString(string: string): CosmosAddress {
        return new CosmosAddress(WalletCore.Js_wallet_core_napi_CosmosAddress_createWithString(string));
    }

    public static createWithKeyHash(hrp: HRP, keyHash: Uint8Array): CosmosAddress {
        return new CosmosAddress(WalletCore.Js_wallet_core_napi_CosmosAddress_createWithKeyHash(hrp, keyHash));
    }

    public static createWithPublicKey(hrp: HRP, publicKey: PublicKey): CosmosAddress {
        return new CosmosAddress(WalletCore.Js_wallet_core_napi_CosmosAddress_createWithPublicKey(hrp, publicKey.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_CosmosAddress_delete(this.getNativeHandle());
    }

    public static equals(lhs: CosmosAddress, rhs: CosmosAddress): boolean {
        return WalletCore.Js_wallet_core_napi_CosmosAddress_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static isValidString(string: string): boolean {
        return WalletCore.Js_wallet_core_napi_CosmosAddress_isValidString(string);
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_CosmosAddress_description(this.getNativeHandle());
    }

    public hrp(): HRP {
        return WalletCore.Js_wallet_core_napi_CosmosAddress_hrp(this.getNativeHandle());
    }

    public keyHash(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_CosmosAddress_keyHash(this.getNativeHandle());
    }

}
