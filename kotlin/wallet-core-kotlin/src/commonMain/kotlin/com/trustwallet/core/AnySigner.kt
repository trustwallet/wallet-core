// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

expect object AnySigner {

    fun sign(input: ByteArray, coin: CoinType): ByteArray

    fun supportsJson(coin: CoinType): Boolean

    fun signJson(json: String, key: ByteArray, coin: CoinType): String

    fun plan(input: ByteArray, coin: CoinType): ByteArray
}

fun <T : Message<T, *>> AnySigner.sign(input: Message<*, *>, coin: CoinType, adapter: ProtoAdapter<T>): T =
    adapter.decode(sign(input.encode(), coin))

fun <T : Message<T, *>> AnySigner.plan(input: Message<*, *>, coin: CoinType, adapter: ProtoAdapter<T>): T =
    adapter.decode(plan(input.encode(), coin))
