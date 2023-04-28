// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

actual object AnySigner {

    actual fun sign(input: ByteArray, coin: CoinType): ByteArray =
        TWAnySignerSign(input.toTwData(), coin.value)!!.readTwBytes()!!

    actual fun supportsJson(coin: CoinType): Boolean =
        TWAnySignerSupportsJSON(coin.value)

    actual fun signJson(json: String, key: ByteArray, coin: CoinType): String =
        TWAnySignerSignJSON(json.toTwString(), key.toTwData(), coin.value).fromTwString()!!

    actual fun plan(input: ByteArray, coin: CoinType): ByteArray =
        TWAnySignerPlan(input.toTwData(), coin.value)?.readTwBytes()!!
}
