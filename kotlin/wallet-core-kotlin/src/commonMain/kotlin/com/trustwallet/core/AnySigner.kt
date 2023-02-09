// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

object AnySigner {

    fun <T : Message<T, *>> sign(input: Message<*, *>, coin: CoinType, adapter: ProtoAdapter<T>): T =
        adapter.decode(signImpl(input.encode(), coin))

    fun supportsJson(coin: CoinType): Boolean = supportsJsonImpl(coin)

    fun signJson(json: String, key: ByteArray, coin: CoinType): String =
        signJsonImpl(json, key, coin)

    fun <T : Message<T, *>> plan(input: Message<*, *>, coin: CoinType, adapter: ProtoAdapter<T>): T =
        adapter.decode(planImpl(input.encode(), coin))
}

internal expect fun signImpl(input: ByteArray, coin: CoinType): ByteArray

internal expect fun supportsJsonImpl(coin: CoinType): Boolean

internal expect fun signJsonImpl(json: String, key: ByteArray, coin: CoinType): String

internal expect fun planImpl(input: ByteArray, coin: CoinType): ByteArray
