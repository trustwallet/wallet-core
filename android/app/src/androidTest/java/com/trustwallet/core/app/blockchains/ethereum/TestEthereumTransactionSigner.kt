package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.ETHEREUM
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.proto.Ethereum.SigningOutput
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertArrayEquals

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
            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x0de0b6b3a7640000".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())
        val encoded = AnySigner.encode(signingInput.build(), ETHEREUM)

        assertArrayEquals(output.encoded.toByteArray(), encoded)
        assertEquals(Numeric.toHexString(output.v.toByteArray()), "0x25")
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0x28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
        assertEquals(Numeric.toHexString(encoded), "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    @Test
    fun testEthereumERC20Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x09c7652400".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x0130B9".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                erc20Transfer = Ethereum.Transaction.ERC20Transfer.newBuilder().apply {
                    to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    amount = ByteString.copyFrom("0x1bc16d674ec80000".toHexByteArray())       
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())
        val encoded = AnySigner.encode(signingInput.build(), ETHEREUM)

        assertArrayEquals(output.encoded.toByteArray(), encoded)
        assertEquals(Numeric.toHexString(output.v.toByteArray()), "0x25")
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0x724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98ce")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
        assertEquals(Numeric.toHexString(encoded), "0xf8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
    }

    @Test
    fun testEthereumERC721Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x09c7652400".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x0130B9".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                erc721Transfer = Ethereum.Transaction.ERC721Transfer.newBuilder().apply {
                    from = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc"
                    to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    tokenId = ByteString.copyFrom("0x23c47ee5".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())
        val encoded = AnySigner.encode(signingInput.build(), ETHEREUM)

        assertArrayEquals(output.encoded.toByteArray(), encoded)
        assertEquals(Numeric.toHexString(output.v.toByteArray()), "0x26")
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0x4f35575c8dc6d0c12fd1ae0007a1395f2baa992d5d498f5ee381cdb7d46ed43c")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x0935b9ceb724ab73806e7f43da6a3079e7404e2dc28fe030fef96cd13779ac04")
        assertEquals(Numeric.toHexString(encoded), "0xf8b6808509c7652400830130b98080b86423b872dd000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee526a04f35575c8dc6d0c12fd1ae0007a1395f2baa992d5d498f5ee381cdb7d46ed43ca00935b9ceb724ab73806e7f43da6a3079e7404e2dc28fe030fef96cd13779ac04")
    }

    @Test
    fun testSignJSON() {
        val json = """
            {
                "chainId": "AQ==",
                "gasPrice": "1pOkAA==",
                "gasLimit": "Ugg=",
                "toAddress": "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1",
                "transaction": {
                    "transfer": {
                        "amount":"A0i8paFgAA=="
                    }
                }                
            }
        """
        val key = "17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543".toHexByteArray()
        val result = AnySigner.signJSON(json, key, CoinType.ETHEREUM.value())

        assertEquals("f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10", result)
    }

    @Test
    fun testEthereumTransactionDecoding() {
        val rawTx = "0xf8a86484b2d05e008277fb9400000000000c2e074ec69a0dfb2997ba6c7d2e1e80b8441896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba4125a0b55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4dfa077b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d".toHexByteArray()
        val decoded = AnySigner.decode(rawTx, ETHEREUM)

        assertEquals(String(decoded), """{"gas":"0x77fb","gasPrice":"0xb2d05e00","input":"0x1896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba41","nonce":"0x64","r":"0xb55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4df","s":"0x77b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d","to":"0x00000000000c2e074ec69a0dfb2997ba6c7d2e1e","v":"0x25","value":"0x"}""")
    }
}
