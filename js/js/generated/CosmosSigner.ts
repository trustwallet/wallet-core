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
export class CosmosSigner {

    public static sign(input: TW.Cosmos.Proto.SigningInput): TW.Cosmos.Proto.SigningOutput {
        return TW.Cosmos.Proto.SigningOutput.decode(WalletCore.Js_wallet_core_napi_CosmosSigner_sign(
                  TW.Cosmos.Proto.SigningInput.encode(input).finish()));
    }

}
