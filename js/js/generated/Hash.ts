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
export class Hash {

    public static sha1(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha1(data);
    }

    public static sha256(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha256(data);
    }

    public static sha512(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha512(data);
    }

    public static keccak256(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_keccak256(data);
    }

    public static keccak512(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_keccak512(data);
    }

    public static sha3256(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha3256(data);
    }

    public static sha3512(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha3512(data);
    }

    public static ripemd(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_ripemd(data);
    }

    public static blake2b(data: Uint8Array, sizeUint64: Long): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_blake2b(data, sizeUint64.toString());
    }

    public static sha256RIPEMD(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha256RIPEMD(data);
    }

    public static sha256SHA256(data: Uint8Array): Uint8Array {
        return WalletCore.Js_wallet_core_napi_Hash_sha256SHA256(data);
    }

}
