package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.EthereumAbiStruct

class TestEthereumAbiStruct {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAbiStructHashStruct() {
        val hash = EthereumAbiStruct.hashStruct("Mail", 
        """
            {
                "from": {
                    "name": "Cow",
                    "wallets": [
                        "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
                        "DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
                    ]
                },
                "to": [
                    {
                        "name": "Bob",
                        "wallets": [
                            "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
                            "B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57",
                            "B0B0b0b0b0b0B000000000000000000000000000"
                        ]
                    }
                ],
                "contents": "Hello, Bob!"
            }
        """,
        """
            [
                {
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallets", "type": "address[]"}
                    ]
                },
                {
                    "Mail": [
                        {"name": "from", "type": "Person"},
                        {"name": "to", "type": "Person[]"},
                        {"name": "contents", "type": "string"}
                    ]
                }
            ]
        """)
        assertEquals(Numeric.toHexString(hash), "0xeb4221181ff3f1a83ea7313993ca9218496e424604ba9492bb4052c03d5c3df8")
    }
}
