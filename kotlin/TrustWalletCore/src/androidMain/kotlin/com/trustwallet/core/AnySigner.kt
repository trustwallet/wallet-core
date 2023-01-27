package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

actual object AnySigner {

    actual fun <T : Message<T, *>> sign(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(nativeSign(input.encode(), coin.value))

    private external fun nativeSign(data: ByteArray, coin: UInt): ByteArray

    private external fun supportsJSON(coin: UInt): Boolean

    actual fun supportsJSON(coin: CoinType): Boolean =
        supportsJSON(coin.value)

    private external fun signJSON(json: String, key: ByteArray, coin: UInt): String

    actual fun signJSON(json: String, key: ByteArray, coin: CoinType): String =
        signJSON(json, key, coin.value)

    private external fun nativePlan(data: ByteArray, coin: UInt): ByteArray

    actual fun <T : Message<T, *>> plan(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(nativePlan(input.encode(), coin.value))
}
