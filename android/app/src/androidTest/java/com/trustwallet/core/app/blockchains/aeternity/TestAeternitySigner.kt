package com.trustwallet.core.app.blockchains.aeternity

import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.AeternitySigner
import wallet.core.jni.proto.Aeternity

class TestAeternitySigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun aeternityTransactionSigning() {
        val signingInput = Aeternity.SigningInput.newBuilder()
            .setFromAddress("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
            .setToAddress("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v")
            .setAmount(10L)
            .setFee(2e13.toLong())
            .setPayload("Hello World")
            .setTtl(82757)
            .setNonce(49)
            .setPrivateKey("4646464646464646464646464646464646464646464646464646464646464646".toHexBytesInByteString())
            .build()

        val output: Aeternity.SigningOutput = AeternitySigner.sign(signingInput)

        assertEquals(
            output.encoded,
            "tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg=="
        )
        assertEquals(
            output.signature,
            "sg_2dlw1eTrh79Yri5+urBSwVMJ86dSvCVtWc/nxIJrhIehxLzbtEdddjNyGJFc700p09KSd01oVTrpoCfiFsFvB3kDla0="
        )
    }
}
