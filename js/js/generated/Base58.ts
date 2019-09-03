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

// Struct
export class Base58 {

    public static encode(data: Uint8Array): string {
        return WalletCore.Js_wallet_core_napi_Base58_encode(data);
    }

    public static encodeNoCheck(data: Uint8Array): string {
        return WalletCore.Js_wallet_core_napi_Base58_encodeNoCheck(data);
    }

    public static decode(string: string): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Base58_decode(string);
    }

    public static decodeNoCheck(string: string): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Base58_decodeNoCheck(string);
    }

}
