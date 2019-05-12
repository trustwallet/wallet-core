package com.trustwallet.core.app.blockchains.ark

import com.google.gson.Gson
import com.google.gson.annotations.SerializedName
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.ARKSigner
import wallet.core.jni.proto.ARK

class TestArkTransactionSigning{
    init {
        System.loadLibrary("TrustWalletCore")
    }
    @Test
    fun arkTransactionSigning(){
        val signingInput = ARK.SigningInput.newBuilder()
            .setAmount(1000000)
            .setPrivateKey("a76fa06ec56a720cdaa4b4764c7685833c7ffbcc03d193963c01d6890388376f".toHexBytesInByteString())
            .setTimestamp(67855156)
            .setRecipientId("AM5P17vdMPRscmyggkac6AgNp6CjUjLdwV")
            .setFee(1500000)
            .build()

        val output = ARKSigner.sign(signingInput)
        val arkTransaction = Gson().fromJson(output.json, ArkTransaction::class.java)

        assertEquals(arkTransaction.signature, "3045022100cbe6803267d41803deef08ff150600101b1233468e212883d56739b491b1eb240220357bfa607eb1c2e9858cc6a55446ce6fe7556da9c90e61f14d8d5f86f9d306a4")
        assertEquals(arkTransaction.senderPublicKey, "02ceb971ddabd391df62f4762ba45def3e2b36d002d8344ebbb4be9a941071c332")

    }

    data class ArkTransaction (
        @SerializedName("amount") val amount : Int,
        @SerializedName("asset") val asset : Asset,
        @SerializedName("fee") val fee : Int,
        @SerializedName("id") val id : String,
        @SerializedName("recipientId") val recipientId : String,
        @SerializedName("senderPublicKey") val senderPublicKey : String,
        @SerializedName("signature") val signature : String,
        @SerializedName("timestamp") val timestamp : Int,
        @SerializedName("type") val type : Int
    )

    class Asset
}