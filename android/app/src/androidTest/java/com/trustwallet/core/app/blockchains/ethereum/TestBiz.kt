package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.CoinType.ETHEREUM
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.proto.Ethereum.SigningOutput
import wallet.core.jni.proto.Ethereum.TransactionMode
import wallet.core.jni.Biz as WCBiz

class TestBiz {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    // https://bscscan.com/tx/0x425eb17a8e1dee2fcee8352a772d83cbb069c2e03f2c5d9d00da3b3ef66ce48b
    @Test
    fun testSignEip7702EoaBatched() {
        val transferFunc1 = EthereumAbiFunction("transfer")
        transferFunc1.addParamAddress("0x2EF648D7C03412B832726fd4683E2625deA047Ba".toHexByteArray(), false)
        // 100_000_000_000_000
        transferFunc1.addParamUInt256("0x5af3107a4000".toHexByteArray(), false)
        val transferPayload1 = EthereumAbi.encode(transferFunc1)

        val transferFunc2 = EthereumAbiFunction("transfer")
        transferFunc2.addParamAddress("0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e".toHexByteArray(), false)
        // 500_000_000_000_000
        transferFunc2.addParamUInt256("0x1c6bf52634000".toHexByteArray(), false)
        val transferPayload2 = EthereumAbi.encode(transferFunc2)

        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xe148e40f06ee3ba316cdb2571f33486cf879c0ffd2b279ce9f9a88c41ce962e7".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x38".toHexByteArray())
            nonce = ByteString.copyFrom("0x12".toHexByteArray())
            txMode = TransactionMode.SetCode

            gasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x3b9aca00".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x3b9aca00".toHexByteArray())

            transaction = Ethereum.Transaction.newBuilder().apply {
                scwBatch = Ethereum.Transaction.SCWalletBatch.newBuilder().apply {
                    walletType = Ethereum.SCWalletType.Biz
                    addAllCalls(listOf(
                        Ethereum.Transaction.SCWalletBatch.BatchedCall.newBuilder().apply {
                            // TWT
                            address = "0x4B0F1812e5Df2A09796481Ff14017e6005508003"
                            amount = ByteString.copyFrom("0x00".toHexByteArray())
                            payload = ByteString.copyFrom(transferPayload1)
                        }.build(),
                        Ethereum.Transaction.SCWalletBatch.BatchedCall.newBuilder().apply {
                            // TWT
                            address = "0x4B0F1812e5Df2A09796481Ff14017e6005508003"
                            amount = ByteString.copyFrom("0x00".toHexByteArray())
                            payload = ByteString.copyFrom(transferPayload2)
                        }.build()
                    ))
                }.build()
            }.build()

            eip7702Authorization = Ethereum.Authorization.newBuilder().apply {
                address = "0x117BC8454756456A0f83dbd130Bb94D793D3F3F7"
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.preHash.toByteArray()), "0x00b2d13719df301927ddcbdad5b6bc6214f2007c6408df883c9ea483b45e6f44")
        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x04f9030f3812843b9aca00843b9aca00830186a0945132829820b44dc3e8586cec926a16fca0a5608480b9024434fcd5be00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb0000000000000000000000002ef648d7c03412b832726fd4683e2625dea047ba00000000000000000000000000000000000000000000000000005af3107a4000000000000000000000000000000000000000000000000000000000000000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb00000000000000000000000095dc01ebd10b6dccf1cc329af1a3f73806117c2e0000000000000000000000000000000000000000000000000001c6bf5263400000000000000000000000000000000000000000000000000000000000c0f85cf85a3894117bc8454756456a0f83dbd130bb94d793d3f3f71380a0073afc661c158a2dccf4183f87e1e4d62b4d406af418cfd69959368ec9bec2a6a064292fd61d4d16b840470a86fc4f7a89413f9126d897f2268eb76a1d887c6d7a01a0e8bcbd96323c9d3e67b74366b2f43299100996d9e8874a6fd87186ac8f580d4ca07c25b4f0619af77fb953e8f0e4372bfbee62616ad419697516108eeb9bcebb28")
    }

    // https://bscscan.com/tx/0x6f8b2c8d50e8bb543d7124703b75d9e495832116a1a61afabf40b9b0ac43c980
    @Test
    fun testSignEnvelopedBiz() {
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xe762e91cc4889a9fce79b2d2ffc079f86c48331f57b2cd16a33bee060fe448e1".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x38".toHexByteArray())
            nonce = ByteString.copyFrom("0x02".toHexByteArray())
            txMode = TransactionMode.Enveloped

            gasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x3b9aca00".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x3b9aca00".toHexByteArray())

            transaction = Ethereum.Transaction.newBuilder().apply {
                scwExecute = Ethereum.Transaction.SCWalletExecute.newBuilder().apply {
                    walletType = Ethereum.SCWalletType.Biz
                    transaction = Ethereum.Transaction.newBuilder().apply {
                        erc20Transfer = Ethereum.Transaction.ERC20Transfer.newBuilder().apply {
                            to = "0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e"
                            amount = ByteString.copyFrom("0xb5e620f48000".toHexByteArray())
                        }.build()
                    }.build()
                }.build()
            }.build()

            // TWT token.
            toAddress = "0x4B0F1812e5Df2A09796481Ff14017e6005508003"
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.preHash.toByteArray()), "0x60260356568ae70838bd80085b971e1e4ebe42046688fd8511a268986e522121")
        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x02f901503802843b9aca00843b9aca00830186a0946e860086bba8fdeafb553815af0f09a854cc887a80b8e4b61d27f60000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb00000000000000000000000095dc01ebd10b6dccf1cc329af1a3f73806117c2e0000000000000000000000000000000000000000000000000000b5e620f4800000000000000000000000000000000000000000000000000000000000c080a0fb45762a262f4c32090576e9de087482d25cd00b6ea2522eb7d5a40f435acdbaa0151dbd48a4f4bf06080313775fe32ececd68869d721518a92bf292e4a84322f9")
    }

    @Test
    fun testBizTransferAccountDeployedV07() {
        val chainIdByteArray = "0x7A69".toHexByteArray() // 31337
        val wallet = "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029"

        val transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
            amount = ByteString.copyFrom("0x2386f26fc10000".toHexByteArray())
            data = ByteString.EMPTY
        }.build()

        val userOpV07 = Ethereum.UserOperationV0_7.newBuilder().apply {
            entryPoint = "0x0000000071727De22E5E9d8BAf0edAc6f37da032"
            sender = wallet
            preVerificationGas = ByteString.copyFrom("0xF4240".toHexByteArray())          // 1000000
            verificationGasLimit = ByteString.copyFrom("0x186A0".toHexByteArray())        // 100000
            factory = "0xf471789937856d80e589f5996cf8b0511ddd9de4"
            factoryData = ByteString.copyFrom("0xf471789937856d80e589f5996cf8b0511ddd9de4".toHexByteArray())
            paymaster = "0xf62849f9a0b5bf2913b396098f7c7019b51a820a"
            paymasterVerificationGasLimit = ByteString.copyFrom("0x1869F".toHexByteArray()) // 99999
            paymasterPostOpGasLimit = ByteString.copyFrom("0x15B38".toHexByteArray())      // 88888
            paymasterData = ByteString.copyFrom(
                "0x00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b"
                    .toHexByteArray()
            )
        }.build()

        // Create signing input
        val signingInput = Ethereum.SigningInput.newBuilder().apply {
            privateKey = ByteString.copyFrom(PrivateKey("3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483".toHexByteArray()).data())
            chainId = ByteString.copyFrom(chainIdByteArray)               // 31337
            nonce = ByteString.copyFrom("0x00".toHexByteArray())
            txMode = Ethereum.TransactionMode.UserOp
            gasLimit = ByteString.copyFrom("0x186A0".toHexByteArray())            // 100000
            maxFeePerGas = ByteString.copyFrom("0x186A0".toHexByteArray())        // 100000
            maxInclusionFeePerGas = ByteString.copyFrom("0x186A0".toHexByteArray())
            toAddress = "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9"

            transaction = Ethereum.Transaction.newBuilder().apply {
                scwExecute = Ethereum.Transaction.SCWalletExecute.newBuilder().apply {
                    transaction = Ethereum.Transaction.newBuilder().apply {
                        this.transfer = transfer
                    }.build()
                }.build()
            }.build()

            userOperationV07 = userOpV07
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.ETHEREUM, Ethereum.SigningOutput.parser())

        assertEquals(
            "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb",
            Numeric.toHexString(output.preHash.toByteArray())
        )

        val codeAddress = "0x2e234DAe75C793f67A35089C9d99245E1C58470b"
        val codeName = "Biz"
        val codeVersion = "v1.0.0"
        val typeHash = "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867"
        val domainSeparatorHash = "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472"
        val hash = "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb"

        val encodedHash = WCBiz.getEncodedHash(
            chainIdByteArray,
            codeAddress,
            codeName,
            codeVersion,
            typeHash,
            domainSeparatorHash,
            wallet,
            hash
        )
        assertEquals(
            "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635",
            Numeric.toHexString(encodedHash)
        )

        val privateKey = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8"
        val signedHash = WCBiz.getSignedHash(
            "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635",
            privateKey
        )
        assertEquals(
            "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c",
            Numeric.toHexString(signedHash)
        )
    }

    @Test
    fun testBizSignExecuteWithSignatureCall() {
        // Create ERC20 approve function call
        val approveFunc = EthereumAbiFunction("approve")
        approveFunc.addParamAddress("0xBC472b43BC237f733c78a581078F58A6a89c46Ec".toHexByteArray(), false)
        approveFunc.addParamUInt256("0x03e8".toHexByteArray(), false) // 1000
        val approvePayload = EthereumAbi.encode(approveFunc)

        // Build the ExecuteWithSignatureInput protobuf
        val input = wallet.core.jni.proto.Biz.ExecuteWithSignatureInput.newBuilder()
        input.apply {
            // Add execution
            addExecutions(wallet.core.jni.proto.Biz.Execution.newBuilder().apply {
                address = "0x4B0F1812e5Df2A09796481Ff14017e6005508003" // TWT token
                amount = ByteString.copyFrom("0x00".toHexByteArray())
                payload = ByteString.copyFrom(approvePayload)
            }.build())

            // Private key
            privateKey = ByteString.copyFrom("0xefec50f00ef0c09d967f3e363ee96502ce18a1881f6ac22321aa58071d43c66f".toHexByteArray())

            // Nonce
            nonce = ByteString.copyFrom("0x00".toHexByteArray())

            // Encoding hash params
            encodingHashParams = wallet.core.jni.proto.Biz.EncodingHashParams.newBuilder().apply {
                chainId = ByteString.copyFrom("0x38".toHexByteArray()) // 56 (BSC)
                codeAddress = "0xba083F0EeAF806603d31582D4e7667fB5A4A1B30"
                codeName = "Biz"
                codeVersion = "v1.0.0"
                typeHash = "0xec429430bbd6d0e373848272230d6fe2bac6319d903762e089c5cae97af53df0"
                domainSeparatorHash = "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472"
            }.build()
        }

        // Call the native function
        val encoded = WCBiz.signExecuteWithSignatureCall(input.build().toByteArray())

        // Verify the result
        val expected = "0x1d92e4b600000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000160000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000bc472b43bc237f733c78a581078f58a6a89c46ec00000000000000000000000000000000000000000000000000000000000003e8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000041db18e3a0150ddef964810e480b25592942a22d0b583f7d5cbb33ef6fb4baa66e753af78e967ee374070e16cf963a6cd7a3adb713e50d553aefbc361c48366a101b00000000000000000000000000000000000000000000000000000000000000"
        assertEquals(expected, Numeric.toHexString(encoded))
    }
}
