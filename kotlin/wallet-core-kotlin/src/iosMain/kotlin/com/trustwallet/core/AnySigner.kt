// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.toCValues

@OptIn(kotlinx.cinterop.ExperimentalForeignApi::class)
actual object AnySigner {

    actual fun sign(input: ByteArray, coin: CoinType): ByteArray {
        val inputData = TWDataCreateWithBytes(input.toUByteArray().toCValues(), input.size.toULong())
        val result = TWAnySignerSign(inputData, coin.value)!!.readTwBytes()!!
        TWDataDelete(inputData)
        return result
    }

    actual fun supportsJson(coin: CoinType): Boolean =
        TWAnySignerSupportsJSON(coin.value)

    actual fun signJson(json: String, key: ByteArray, coin: CoinType): String {
        val jsonString = TWStringCreateWithUTF8Bytes(json)
        val keyData = TWDataCreateWithBytes(key.toUByteArray().toCValues(), key.size.toULong())
        val result = TWAnySignerSignJSON(jsonString, keyData, coin.value).fromTwString()!!
        TWStringDelete(jsonString)
        TWDataDelete(keyData)
        return result
    }

    actual fun plan(input: ByteArray, coin: CoinType): ByteArray {
        val inputData = TWDataCreateWithBytes(input.toUByteArray().toCValues(), input.size.toULong())
        val result = TWAnySignerPlan(inputData, coin.value)?.readTwBytes()!!
        TWDataDelete(inputData)
        return result
    }
}
