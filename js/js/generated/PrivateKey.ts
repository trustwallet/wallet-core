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
import { Curve } from './Curve';

// Class
export class PrivateKey {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new PrivateKey(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(): PrivateKey {
        return new PrivateKey(WalletCore.Js_wallet_core_napi_PrivateKey_create());
    }

    public static createWithData(data: Uint8Array): PrivateKey {
        return new PrivateKey(WalletCore.Js_wallet_core_napi_PrivateKey_createWithData(data));
    }

    public static createCopy(key: PrivateKey): PrivateKey {
        return new PrivateKey(WalletCore.Js_wallet_core_napi_PrivateKey_createCopy(key.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_PrivateKey_delete(this.getNativeHandle());
    }

    public static isValid(data: Uint8Array, curve: Curve): boolean {
        return WalletCore.Js_wallet_core_napi_PrivateKey_isValid(data, curve);
    }

    public data(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_PrivateKey_data(this.getNativeHandle());
    }

    public getPublicKeySecp256k1(compressed: boolean): PublicKey {

        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PrivateKey_getPublicKeySecp256k1(this.getNativeHandle(), compressed));
    }

    public getPublicKeyNist256p1(): PublicKey {

        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PrivateKey_getPublicKeyNist256p1(this.getNativeHandle()));
    }

    public getPublicKeyEd25519(): PublicKey {

        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519(this.getNativeHandle()));
    }

    public getPublicKeyEd25519Blake2b(): PublicKey {

        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519Blake2b(this.getNativeHandle()));
    }

    public getPublicKeyCurve25519(): PublicKey {

        return PublicKey.createWithNativeHandle(WalletCore.Js_wallet_core_napi_PrivateKey_getPublicKeyCurve25519(this.getNativeHandle()));
    }

    public sign(digest: Uint8Array, curve: Curve): Uint8Array {

        return WalletCore.Js_wallet_core_napi_PrivateKey_sign(this.getNativeHandle(), digest, curve);
    }

    public signAsDER(digest: Uint8Array, curve: Curve): Uint8Array {

        return WalletCore.Js_wallet_core_napi_PrivateKey_signAsDER(this.getNativeHandle(), digest, curve);
    }

    public signSchnorr(message: Uint8Array, curve: Curve): Uint8Array {

        return WalletCore.Js_wallet_core_napi_PrivateKey_signSchnorr(this.getNativeHandle(), message, curve);
    }

}
