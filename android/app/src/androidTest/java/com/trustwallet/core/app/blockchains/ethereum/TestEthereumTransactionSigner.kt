package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.proto.Ethereum.SigningOutput
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertArrayEquals
import wallet.core.jni.CoinType.ETHEREUM

class TestEthereumTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumTransactionSigning() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x4646464646464646464646464646464646464646464646464646464646464646".toHexByteArray()).data())
            toAddress = "0x3535353535353535353535353535353535353535"
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x9".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x04a817c800".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            amount = ByteString.copyFrom("0x0de0b6b3a7640000".toHexByteArray())
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())
        val encoded = AnySigner.encode(signingInput.build(), ETHEREUM)

        assertArrayEquals(output.encoded.toByteArray(), encoded)
        assertEquals(Numeric.toHexString(output.v.toByteArray()), "0x25")
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0x28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
        assertEquals(Numeric.toHexString(encoded), "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    @Test
    fun testEthereumTransactionDecoding() {
        val rawTx = "0xf8a86484b2d05e008277fb9400000000000c2e074ec69a0dfb2997ba6c7d2e1e80b8441896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba4125a0b55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4dfa077b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d".toHexByteArray()
        val decoded = AnySigner.decode(rawTx, ETHEREUM)

        assertEquals(String(decoded), """{"gas":"0x77fb","gasPrice":"0xb2d05e00","input":"0x1896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba41","nonce":"0x64","r":"0xb55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4df","s":"0x77b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d","to":"0x00000000000c2e074ec69a0dfb2997ba6c7d2e1e","v":"0x25","value":"0x"}""")
    }
}
