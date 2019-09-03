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

import { PublicKeyType } from './PublicKeyType';

// Class
export class PublicKey {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new PublicKey(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static createWithData(data: Uint8Array, type: PublicKeyType): PublicKey {
        return new PublicKey(WalletCore.Js_wallet_core_napi_PublicKey_createWithData(data, type));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_PublicKey_delete(this.getNativeHandle());
    }

    public static isValid(data: Uint8Array, type: PublicKeyType): boolean {
        return WalletCore.Js_wallet_core_napi_PublicKey_isValid(data, type);
    }

    public static recover(signature: Uint8Array, message: Uint8Array): PublicKey {
        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PublicKey_recover(signature, message));
    }

    public isCompressed(): boolean {
        return WalletCore.Js_wallet_core_napi_PublicKey_isCompressed(this.getNativeHandle());
    }

    public compressed(): PublicKey {
        return WalletCore.Js_wallet_core_napi_PublicKey_compressed(this.getNativeHandle());
    }

    public uncompressed(): PublicKey {
        return WalletCore.Js_wallet_core_napi_PublicKey_uncompressed(this.getNativeHandle());
    }

    public data(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_PublicKey_data(this.getNativeHandle());
    }

    public keyType(): PublicKeyType {
        return WalletCore.Js_wallet_core_napi_PublicKey_keyType(this.getNativeHandle());
    }

    public description(): string {
        return WalletCore.Js_wallet_core_napi_PublicKey_description(this.getNativeHandle());
    }

    public verify(signature: Uint8Array, message: Uint8Array): boolean {

        return WalletCore.Js_wallet_core_napi_PublicKey_verify(this.getNativeHandle(), signature, message);
    }

    public verifySchnorr(signature: Uint8Array, message: Uint8Array): boolean {

        return WalletCore.Js_wallet_core_napi_PublicKey_verifySchnorr(this.getNativeHandle(), signature, message);
    }

}
