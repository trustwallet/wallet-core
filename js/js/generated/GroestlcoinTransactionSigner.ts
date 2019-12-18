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

// Class
export class GroestlcoinTransactionSigner {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new GroestlcoinTransactionSigner(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(input: TW.Bitcoin.Proto.SigningInput): GroestlcoinTransactionSigner {
        return new GroestlcoinTransactionSigner(WalletCore.Js_wallet_core_napi_GroestlcoinTransactionSigner_create(
                  TW.Bitcoin.Proto.SigningInput.encode(input).finish()));
    }

    public static createWithPlan(input: TW.Bitcoin.Proto.SigningInput, plan: TW.Bitcoin.Proto.TransactionPlan): GroestlcoinTransactionSigner {
        return new GroestlcoinTransactionSigner(WalletCore.Js_wallet_core_napi_GroestlcoinTransactionSigner_createWithPlan(
                  TW.Bitcoin.Proto.SigningInput.encode(input).finish(), 
                  TW.Bitcoin.Proto.TransactionPlan.encode(plan).finish()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_GroestlcoinTransactionSigner_delete(this.getNativeHandle());
    }

    public plan(): TW.Bitcoin.Proto.TransactionPlan {
        return TW.Bitcoin.Proto.TransactionPlan.decode(WalletCore.Js_wallet_core_napi_GroestlcoinTransactionSigner_plan(this.getNativeHandle()));
    }

    public sign(): TW.Proto.Result {
        return TW.Proto.Result.decode(WalletCore.Js_wallet_core_napi_GroestlcoinTransactionSigner_sign(this.getNativeHandle()));
    }

}
