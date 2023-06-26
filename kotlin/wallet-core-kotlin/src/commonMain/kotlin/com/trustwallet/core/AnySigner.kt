// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
