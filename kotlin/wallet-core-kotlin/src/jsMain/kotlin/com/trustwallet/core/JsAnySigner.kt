// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
