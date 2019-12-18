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
export class AES {

    public static cbcencrypt(key: Uint8Array, data: Uint8Array, iv: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_AES_cbcencrypt(key, data, iv);
    }

    public static cbcdecrypt(key: Uint8Array, data: Uint8Array, iv: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_AES_cbcdecrypt(key, data, iv);
    }

    public static ctrencrypt(key: Uint8Array, data: Uint8Array, iv: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_AES_ctrencrypt(key, data, iv);
    }

    public static ctrdecrypt(key: Uint8Array, data: Uint8Array, iv: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_AES_ctrdecrypt(key, data, iv);
    }

}
