package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.EthereumSigner
import wallet.core.jni.proto.Ethereum
import com.trustwallet.core.app.utils.Numeric

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

        val sign: Ethereum.SigningOutput = EthereumSigner.sign(signingInput.build())

        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x25")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }
}