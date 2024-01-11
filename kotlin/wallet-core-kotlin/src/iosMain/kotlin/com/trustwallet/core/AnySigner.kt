// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
