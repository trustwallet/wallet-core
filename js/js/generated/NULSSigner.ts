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
export class NULSSigner {

    public static plan(purpose: TW.NULS.Proto.TransactionPurpose): TW.NULS.Proto.TransactionPlan {
        return TW.NULS.Proto.TransactionPlan.decode(WalletCore.Js_wallet_core_napi_NULSSigner_plan(
                  TW.NULS.Proto.TransactionPurpose.encode(purpose).finish()));
    }

    public static sign(plan: TW.NULS.Proto.TransactionPlan): TW.NULS.Proto.SigningOutput {
        return TW.NULS.Proto.SigningOutput.decode(WalletCore.Js_wallet_core_napi_NULSSigner_sign(
                  TW.NULS.Proto.TransactionPlan.encode(plan).finish()));
    }

}
