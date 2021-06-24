package com.trustwallet.core.app.blockchains.top

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.TOP
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Top
import wallet.core.jni.proto.Top.SigningOutput

class TestTopAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val privateKey = PrivateKey("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(publicKey, CoinType.TOP)

        assertEquals(address.description(), "T80000968927100f3cb7b23e8d477298311648978d8613")
    }

    @Test
    fun testAddressFromString() {
        val addressString = "T80000968927100f3cb7b23e8d477298311648978d8613"
        val address = AnyAddress(addressString, CoinType.TOP)
        assertEquals(address.description(), addressString)
    }

    @Test
    fun aeternityTransactionSigning() {
        val signingInput = Top.SigningInput.newBuilder()
            .setFrom("T0000066ab344963eaa071f9636faac26b0d1a39900325")
            .setTo("T0000066ab344963eaa071f9636faac26b0d1a39900325")
            .setAmount(1)
            .setTxDeposit(2)
            .setLastTxNonce(3)
            .setLastTxHash("12345678")
            .setToken("hello world")
            .setNote("top unit test")
            .setPrivateKey("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62".toHexBytesInByteString())
            .build()

        val output = AnySigner.sign(signingInput, TOP, SigningOutput.parser())

        assertEquals(
            "0xf8d49466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b8820488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100",
            output.encoded.toByteArray().toHex()
        )
        assertEquals(
            "0x0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100",
            output.signature.toByteArray().toHex()
        )
    }
}
