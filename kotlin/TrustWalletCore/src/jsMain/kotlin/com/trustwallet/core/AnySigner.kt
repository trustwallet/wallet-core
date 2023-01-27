package com.trustwallet.core

import com.squareup.wire.Message
import com.squareup.wire.ProtoAdapter

actual object AnySigner {

    actual fun <T : Message<T, *>> sign(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(
            WalletCore.Instance.AnySigner.sign(input.encode().toUInt8Array(), coin._value)
                .unsafeCast<UInt8Array>()
                .toByteArray(),
        )

    actual fun supportsJSON(coin: CoinType): Boolean =
        WalletCore.Instance.AnySigner.supportsJSON(coin._value) as Boolean

    actual fun signJSON(json: String, key: ByteArray, coin: CoinType): String =
        TODO()

    actual fun <T : Message<T, *>> plan(
        input: Message<*, *>,
        coin: CoinType,
        adapter: ProtoAdapter<T>,
    ): T =
        adapter.decode(
            WalletCore.Instance.AnySigner.plan(input.encode().toUInt8Array(), coin._value)
                .unsafeCast<UInt8Array>()
                .toByteArray(),
        )
}
