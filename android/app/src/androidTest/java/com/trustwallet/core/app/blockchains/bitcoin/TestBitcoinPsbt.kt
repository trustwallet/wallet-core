package com.trustwallet.core.app.blockchains.bitcoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinScript
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.BITCOIN
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.BitcoinV2
import wallet.core.jni.proto.Common.SigningError

class TestBitcoinPsbt {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignThorSwap() {
        // Successfully broadcasted tx: https://mempool.space/tx/634a416e82ac710166725f6a4090ac7b5db69687e86b2d2e38dcb3d91c956c32

        val privateKey = "f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55".toHexBytesInByteString()
        val psbt = "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000".toHexBytesInByteString()

        val input = BitcoinV2.SigningInput.newBuilder()
            .setPsbt(BitcoinV2.Psbt.newBuilder().setPsbt(psbt))
            .addPrivateKeys(privateKey)
            .build()

        val legacyInput = Bitcoin.SigningInput.newBuilder()
            .setSigningV2(input)
            .build()

        val legacyOutput = AnySigner.sign(legacyInput, BITCOIN, Bitcoin.SigningOutput.parser())
        val output = legacyOutput.signingResultV2

        assertEquals(output.error, SigningError.OK)
        assertEquals(
            output.psbt.psbt.toByteArray().toHex(),
            "0x70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d01086c02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000"
        )
        assertEquals(
            output.encoded.toByteArray().toHex(),
            "0x02000000000101147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000"
        )
        assertEquals(
            output.txid.toByteArray().toHex(),
            "0x634a416e82ac710166725f6a4090ac7b5db69687e86b2d2e38dcb3d91c956c32"
        )
    }

    @Test
    fun testPlanThorSwap() {
        // Successfully broadcasted tx: https://mempool.space/tx/634a416e82ac710166725f6a4090ac7b5db69687e86b2d2e38dcb3d91c956c32

        val privateKey = PrivateKey("f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55".toHexBytes())
        val publicKey = privateKey.getPublicKeySecp256k1(true)
        val psbt = "70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000".toHexBytesInByteString()

        val input = BitcoinV2.SigningInput.newBuilder()
            .setPsbt(BitcoinV2.Psbt.newBuilder().setPsbt(psbt))
            .addPublicKeys(ByteString.copyFrom(publicKey.data()))
            .build()

        val legacyInput = Bitcoin.SigningInput.newBuilder()
            .setSigningV2(input)
            .build()

        val legacyPlan = AnySigner.plan(legacyInput, BITCOIN, Bitcoin.TransactionPlan.parser())
        val plan = legacyPlan.planningResultV2

        assertEquals(plan.error, SigningError.OK)

        assertEquals(plan.getInputs(0).receiverAddress, "bc1qkyu3n8k8jmekl3pwvdl59k5w8enjp25akz2r3z")
        assertEquals(plan.getInputs(0).value, 66_406)

        // Vault transfer
        assertEquals(plan.getOutputs(0).toAddress, "bc1q7g48qdshqd000aysws74pun2uzxrp598gcfum0")
        assertEquals(plan.getOutputs(0).value, 60_000)

        // OP_RETURN
        assertEquals(
            plan.getOutputs(1).customScriptPubkey.toByteArray().toHex(),
            "0x6a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a3530"
        )
        assertEquals(plan.getOutputs(1).value, 0)

        // Change output
        assertEquals(plan.getOutputs(2).toAddress, "bc1qkyu3n8k8jmekl3pwvdl59k5w8enjp25akz2r3z")
        assertEquals(plan.getOutputs(2).value, 4_670)

        assertEquals(plan.feeEstimate, 1736)
        // Please note that `change` in PSBT planning is always 0.
        // That's because we aren't able to determine which output is an actual change from PSBT.
        assertEquals(plan.change, 0)
    }
}
