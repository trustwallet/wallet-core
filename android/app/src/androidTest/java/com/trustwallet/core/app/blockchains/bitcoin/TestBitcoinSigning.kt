package com.trustwallet.core.app.blockchains.bitcoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import wallet.core.jni.BitcoinTransactionSigner
import wallet.core.jni.proto.Bitcoin
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.Curve
import wallet.core.jni.Purpose

class TestBitcoinSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignP2WPKH() {
        val signerBuilder = Bitcoin.SigningInput.newBuilder()
            .setAmount(335_790_000)
            .setHashType(0x01)
            .setToAddress("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx")
            .setChangeAddress("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU")
            .setByteFee(1)

        // Setting Private Keys

        val utxoKey0 =
            (Numeric.hexStringToByteArray("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866"))
        signerBuilder.addPrivateKey(ByteString.copyFrom(utxoKey0))
        val utxoKey1 =
            (Numeric.hexStringToByteArray("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9"))
        signerBuilder.addPrivateKey(ByteString.copyFrom(utxoKey1))


        // Redeem scripts

        val outpoint0 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(Numeric.hexStringToByteArray("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f")))
            .setIndex(0)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo0 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(625_000_000)
            .setOutPoint(outpoint0)
            .setScript(ByteString.copyFrom("2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac".toHexBytes()))
            .build()

        signerBuilder.addUtxo(utxo0)


        val outpoint1 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom("ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a".toHexBytes()))
            .setIndex(1)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo1 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(600_000_000)
            .setOutPoint(outpoint1)
            .setScript(ByteString.copyFrom(Numeric.hexStringToByteArray("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1")))
            .build()

        signerBuilder.addUtxo(utxo1)

        val signer = BitcoinTransactionSigner(signerBuilder.build())
        val result = signer.sign()
        assert(result.success)
        assertEquals(1, result.objectsCount)

        val output = result.getObjects(0).unpack(wallet.core.jni.proto.Bitcoin.SigningOutput::class.java)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(1, signedTransaction.version)
        assertEquals(1, signedTransaction.inputsCount)
        assertEquals(2, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x0100000001fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100b6006eb0fe2da8cbbd204f702b1ffdb1e29c49f3de51c4983d420bf9f9125635022032a195b153ccb2c4978333b4aad72aaa7e6a0b334a14621d5d817a42489cb0d301ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088acaefd3c11000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000",
            Numeric.toHexString(encoded.toByteArray()));
    }


    @Test
    fun testSignP2PKH() {
        val signerBuilder = Bitcoin.SigningInput.newBuilder()
            .setAmount(55_000)
            .setHashType(0x01)
            .setToAddress("1GDCMHsTLBkawQXP8dqcZtr8zGgb4XpCug")
            .setChangeAddress("1CSR6tXqngr1CfwVF23V4bQotttJmzXqpf")
            .setByteFee(10)

        // Setting Private Keys

        val utxoKey0 =
            (Numeric.hexStringToByteArray("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866"))
        signerBuilder.addPrivateKey(ByteString.copyFrom(utxoKey0))
        val utxoKey1 =
            (Numeric.hexStringToByteArray("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9"))
        signerBuilder.addPrivateKey(ByteString.copyFrom(utxoKey1))


        // Redeem scripts

        val outpoint0 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(Numeric.hexStringToByteArray("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f")))
            .setIndex(0)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo0 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(30_000)
            .setOutPoint(outpoint0)
            .setScript(ByteString.copyFrom("2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac".toHexBytes()))
            .build()

        signerBuilder.addUtxo(utxo0)

        val outpoint1 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom("ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a".toHexBytes()))
            .setIndex(1)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo1 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(45_000)
            .setOutPoint(outpoint1)
            .setScript(ByteString.copyFrom(Numeric.hexStringToByteArray("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1")))
            .build()

        signerBuilder.addUtxo(utxo1)

        val signer = BitcoinTransactionSigner(signerBuilder.build())
        val result = signer.sign()
        assert(result.success)
        assertEquals(1, result.objectsCount)

        val output = result.getObjects(0).unpack(Bitcoin.SigningOutput::class.java)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(1, signedTransaction.version)
        assertEquals(2, signedTransaction.inputsCount)
        assertEquals(2, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x01000000000102fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100991ea84c8f22cbcbdee114a687b31bc80fca181161adc354e37b16b0f4664a6f022016e34b232524a1296a636026f8bb1f5f3635d88bf936532aae70a499c52f77d201ffffffffef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff02d8d60000000000001976a914a6d85a488bb777a540f24bf777d30d1486036f6188ac843f0000000000001976a9147d77e6cfb05a9cfc123824279f6caf8b66ac267688ac000247304402200ebd8fe637d7344984dd173d4a3089c4fc03a51117ee0363d04c714f033b33cf02204e2831939fff068068cc08fe35d84950f244fd2fe39795d839bfb8795484cc230121025476c2e83188368da1ff3e292e7acafcdb3566bb0ad253f62fc70f07aeee635700000000",
            Numeric.toHexString(encoded.toByteArray()));
    }
}