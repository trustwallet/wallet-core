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
import wallet.core.jni.EthereumAbi
import wallet.core.jni.EthereumAbiFunction
import wallet.core.jni.proto.Ethereum.SigningOutput
import wallet.core.jni.proto.Ethereum.TransactionMode
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
            // txMode not set, Legacy is the default
            gasPrice = ByteString.copyFrom("0x04a817c800".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x0de0b6b3a7640000".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    @Test
    fun testEthereumERC20Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            // txMode not set, Legacy is the default
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

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
        assertEquals(Numeric.toHexString(output.data.toByteArray()), "0xa9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000")
    }

    @Test
    fun testEthereumERC20_1559_Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            txMode = TransactionMode.Enveloped
            maxInclusionFeePerGas = ByteString.copyFrom("0x77359400".toHexByteArray()) // 2000000000
            maxFeePerGas = ByteString.copyFrom("0xB2D05E00".toHexByteArray()) // 3000000000
            gasLimit = ByteString.copyFrom("0x0130B9".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                erc20Transfer = Ethereum.Transaction.ERC20Transfer.newBuilder().apply {
                    to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    amount = ByteString.copyFrom("0x1bc16d674ec80000".toHexByteArray())       
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x02f8b00180847735940084b2d05e00830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000c080a0adfcfdf98d4ed35a8967a0c1d78b42adb7c5d831cf5a3272654ec8f8bcd7be2ea011641e065684f6aa476f4fd250aa46cd0b44eccdb0a6e1650d658d1998684cdf")
    }

    @Test
    fun testEthereumERC721Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x0d8c864DA1985525e0af0acBEEF6562881827bd5"
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x02de".toHexByteArray())
            // txMode not set, Legacy is the default
            gasPrice = ByteString.copyFrom("0x22ecb25c00".toHexByteArray()) // 150 Gwei
            gasLimit = ByteString.copyFrom("0x0130b9".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                erc721Transfer = Ethereum.Transaction.ERC721Transfer.newBuilder().apply {
                    from = "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1"
                    to = "0x47331175b23C2f067204B506CA1501c26731C990"
                    tokenId = ByteString.copyFrom("0x0fd8".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf8cc8202de8522ecb25c00830130b9940d8c864da1985525e0af0acbeef6562881827bd580b86423b872dd0000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f100000000000000000000000047331175b23c2f067204b506ca1501c26731c9900000000000000000000000000000000000000000000000000000000000000fd825a04c5d8242a8c2db1cfa352a3486dd85c82824e01b9bcf0ce4170fcd2329fb7bcaa02d85ab09e750a73fd4dd26b142830ada1e991f8474795b43d96d93e65caaefe7")
        assertEquals(Numeric.toHexString(output.data.toByteArray()), "0x23b872dd0000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f100000000000000000000000047331175b23c2f067204b506ca1501c26731c9900000000000000000000000000000000000000000000000000000000000000fd8")
    }

    @Test
    fun testEthereumERC1155Signing() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151".toHexByteArray()).data())
            toAddress = "0x4e45e92ed38f885d39a733c14f1817217a89d425" // contract
            chainId = ByteString.copyFrom("0x01".toHexByteArray())
            nonce = ByteString.copyFrom("0x00".toHexByteArray())
            // txMode not set, Legacy is the default
            gasPrice = ByteString.copyFrom("0x09C7652400".toHexByteArray()) // 42000000000
            gasLimit = ByteString.copyFrom("0x0130b9".toHexByteArray()) // 78009
            transaction = Ethereum.Transaction.newBuilder().apply {
                erc1155Transfer = Ethereum.Transaction.ERC1155Transfer.newBuilder().apply {
                    from = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc"
                    to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    tokenId = ByteString.copyFrom("0x23c47ee5".toHexByteArray())
                    value = ByteString.copyFrom("0x1BC16D674EC80000".toHexByteArray()) // 2000000000000000000
                    data = ByteString.copyFrom("0x01020304".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf9014a808509c7652400830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b8e4f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000004010203040000000000000000000000000000000000000000000000000000000026a010315488201ac801ce346bffd1570de147615462d7e7db3cf08cf558465c6b79a06643943b24593bc3904a9fda63bb169881730994c973ab80f07d66a698064573")
        assertEquals(Numeric.toHexString(output.data.toByteArray()), "0xf242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000")
    }

    @Test
    fun testEthereumStakeRocketPool() {
        val function = EthereumAbiFunction("deposit")
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            chainId = ByteString.copyFrom("01".toHexByteArray())
            nonce = ByteString.copyFrom("01".toHexByteArray())
            txMode = TransactionMode.Enveloped
            gasPrice = ByteString.copyFrom("77541880".toHexByteArray()) // 2002000000
            gasLimit = ByteString.copyFrom("0320c8".toHexByteArray()) // 205000
            maxFeePerGas = ByteString.copyFrom("067ef83700".toHexByteArray()) // 27900000000
            maxInclusionFeePerGas = ByteString.copyFrom("3b9aca00".toHexByteArray()) // 1000000000
            toAddress = "0x2cac916b2a963bf162f076c0a8a4a8200bcfbfb4" // contract
            privateKey = ByteString.copyFrom(PrivateKey("9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498".toHexByteArray()).data())
            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("2386f26fc10000".toHexByteArray()) // 0.01 ETH
                    data = ByteString.copyFrom(EthereumAbi.encode(function))
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        // https://etherscan.io/tx/0xfeba0c579f3e964fbc4eafa500e86891b9f4113735b1364edd4433d765506f1e
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0xfb39e5079d7a0598ec45785d73a06b91fe1db707b9c6a150c87ffce2492c66d6")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x7fbd43a6f4733b2b4f98ad1bc4678ea2615f5edf56ad91408337adec2f07c0ac")
    }

    @Test
    fun testEthereumUnstakeRocketPool() {
        val function = EthereumAbiFunction("burn")
        function.addParamUInt256("0x21faa32ab2502b".toHexByteArray(), false)

        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            chainId = ByteString.copyFrom("01".toHexByteArray())
            nonce = ByteString.copyFrom("03".toHexByteArray())
            txMode = TransactionMode.Enveloped
            gasPrice = ByteString.copyFrom("77541880".toHexByteArray()) // 2002000000
            gasLimit = ByteString.copyFrom("055730".toHexByteArray()) // 350000
            maxFeePerGas = ByteString.copyFrom("067ef83700".toHexByteArray()) // 27900000000
            maxInclusionFeePerGas = ByteString.copyFrom("3b9aca00".toHexByteArray()) // 1000000000
            toAddress = "0xae78736Cd615f374D3085123A210448E74Fc6393" // contract
            privateKey = ByteString.copyFrom(PrivateKey("9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498".toHexByteArray()).data())
            transaction = Ethereum.Transaction.newBuilder().apply {
                contractGeneric = Ethereum.Transaction.ContractGeneric.newBuilder().apply {
                    amount = ByteString.copyFrom("00".toHexByteArray())
                    data = ByteString.copyFrom(EthereumAbi.encode(function))
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        // https://etherscan.io/tx/0x7fd3c0e9b8b309b4258baa7677c60f5e00e8db7b647fbe3a52adda25058a4b37
        assertEquals(Numeric.toHexString(output.r.toByteArray()), "0x1fc6e94908107584357799e952b4e3fb87f088aeb66d7930a7015643f19c9e7f")
        assertEquals(Numeric.toHexString(output.s.toByteArray()), "0x2c56a0b70ff2e52bf374a3dcd404bc42317d5ca15d319f5e33665352eb48f06f")
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

    // EIP4337
    @Test
    fun testEIP4337TransactionSigningAccountNotDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            toAddress = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0x5580".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x01952f1f85".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = false

                preVerificationGas = ByteString.copyFrom("0xbc18".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x073272".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x2386f26fc10000".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"21888\",\"initCode\":\"0x5a87209b755781cf65feeedd3855ade0317f4a925fbfb9cf00000000000000000000000078d9c32b96bb872d66d51818227563f44e67e2380000000000000000000000000000000000000000000000000000000000000000\",\"maxFeePerGas\":\"6797860741\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"48152\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x1560b19d17613ec8580cb0feaf7ac2953771404c5bd7830f585e5062e6ddd4b82ae3bb8dbddb659c0300e8009857b5c77501e1cfd5bbab48d03de0ea7207d07c1b\",\"verificationGasLimit\":\"471666\"}");
    }

    @Test
    fun testEIP4337TransactionSigningAccountDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0x1".toHexByteArray())

            toAddress = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0x9d55".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x1a339c9e9".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = true

                preVerificationGas = ByteString.copyFrom("0xb708".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x2386f26fc10000".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"40277\",\"initCode\":\"0x\",\"maxFeePerGas\":\"7033440745\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"1\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"46856\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xaed2011e5cf267de495b38ecf86ad6f1d4c05217a99e59f47e8d52ba3d41c10144785893fa3e7c116a054999e3902fc2771064d0545148bc49f6d7c827fc7a9a1c\",\"verificationGasLimit\":\"100000\"}");
    }

    @Test
    fun testEIP4337ERC20TransferAccountDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0x6".toHexByteArray())
            toAddress = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0xf78e".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x168ad5950f".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = true

                preVerificationGas = ByteString.copyFrom("0xbb10".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                erc20Transfer = Ethereum.Transaction.ERC20Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x186a0".toHexByteArray())
                    to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"6\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xd006c93d6a8753b5e7c1e6349de0dea34eab2e7a533106e0f2e1a3a3b013c8e97b007546dab9d7b8fc471ad14ff2e8aa351dc4f1ecb63bf20f33858dc7366cbe1c\",\"verificationGasLimit\":\"100000\"}");
    }

    @Test
    fun testEIP4337ERC20ApproveAccountDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0x9".toHexByteArray())

            toAddress = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0xf78e".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x168ad5950f".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = true

                preVerificationGas = ByteString.copyFrom("0xbb10".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                erc20Approve = Ethereum.Transaction.ERC20Approve.newBuilder().apply {
                    amount = ByteString.copyFrom("0x186a0".toHexByteArray())
                    spender = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"9\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x262a67dd8cf3d16a72b7809b3b5ed55e9f4c2b93eedd5a3c6be035fbbd7111164464ec933d0fdfa359e266e318f3ac22702ae428ce14fc142e4475603e6ec15e1c\",\"verificationGasLimit\":\"100000\"}");
    }

    @Test
    fun testEIP4337ERC721TransferAccountDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0xc".toHexByteArray())
            toAddress = "0xf5de760f2e916647fd766b4ad9e85ff943ce3a2b"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0x60B378".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x168ad5950f".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = true

                preVerificationGas = ByteString.copyFrom("0xC34F".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x16E360".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                erc721Transfer = Ethereum.Transaction.ERC721Transfer.newBuilder().apply {
                    tokenId = ByteString.copyFrom("0x2A8E57".toHexByteArray())
                    from = "0x8cE23B8769ac01d0df0d5f47Be1A38FeA97F3879"
                    to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f6000000000000000000000000f5de760f2e916647fd766b4ad9e85ff943ce3a2b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000006423b872dd0000000000000000000000008ce23b8769ac01d0df0d5f47be1a38fea97f3879000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000002a8e5700000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"12\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"49999\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x5951cc161a4d60d6b59503efb93e446f5d1a2e3a41d4503ba6393bcf2a2637340d0a865ed5d4d7650a68cbb95915eaa7ed54fd2c42b4bf7c83376f5c5d70691d1b\",\"verificationGasLimit\":\"1500000\"}");
    }

    @Test
    fun testEIP4337ERC1155TransferAccountDeployed() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x5".toHexByteArray())
            nonce = ByteString.copyFrom("0x0".toHexByteArray())
            toAddress = "0x428ce4b916332e1afccfddce08baecc97cb40b12"
            txMode = TransactionMode.UserOp

            gasLimit = ByteString.copyFrom("0x60B378".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x168ad5950f".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x0f".toHexByteArray())

            userOperation = Ethereum.UserOperation.newBuilder().apply {
                entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                accountFactory = "0x76627b8D1E01fAF0C73B69625BC1fCb8FA19a2AD"
                accountLogic = "0x510ab68bd111ce7115df797118b0334d727d564b"
                owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                isAccountDeployed = true

                preVerificationGas = ByteString.copyFrom("0xC738".toHexByteArray())
                verificationGasLimit = ByteString.copyFrom("0x16E360".toHexByteArray())
            }.build()

            transaction = Ethereum.Transaction.newBuilder().apply {
                erc1155Transfer = Ethereum.Transaction.ERC1155Transfer.newBuilder().apply {
                    tokenId = ByteString.copyFrom("0x01".toHexByteArray())
                    from = "0x8c560E00680b973645900528EDe71a99b8d4dca8"
                    to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(output.encoded.toStringUtf8(), "{\"callData\":\"0xb61d27f6000000000000000000000000428ce4b916332e1afccfddce08baecc97cb40b120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000c4f242432a0000000000000000000000008c560e00680b973645900528ede71a99b8d4dca8000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a0000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"51000\",\"sender\":\"0x8c560e00680b973645900528ede71a99b8d4dca8\",\"signature\":\"0xaae38bcf9f946921541b44c2a66596968beecb9420471e2c9c531f758a2d652930ffdeeab95742e57e8520fb5c8ca4fee6a8e47e37336d4201fe104103f85e111c\",\"verificationGasLimit\":\"1500000\"}");
    }
}
