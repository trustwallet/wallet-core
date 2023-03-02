// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

@JsModule("@trustwallet/wallet-core")
@JsName("AnySigner")
external class JsAnySigner {
    companion object {
        fun sign(data: UInt8Array, coin: JsCoinType): UInt8Array
        fun plan(data: UInt8Array, coin: JsCoinType): UInt8Array
        fun supportsJSON(coin: JsCoinType): Boolean
    }
}

inline val JsWalletCore.AnySigner: JsAnySigner.Companion
    get() = asDynamic().AnySigner.unsafeCast<JsAnySigner.Companion>()
