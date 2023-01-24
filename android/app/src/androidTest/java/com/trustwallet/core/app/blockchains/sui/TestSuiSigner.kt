// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.sui

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Sui

class TestSuiSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun SuiTransactionSigning() {
        // Successfully broadcasted https://explorer.sui.io/transaction/rxLgxcAqgMg8gphp6eCsSGQcdZnwFYx2SRdwEhnAUC4
        val txBytes = """
            AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA
        """.trimIndent()
        val key =
            "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266".toHexBytesInByteString()
        val signDirect = Sui.SignDirect.newBuilder().setUnsignedTxMsg(txBytes).build()
        val signingInput =
            Sui.SigningInput.newBuilder().setSignDirectMessage(signDirect).setPrivateKey(key).build()
        val result = AnySigner.sign(signingInput, CoinType.SUI, Sui.SigningOutput.parser())
        val expectedSignature = "AIYRmHDpQesfAx3iWBCMwInf3MZ56ZQGnPWNtECFjcSq0ssAgjRW6GLnFCX24tfDNjSm9gjYgoLmn1No15iFJAtqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg=="
        assertEquals(result.unsignedTx, txBytes);
        assertEquals(result.signature, expectedSignature)
    }
}
