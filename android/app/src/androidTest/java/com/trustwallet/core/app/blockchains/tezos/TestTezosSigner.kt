package com.trustwallet.core.app.blockchains.tezos

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.CoinType.TEZOS
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Tezos.*

class TestTezosTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningFA2() {
        val key =
            "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6".toHexBytesInByteString()

        val transferInfos = Txs.newBuilder()
            .setAmount("10")
            .setTokenId("0")
            .setTo("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .build()

        val txObj = TxObject.newBuilder()
            .setFrom("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .addTxs(transferInfos)
            .build()

        val fa12 = FA2Parameters.newBuilder()
            .setEntrypoint("transfer")
            .addTxsObject(txObj)
            .build()

        val parameters = OperationParameters.newBuilder()
            .setFa2Parameters(fa12)
            .build()

        val transactionData = TransactionOperationData.newBuilder()
            .setAmount(0)
            .setDestination("KT1DYk1XDzHredJq1EyNkDindiWDqZyekXGj")
            .setParameters(parameters)
            .build()

        val transaction = Operation.newBuilder()
            .setSource("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .setFee(100000)
            .setCounter(2993173)
            .setGasLimit(100000)
            .setStorageLimit(0)
            .setKind(Operation.OperationKind.TRANSACTION)
            .setTransactionOperationData(transactionData)
            .build();

        val operationList = OperationList.newBuilder()
            .setBranch("BKvEAX9HXfJZWYfTQbR1C7B3ADoKY6a1aKVRF7qQqvc9hS8Rr3m")
            .addOperations(transaction)
            .build();

        val signingInput = SigningInput.newBuilder()
            .setPrivateKey(key)
            .setOperationList(operationList)
            .build()

        val result = AnySigner.sign(signingInput, TEZOS, SigningOutput.parser())

        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.encoded.toByteArray())),
            "1b1f9345dc9f77bd24b09034d1d2f9a28f02ac837f49db54b8d68341f53dc4b76c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a552d24710d6c59383286700c6c2917b25a6c1fa8b587e593c289dd47704278796792f1e522c1623845ec991e292b0935445e6994850bd03f035a006c5ed93806"
        )
    }

    @Test
    fun testSigningFA12() {
        val key =
            "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6".toHexBytesInByteString()

        val fa12 = FA12Parameters.newBuilder()
            .setEntrypoint("transfer")
            .setFrom("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .setTo("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .setValue("123")
            .build()

        val parameters = OperationParameters.newBuilder()
            .setFa12Parameters(fa12)
            .build()

        val transactionData = TransactionOperationData.newBuilder()
            .setAmount(0)
            .setDestination("KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5")
            .setParameters(parameters)
            .build()

        val transaction = Operation.newBuilder()
            .setSource("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP")
            .setFee(100000)
            .setCounter(2993172)
            .setGasLimit(100000)
            .setStorageLimit(0)
            .setKind(Operation.OperationKind.TRANSACTION)
            .setTransactionOperationData(transactionData)
            .build();

        val operationList = OperationList.newBuilder()
            .setBranch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp")
            .addOperations(transaction)
            .build();

        val signingInput = SigningInput.newBuilder()
            .setPrivateKey(key)
            .setOperationList(operationList)
            .build()

        val result = AnySigner.sign(signingInput, TEZOS, SigningOutput.parser())

        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.encoded.toByteArray())),
            "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb012914d768155fba2df319a81136e8e3e573b9cadb1676834490c90212615d271da029b6b0531e290e9063bcdb40bea43627af048b18e036f02be2b6b22fc8b307"
        )
    }

    @Test
    fun testSigningJSON() {
        val json = """
        {
            "operationList": {
                "branch": "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp",
                "operations": [{
                    "source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW",
                    "fee": 1272,
                    "counter": 30738,
                    "gasLimit": 10100,
                    "storageLimit": 257,
                    "kind": 107,
                    "revealOperationData": {
                        "publicKey": "QpqYbIBypAofOj4qtaWBm7Gy+2mZPFAEg3gVudxVkj4="
                    }
                }, {
                    "source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW",
                    "fee": 1272,
                    "counter": 30739,
                    "gasLimit": 10100,
                    "storageLimit": 257,
                    "kind": 108,
                    "transactionOperationData": {
                        "destination": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW",
                        "amount": 1
                    }
                }]
            }
        }
        """
        val key =
            "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f".toHexByteArray()
        val result = AnySigner.signJSON(json, key, TEZOS.value())
        assertTrue(AnySigner.supportsJSON(TEZOS.value()))
        assertEquals(
            result,
            "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b957210001b86398d5b9be737dca8e4106ea18d70e69b75e92f892fb283546a99152b8d7794b919c0fbf1c31de386069a60014491c0e7505adef5781cead1cfe6608030b"
        )
    }
}
