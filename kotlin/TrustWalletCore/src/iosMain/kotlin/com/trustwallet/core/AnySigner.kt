package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

actual object AnySigner {

    actual fun <T : Message<T, *>> sign(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(TWAnySignerSign(input.encode().toTwData(), coin.value)!!.readTwBytes()!!)

    actual fun supportsJSON(coin: CoinType): Boolean =
        TWAnySignerSupportsJSON(coin.value)

    actual fun signJSON(json: String, key: ByteArray, coin: CoinType): String =
        TWAnySignerSignJSON(json.toTwString(), key.toTwData(), coin.value).fromTwString()!!

    actual fun <T : Message<T, *>> plan(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(TWAnySignerPlan(input.encode().toTwData(), coin.value)?.readTwBytes()!!)
}
