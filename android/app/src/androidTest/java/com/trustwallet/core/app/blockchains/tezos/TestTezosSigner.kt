package com.trustwallet.core.app.blockchains.waves

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.CoinType.TEZOS
import wallet.core.java.AnySigner

class TestTezosTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
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
        val key = "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f".toHexByteArray()
        val result = AnySigner.signJSON(json, key, TEZOS.value())
        assertTrue(AnySigner.supportsJSON(TEZOS.value()))
        assertEquals(result, "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b957210001b86398d5b9be737dca8e4106ea18d70e69b75e92f892fb283546a99152b8d7794b919c0fbf1c31de386069a60014491c0e7505adef5781cead1cfe6608030b")
    }
}
