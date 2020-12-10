package com.trustwallet.core.app.blockchains.aeternity

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.AETERNITY
import wallet.core.jni.proto.Aeternity
import wallet.core.jni.proto.Aeternity.SigningOutput

class TestAeternitySigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun aeternityTransferTransactionSign() {
        val signingInput = Aeternity.SigningInput.newBuilder()
            .setFromAddress("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
            .setToAddress("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v")
            .setFee(ByteString.copyFrom("12309ce54000".toHexByteArray()))
            .setTtl(82757)
            .setNonce(49)
            .setPrivateKey("4646464646464646464646464646464646464646464646464646464646464646".toHexBytesInByteString())
            .setPayload(
                Aeternity.Payload.newBuilder()
                    .setPayloadTransfer(
                        Aeternity.Payload.Transfer.newBuilder()
                            .setAmount(ByteString.copyFrom("0a".toHexByteArray()))
                    )
            )
            .build()

        val output = AnySigner.sign(signingInput, AETERNITY, SigningOutput.parser())

        assertEquals(
            output.encoded,
            "tx_+J4LAfhCuEC1SpnmpRoEbMF9BABAePYkH7yNuar+WN8CTrmw0xwhqhsj9a/DxLcEM+gvWsgkr/SEJkbV0aoxR/ecLCsiV1UPuFb4VAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0UxgFv6iPQ="
        )
        assertEquals(
            output.signature,
            "sg_QiecgDJnxyeCeYb2fiEt8a3d74Q4NCGGrPHBSJCkr8cJ55VwtF6q8V6tc5cfWdArR6fQTvr9M1DpEny7kzoJPJjL5UR5F"
        )
    }

    @Test
    fun aeternityContractTransactionSign() {
        val signingInput = Aeternity.SigningInput.newBuilder()
            .setFromAddress("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
            .setToAddress("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v")
            .setFee(ByteString.copyFrom("12309ce54000".toHexByteArray()))
            .setTtl(82757)
            .setNonce(49)
            .setPrivateKey("4646464646464646464646464646464646464646464646464646464646464646".toHexBytesInByteString())
            .setPayload(
                Aeternity.Payload.newBuilder()
                    .setPayloadContractGeneric(
                        Aeternity.Payload.ContractGeneric.newBuilder()
                            .setPayload("Hello World")
                    )
            )
            .build()

        val output = AnySigner.sign(signingInput, AETERNITY, SigningOutput.parser())

        assertEquals(
            output.encoded,
            "tx_+KkLAfhCuEBAxVYM3tXXtZNrpF5oTQ7JCjviPN9k0sLuE24QlLXW+xp5BUhYTBNcVE5AQPBHxEEqtzQh+K+7i1vzhTylwtQBuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMAhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkooC1hg=="
        )
        assertEquals(
            output.signature,
            "sg_9UV6qNYu78uck4Ds3sEruDNnJKjvqF469S4C9rAvBBZer3FPwxuSbeLtEBH9ufJiup2NyudC5Hi5ZHkAXTm6NnsGm55dd"
        )
    }
}
