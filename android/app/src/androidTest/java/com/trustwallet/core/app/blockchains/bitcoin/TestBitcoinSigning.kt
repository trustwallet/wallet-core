package com.trustwallet.core.app.blockchains.bitcoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinScript
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.BITCOIN
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.Bitcoin.SigningOutput
import wallet.core.jni.proto.Common.SigningError

class TestBitcoinSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignP2WPKH() {
        val input = Bitcoin.SigningInput.newBuilder()
            .setAmount(335_790_000)
            .setHashType(BitcoinScript.hashTypeForCoin(CoinType.BITCOIN))
            .setToAddress("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx")
            .setChangeAddress("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU")
            .setByteFee(1)

        // Setting Private Keys

        val utxoKey0 =
            (Numeric.hexStringToByteArray("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey0))
        val utxoKey1 =
            (Numeric.hexStringToByteArray("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey1))


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

        input.addUtxo(utxo0)


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

        input.addUtxo(utxo1)

        val output = AnySigner.sign(input.build(), BITCOIN, SigningOutput.parser())
        assertEquals(output.error, SigningError.OK)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(1, signedTransaction.version)
        assertEquals(1, signedTransaction.inputsCount)
        assertEquals(2, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x0100000001fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100c327babdd370f0fc5b24cf920736446bf7d9c5660e4a5f7df432386fd652fe280220269c4fc3690c1c248e50c8bf2435c20b4ef00f308b403575f4437f862a91c53a01ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088acd0fd3c11000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000",
            Numeric.toHexString(encoded.toByteArray()));
    }


    @Test
    fun testSignP2PKH() {
        val input = Bitcoin.SigningInput.newBuilder()
            .setAmount(55_000)
            .setHashType(BitcoinScript.hashTypeForCoin(CoinType.BITCOIN))
            .setToAddress("1GDCMHsTLBkawQXP8dqcZtr8zGgb4XpCug")
            .setChangeAddress("1CSR6tXqngr1CfwVF23V4bQotttJmzXqpf")
            .setByteFee(10)

        // Setting Private Keys

        val utxoKey0 =
            (Numeric.hexStringToByteArray("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey0))
        val utxoKey1 =
            (Numeric.hexStringToByteArray("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey1))


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

        input.addUtxo(utxo0)

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

        input.addUtxo(utxo1)

        // Calculate fee (plan a transaction)
        val plan = AnySigner.plan(input.build(), BITCOIN, Bitcoin.TransactionPlan.parser())
        assertEquals(55_000, plan.amount)
        assertEquals(75_000, plan.availableAmount)
        assertEquals(2610, plan.fee)
        assertEquals(17390, plan.change)

        // Set the precomputed plan
        input.plan = plan
        val output = AnySigner.sign(input.build(), BITCOIN, SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(1, signedTransaction.version)
        assertEquals(2, signedTransaction.inputsCount)
        assertEquals(2, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x01000000000102fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f00000000494830450221008d49c4d7cc5ab93c01a67ce3f4ed2c45c59d4da6c76c891a9b56e67eda2e8cb4022078849134c697b1c70c1a19b900d94d8cab00ad7bcc8afe7ad1f6b184c13effa601ffffffffef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff02d8d60000000000001976a914a6d85a488bb777a540f24bf777d30d1486036f6188acee430000000000001976a9147d77e6cfb05a9cfc123824279f6caf8b66ac267688ac0002473044022074573d7f7828ae193fbea6d72c0fe2df6cee5c02bf455ea3d9312e16d6a9576502203861c5a3b3a83d4fe372034073f60201a8a944fb4536be0ea7544ab177b967600121025476c2e83188368da1ff3e292e7acafcdb3566bb0ad253f62fc70f07aeee635700000000",
            Numeric.toHexString(encoded.toByteArray()));
    }

    @Test
    fun testSignBrc20Commit() {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
        val privateKeyData = (Numeric.hexStringToByteArray("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129"))
        val fullAmount = 26400
        val minerFee = 3000
        val brcInscribeAmount = 7000
        val forFeeAmount = fullAmount - brcInscribeAmount - minerFee
        val txId = Numeric.hexStringToByteArray("089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e")
        val ticker = "oadf"
        val amount = "20"

        val privateKey = PrivateKey(privateKeyData)
        val publicKey = privateKey.getPublicKeySecp256k1(true)
        val pubKeyHash = Hash.ripemd(Hash.sha256(publicKey.data()))

        val p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(pubKeyHash)
        val outputInscribe = BitcoinScript.buildBRC20InscribeTransfer(ticker, amount, publicKey.data())
        val outputInscribeProto = Bitcoin.TransactionOutput.parseFrom(outputInscribe)

        val input = Bitcoin.SigningInput.newBuilder()
            .setIsItBrcOperation(true)
            .addPrivateKey(ByteString.copyFrom(privateKeyData))

        val unspentOutputPoint = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(txId))
            .setIndex(1)
            .build()

        val utxo0 = Bitcoin.UnspentTransaction.newBuilder()
            .setScript(ByteString.copyFrom(p2wpkh.data()))
            .setAmount(fullAmount.toLong())
            .setVariant(Bitcoin.TransactionVariant.P2WPKH)
            .setOutPoint(unspentOutputPoint)
            .build()
        input.addUtxo(utxo0)

        val utxoPlan0 = Bitcoin.UnspentTransaction.newBuilder()
            .setScript(outputInscribeProto.script)
            .setAmount(brcInscribeAmount.toLong())
            .setVariant(Bitcoin.TransactionVariant.BRC20TRANSFER)
            .build()
        val utxoPlan1 = Bitcoin.UnspentTransaction.newBuilder()
            .setScript(ByteString.copyFrom(p2wpkh.data()))
            .setAmount(forFeeAmount.toLong())
            .setVariant(Bitcoin.TransactionVariant.P2WPKH)
            .build()

        val plan = Bitcoin.TransactionPlan.newBuilder()
            .addUtxos(utxoPlan0)
            .addUtxos(utxoPlan1)
            .build()
        input.plan = plan

        val output = AnySigner.sign(input.build(), BITCOIN, SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        assertEquals(output.transactionId, "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000")

        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
    }
}
