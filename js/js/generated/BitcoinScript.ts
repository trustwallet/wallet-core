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

import { CoinType } from './CoinType';

// Class
export class BitcoinScript {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new BitcoinScript(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(): BitcoinScript {
        return new BitcoinScript(WalletCore.Js_wallet_core_napi_BitcoinScript_create());
    }

    public static createWithData(data: Uint8Array): BitcoinScript {
        return new BitcoinScript(WalletCore.Js_wallet_core_napi_BitcoinScript_createWithData(data));
    }

    public static createCopy(script: BitcoinScript): BitcoinScript {
        return new BitcoinScript(WalletCore.Js_wallet_core_napi_BitcoinScript_createCopy(script.getNativeHandle()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_BitcoinScript_delete(this.getNativeHandle());
    }

    public static equals(lhs: BitcoinScript, rhs: BitcoinScript): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_equals(lhs.getNativeHandle(), rhs.getNativeHandle());
    }

    public static buildPayToPublicKeyHash(hash: Uint8Array): BitcoinScript {
        return BitcoinScript.createWithNativeHandle(WalletCore.Js_wallet_core_napi_BitcoinScript_buildPayToPublicKeyHash(hash));
    }

    public static buildPayToScriptHash(scriptHash: Uint8Array): BitcoinScript {
        return BitcoinScript.createWithNativeHandle(WalletCore.Js_wallet_core_napi_BitcoinScript_buildPayToScriptHash(scriptHash));
    }

    public static buildPayToWitnessPubkeyHash(hash: Uint8Array): BitcoinScript {
        return BitcoinScript.createWithNativeHandle(WalletCore.Js_wallet_core_napi_BitcoinScript_buildPayToWitnessPubkeyHash(hash));
    }

    public static buildPayToWitnessScriptHash(scriptHash: Uint8Array): BitcoinScript {
        return BitcoinScript.createWithNativeHandle(WalletCore.Js_wallet_core_napi_BitcoinScript_buildPayToWitnessScriptHash(scriptHash));
    }

    public static buildForAddress(address: string, coin: CoinType): BitcoinScript {
        return BitcoinScript.createWithNativeHandle(WalletCore.Js_wallet_core_napi_BitcoinScript_buildForAddress(address, coin));
    }

    public size(): Long {
        return Long.fromString(WalletCore.Js_wallet_core_napi_BitcoinScript_size(this.getNativeHandle()), true);
    }

    public data(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_data(this.getNativeHandle());
    }

    public scriptHash(): Uint8Array {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_scriptHash(this.getNativeHandle());
    }

    public isPayToScriptHash(): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_isPayToScriptHash(this.getNativeHandle());
    }

    public isPayToWitnessScriptHash(): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_isPayToWitnessScriptHash(this.getNativeHandle());
    }

    public isWitnessProgram(): boolean {
        return WalletCore.Js_wallet_core_napi_BitcoinScript_isWitnessProgram(this.getNativeHandle());
    }

    public matchPayToPubkey(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_matchPayToPubkey(this.getNativeHandle());
    }

    public matchPayToPubkeyHash(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_matchPayToPubkeyHash(this.getNativeHandle());
    }

    public matchPayToScriptHash(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_matchPayToScriptHash(this.getNativeHandle());
    }

    public matchPayToWitnessPublicKeyHash(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_matchPayToWitnessPublicKeyHash(this.getNativeHandle());
    }

    public matchPayToWitnessScriptHash(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_matchPayToWitnessScriptHash(this.getNativeHandle());
    }

    public encode(): Uint8Array {

        return WalletCore.Js_wallet_core_napi_BitcoinScript_encode(this.getNativeHandle());
    }

}
