package com.trustwallet.core.app.blockchains.freecash

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.FreeCashAddress
import wallet.core.jni.FreeCashScript
import wallet.core.jni.FreeCashTransactionSigner
import wallet.core.jni.proto.FreeCash

class TestFreeCashSign {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignP2WPKH() {
        val signerBuilder = FreeCash.SigningInput.newBuilder()
            .setAmount(56700)
            .setHashType(0x01 or 0x40)
            .setToAddress("FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ")
            .setChangeAddress("FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ")
            .setByteFee(1)
            .setCoinType(CoinType.FREECASH.value())

        // Setting Private Keys

        val utxoKey0 =
            (Numeric.hexStringToByteArray("c775e7b757ede630cd0aa1113bd102661ab38829ca52a6422ab782862f268646"))
        signerBuilder.addPrivateKey(ByteString.copyFrom(utxoKey0))

        // Redeem scripts

        val outpoint0 = FreeCash.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(Numeric.hexStringToByteArray(getTxid("5f84d17d261f5c141086edf393ef4aeecf11a253d46360e05c5e70b51222bece"))))
            .setIndex(0)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        var hexString = Numeric.toHexString(FreeCashScript.buildForAddress("FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ", CoinType.FREECASH).data())

        val utxo0 = FreeCash.UnspentTransaction.newBuilder()
            .setAmount(1000000000)
            .setOutPoint(outpoint0)
            .setScript(ByteString.copyFrom(hexString.toHexBytes()))
            .build()

        signerBuilder.addUtxo(utxo0)


        val signer = FreeCashTransactionSigner(signerBuilder.build())
        val result = signer.sign()
        assert(result.success)
        assertEquals(1, result.objectsCount)

        val output = result.getObjects(0).unpack(wallet.core.jni.proto.FreeCash.SigningOutput::class.java)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(1, signedTransaction.version)
        assertEquals(1, signedTransaction.inputsCount)
        assertEquals(2, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x0100000001fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100b6006eb0fe2da8cbbd204f702b1ffdb1e29c49f3de51c4983d420bf9f9125635022032a195b153ccb2c4978333b4aad72aaa7e6a0b334a14621d5d817a42489cb0d301ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088acaefd3c11000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000",
            Numeric.toHexString(encoded.toByteArray()));
    }


    fun getTxid(id: String?): String {
        if (null == id || id.isEmpty()) {
            return ""
        }
        val stringBuffer = StringBuffer()
        var i = id.length - 1
        while (i > 0) {
            stringBuffer.append(id.substring(i - 1, i + 1))
            i = i - 2

        }
        Log.e("BitcoinUtil", "origin:" + id + "_result:" + stringBuffer.toString())
        return stringBuffer.toString()

    }

}