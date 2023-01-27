package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

expect object AnySigner {

    fun <T : Message<T, *>> sign(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T

    fun supportsJSON(coin: CoinType): Boolean

    fun signJSON(json: String, key: ByteArray, coin: CoinType): String

    fun <T : Message<T, *>> plan(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T
}
