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
export class ZcashTransactionSigner {

    private readonly nativeHandle: External;

    private constructor(nativeHandle: External) {
        this.nativeHandle = nativeHandle;
    }

    public static createWithNativeHandle(nativeHandle: External) {
        return new ZcashTransactionSigner(nativeHandle);
    }

    public getNativeHandle() {
        return this.nativeHandle;
    }

    public static create(input: TW.Bitcoin.Proto.SigningInput): ZcashTransactionSigner {
        return new ZcashTransactionSigner(WalletCore.Js_wallet_core_napi_ZcashTransactionSigner_create(
                  TW.Bitcoin.Proto.SigningInput.encode(input).finish()));
    }

    public static createWithPlan(input: TW.Bitcoin.Proto.SigningInput, plan: TW.Bitcoin.Proto.TransactionPlan): ZcashTransactionSigner {
        return new ZcashTransactionSigner(WalletCore.Js_wallet_core_napi_ZcashTransactionSigner_createWithPlan(
                  TW.Bitcoin.Proto.SigningInput.encode(input).finish(), 
                  TW.Bitcoin.Proto.TransactionPlan.encode(plan).finish()));
    }

    public delete(): void {
        WalletCore.Js_wallet_core_napi_ZcashTransactionSigner_delete(this.getNativeHandle());
    }

    public plan(): TW.Bitcoin.Proto.TransactionPlan {
        return TW.Bitcoin.Proto.TransactionPlan.decode(WalletCore.Js_wallet_core_napi_ZcashTransactionSigner_plan(this.getNativeHandle()));
    }

    public sign(): TW.Proto.Result {
        return TW.Proto.Result.decode(WalletCore.Js_wallet_core_napi_ZcashTransactionSigner_sign(this.getNativeHandle()));
    }

}
