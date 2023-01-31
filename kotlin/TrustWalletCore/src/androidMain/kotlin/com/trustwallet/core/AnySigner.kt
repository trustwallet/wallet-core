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

    @JvmStatic
    @JvmName("nativeSign")
    private external fun nativeSign(data: ByteArray, coin: UInt): ByteArray

    @JvmStatic
    @JvmName("supportsJSON")
    private external fun supportsJSON(coin: UInt): Boolean

    actual fun supportsJSON(coin: CoinType): Boolean =
        supportsJSON(coin.value)

    @JvmStatic
    @JvmName("signJSON")
    private external fun signJSON(json: String, key: ByteArray, coin: UInt): String

    actual fun signJSON(json: String, key: ByteArray, coin: CoinType): String =
        signJSON(json, key, coin.value)

    @JvmStatic
    @JvmName("nativePlan")
    private external fun nativePlan(data: ByteArray, coin: UInt): ByteArray

    actual fun <T : Message<T, *>> plan(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(nativePlan(input.encode(), coin.value))
}
