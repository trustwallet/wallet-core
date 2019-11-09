package com.trustwallet.core.app.blockchains.aeternity

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
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
            .setAmount(ByteString.copyFrom("0a".toHexByteArray()))
            .setFee(ByteString.copyFrom("12309ce54000".toHexByteArray()))
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
            "sg_VW42qDPP3MMNFAStYaumjZz7mC7BZYpbNa15E57ejqUe7JdQFWCiX65eLNUpGMpt8tSpfgCfkYzcaFppqx7W75CrcWdC8"
        )
    }
}
