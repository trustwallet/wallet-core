package com.trustwallet.core.app.blockchains.nebulas

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.NebulasSigner
import wallet.core.jni.proto.Nebulas

class TestNebulasSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testNebulasSigner() {
        val signingInput = Nebulas.SigningInput.newBuilder()
        signingInput.apply {
            fromAddress = "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"
            toAddress = "n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17"
            chainId = ByteString.copyFrom("0x1".toHexByteArray())       //1
            nonce = ByteString.copyFrom("0x7".toHexByteArray())         //7
            gasPrice = ByteString.copyFrom("0x0f4240".toHexByteArray()) //1000000
            gasLimit = ByteString.copyFrom("0x030d40".toHexByteArray()) //200000
            amount = ByteString.copyFrom("0x98a7d9b8314c0000".toHexByteArray())    //11000000000000000000
            payload = ""
            timestamp = ByteString.copyFrom("0x5cfc84ca".toHexByteArray())   //1560052938
            privateKey = ByteString.copyFrom(PrivateKey("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b".toHexByteArray()).data())
        }

        val output: Nebulas.SigningOutput = NebulasSigner.sign(signingInput.build())

        assertEquals(output.algorithm, 1)
        assertEquals(output.signature.toByteArray().toHex(), 
            "0xf53f4a9141ff8e462b094138eccd8c3a5d7865f9e9ab509626c78460a9e0b0fc35f7ed5ba1795ceb81a5e46b7580a6f7fb431d44fdba92515399cf6a8e47e71500")
        assertEquals(output.raw, 
            "CiBQXdR2neMqnEu21q/U+OHqZHSBX9Q0hNiRfL2eCZO4hRIaGVefwtw23wEobqA40/7aIwQHghETxH4r+50aGhlXf89CeLWgHFjKu9/6tn4KNbelsMDAIIi2IhAAAAAAAAAAAJin2bgxTAAAKAcwyony5wU6CAoGYmluYXJ5QAFKEAAAAAAAAAAAAAAAAAAPQkBSEAAAAAAAAAAAAAAAAAADDUBYAWJB9T9KkUH/jkYrCUE47M2MOl14Zfnpq1CWJseEYKngsPw19+1boXlc64Gl5Gt1gKb3+0MdRP26klFTmc9qjkfnFQA=")
    }
}
