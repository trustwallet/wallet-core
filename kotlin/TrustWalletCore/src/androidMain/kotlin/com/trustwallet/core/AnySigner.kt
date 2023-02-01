package com.trustwallet.core

actual external fun signImpl(input: ByteArray, coin: CoinType): ByteArray

actual external fun supportsJsonImpl(coin: CoinType): Boolean

actual external fun signJsonImpl(json: String, key: ByteArray, coin: CoinType): String

actual external fun planImpl(input: ByteArray, coin: CoinType): ByteArray
