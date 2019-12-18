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
export class ARKSigner {

    public static sign(input: TW.ARK.Proto.SigningInput): TW.ARK.Proto.SigningOutput {
        return TW.ARK.Proto.SigningOutput.decode(WalletCore.Js_wallet_core_napi_ARKSigner_sign(
                  TW.ARK.Proto.SigningInput.encode(input).finish()));
    }

}
