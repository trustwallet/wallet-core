// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

actual object AnySigner {

    @JvmStatic
    actual external fun sign(input: ByteArray, coin: CoinType): ByteArray

    @JvmStatic
    actual external fun supportsJson(coin: CoinType): Boolean

    @JvmStatic
    actual external fun signJson(json: String, key: ByteArray, coin: CoinType): String

    @JvmStatic
    actual external fun plan(input: ByteArray, coin: CoinType): ByteArray
}
