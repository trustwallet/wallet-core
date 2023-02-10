// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

actual external fun signImpl(input: ByteArray, coin: CoinType): ByteArray

actual external fun supportsJsonImpl(coin: CoinType): Boolean

actual external fun signJsonImpl(json: String, key: ByteArray, coin: CoinType): String

actual external fun planImpl(input: ByteArray, coin: CoinType): ByteArray
