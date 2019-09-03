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
export class KeyDerivation {

    public static scrypt(password: string, salt: Uint8Array, nUint64: Long, r: number, p: number, keyLengthUint64: Long): Uint8Array {
        return WalletCore.Js_wallet_core_napi_KeyDerivation_scrypt(password, salt, nUint64.toString(), r, p, keyLengthUint64.toString());
    }

    public static pbkdf2256(password: string, salt: Uint8Array, iterations: number, keyLengthUint64: Long): Uint8Array {
        return WalletCore.Js_wallet_core_napi_KeyDerivation_pbkdf2256(password, salt, iterations, keyLengthUint64.toString());
    }

    public static pbkdf2512(password: string, salt: Uint8Array, iterations: number, keyLengthUint64: Long): Uint8Array {
        return WalletCore.Js_wallet_core_napi_KeyDerivation_pbkdf2512(password, salt, iterations, keyLengthUint64.toString());
    }

}
