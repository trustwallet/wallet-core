package com.trustwallet.core.app.blockchains.tezos

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.TezosMessageSigner
import wallet.core.jni.PrivateKey
import java.util.regex.Pattern

class TestTezosMessageSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testMessageSignerSignAndVerify() {
        val data = Numeric.hexStringToByteArray("91b4fb8d7348db2e7de2693f58ce1cceb966fa960739adac1d9dba2cbaa0940a")
        val privateKey = PrivateKey(data)
        val msg = "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64"
        val signature = TezosMessageSigner.signMessage(privateKey, msg)
        assertEquals("edsigu3se2fcEJUCm1aqxjzbHdf7Wsugr4mLaA9YM2UVZ9Yy5meGv87VqHN3mmDeRwApTj1JKDaYjqmLZifSFdWCqBoghqaowwJ", signature)
        val pubKey = privateKey.getPublicKey(CoinType.TEZOS)
        assertTrue(TezosMessageSigner.verifyMessage(pubKey, msg, signature))
    }

    @Test
    fun testMessageSignerInputToPayload() {
        val payload = TezosMessageSigner.inputToPayload("Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World")
        val expected = "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64"
        assertEquals(expected, payload)
    }

    @Test
    fun testMessageSignerFormatMessage() {
        val formatedMsg = TezosMessageSigner.formatMessage("Hello World", "testUrl")
        val regex = Pattern.compile("Tezos Signed Message: \\S+ \\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{3}Z .+")
        assertTrue(regex.matcher(formatedMsg).matches())
    }
}
