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
import wallet.core.jni.BizPasskeySession as WCBizPasskeySession

class TestBizPasskeySession {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testBizPasskeySessionEncodeRegisterSessionCall() {
        val publicKey = PublicKey(
            "0x041c05286fe694493eae33312f2d2e0d0abeda8db76238b7a204be1fb87f54ce4228fef61ef4ac300f631657635c28e59bfb2fe71bce1634c81c65642042f6dc4d".toHexByteArray(),
            PublicKeyType.NIST256P1EXTENDED
        )
        val validUntil = "0x15181" // 86_401
        val encoded = WCBizPasskeySession.encodeRegisterSessionCall(publicKey, Numeric.hexStringToByteArray(validUntil))
        assertEquals(
            "0x826491fb000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000151810000000000000000000000000000000000000000000000000000000000000041041c05286fe694493eae33312f2d2e0d0abeda8db76238b7a204be1fb87f54ce4228fef61ef4ac300f631657635c28e59bfb2fe71bce1634c81c65642042f6dc4d00000000000000000000000000000000000000000000000000000000000000",
            Numeric.toHexString(encoded)
        )
    }

    @Test
    fun testBizPasskeySessionEncodeRemoveSessionCall() {
        val publicKey = PublicKey(
            "0x041c05286fe694493eae33312f2d2e0d0abeda8db76238b7a204be1fb87f54ce4228fef61ef4ac300f631657635c28e59bfb2fe71bce1634c81c65642042f6dc4d".toHexByteArray(),
            PublicKeyType.NIST256P1EXTENDED
        )
        val encoded = WCBizPasskeySession.encodeRemoveSessionCall(publicKey)
        assertEquals(
            "0xe1c06abd00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000041041c05286fe694493eae33312f2d2e0d0abeda8db76238b7a204be1fb87f54ce4228fef61ef4ac300f631657635c28e59bfb2fe71bce1634c81c65642042f6dc4d00000000000000000000000000000000000000000000000000000000000000",
            Numeric.toHexString(encoded)
        )
    }

    @Test
    fun testBizPasskeySessionEncodePasskeyNonce() {
        val nonce = "0x7b" // 123
        val passkeyNonce = WCBizPasskeySession.encodePasskeySessionNonce(Numeric.hexStringToByteArray(nonce))
        assertEquals(
            "0x00000000000000000000000000000000050041d6a66939a8000000000000007b",
            Numeric.toHexString(passkeyNonce)
        )
    }

    @Test
    fun testSignUserOperationV7WithPasskeySession() {
        val chainIdByteArray = "0x7A69".toHexByteArray() // 31337
        val wallet = "0x336Cd992a83242D91f556C1F7e855AcA366193e0"
        val bizPasskeySessionAccount = "0xa0Cb889707d426A7A386870A03bc70d1b0697598"
        val bizPasskeySessionCodeName = "PasskeySession"
        val codeVersion = "v1.0.0"
        // keccak("PasskeySession(bytes32 userOpHash)")
        val typeHash = "0x3463fe66e4d03af5b942aebde2b191eff52d291c0a2c8cc302d786854f34bfc9"
        // keccak("EIP712Domain(string name,string version,uint256 chainId,address verifyingContract,bytes32 salt)")
        val domainSeparatorHash = "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472"

        // Step 1. Generate UserOperation and its hash.

        val batch = Ethereum.Transaction.SCWalletBatch.newBuilder()
            .setWalletType(Ethereum.SCWalletType.Biz4337)
            .addCalls(Ethereum.Transaction.SCWalletBatch.BatchedCall.newBuilder().apply {
                address = "0x0000000000000000000000000000000000000001"
                amount = ByteString.copyFrom("0xde0b6b3a7640000".toHexByteArray())
                payload = ByteString.EMPTY
            })
            .addCalls(Ethereum.Transaction.SCWalletBatch.BatchedCall.newBuilder().apply {
                address = "0x0000000000000000000000000000000000000002"
                amount = ByteString.copyFrom("0xde0b6b3a7640000".toHexByteArray())
                payload = ByteString.EMPTY
            })
            .addCalls(Ethereum.Transaction.SCWalletBatch.BatchedCall.newBuilder().apply {
                address = "0x0000000000000000000000000000000000000003"
                amount = ByteString.copyFrom("0xde0b6b3a7640000".toHexByteArray())
                payload = ByteString.EMPTY
            })
            .build()

        val actualNonce = Numeric.hexStringToByteArray("0x01")
        val passkeyNonce = WCBizPasskeySession.encodePasskeySessionNonce(actualNonce)

        val userOpV07 = Ethereum.UserOperationV0_7.newBuilder().apply {
            entryPoint = "0x0000000071727De22E5E9d8BAf0edAc6f37da032"
            sender = wallet
            preVerificationGas = ByteString.copyFrom("0x186a0".toHexByteArray())          // 100000
            verificationGasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())        // 100000
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
            // Dummy private key.
            privateKey = ByteString.copyFrom("0x03d99692017473e2d631945a812607b23269d85721e0f370b8d3e7d29a874fd2".toHexByteArray())
            chainId = ByteString.copyFrom(chainIdByteArray)                         // 31337
            nonce = ByteString.copyFrom(passkeyNonce)
            txMode = Ethereum.TransactionMode.UserOp
            gasLimit = ByteString.copyFrom("0x186a0".toHexByteArray())              // 100000
            maxFeePerGas = ByteString.copyFrom("0x186A0".toHexByteArray())          // 100000
            maxInclusionFeePerGas = ByteString.copyFrom("0x186A0".toHexByteArray()) // 100000
            toAddress = "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9"

            transaction = Ethereum.Transaction.newBuilder().apply {
                scwBatch = batch
            }.build()

            userOperationV07 = userOpV07
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.ETHEREUM, Ethereum.SigningOutput.parser())

        val userOpHash = output.preHash.toByteArray()
        assertEquals(
            "0x7762e85586107f2bca787a9163b71f0584eabd84258a93cca0e896589a193571",
            Numeric.toHexString(userOpHash)
        )

        // Step 2. Encode UserOperation hash in Biz format.

        val encodedUserOpHash = WCBiz.getEncodedHash(
            chainIdByteArray,
            bizPasskeySessionAccount,
            bizPasskeySessionCodeName,
            codeVersion,
            typeHash,
            domainSeparatorHash,
            wallet,
            Numeric.toHexString(userOpHash)
        )
        assertEquals(
            "0x7d130331f16bb3d2bc3d72db02879d0745d4452592e56723de8b27cf1ee006c7",
            Numeric.toHexString(encodedUserOpHash)
        )

        // Step 3. Generate a WebAuthn with the given challenge (encodedUserOpHash) and passkey signature.

        val clientJsonFirstPart = "{\"type\":\"webauthn.get\",\"challenge\":\""
        val challengeBase64 = Base64.encodeUrl(encodedUserOpHash)
        val challengeBase64NoPad = challengeBase64.trimEnd('=')
        val clientJsonLastPart = "\",\"origin\":\"https://sign.coinbase.com\",\"crossOrigin\":false}"
        val clientJson = clientJsonFirstPart + challengeBase64NoPad + clientJsonLastPart
        assertEquals(clientJson, "{\"type\":\"webauthn.get\",\"challenge\":\"fRMDMfFrs9K8PXLbAoedB0XURSWS5Wcj3osnzx7gBsc\",\"origin\":\"https://sign.coinbase.com\",\"crossOrigin\":false}")

        // Authenticator data for Chrome Profile touchID signature
        val authenticatorData = "0x49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000"
        // Signature computed on a device using passkey.
        val derSignature = "0x3045022073f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be022100e091f452b74519a2894a96d142bdd1888ac6513f5dff76e48c0312144ef9b382".toHexByteArray()

        val passkeySignature = WebAuthnSolidity.getFormattedSignature(authenticatorData, clientJson, derSignature)
        assertEquals(
            "0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000017000000000000000000000000000000000000000000000000000000000000000173f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be1f6e0bac48bae65e76b5692ebd422e773220a96e491827a067b6b8aead6971cf000000000000000000000000000000000000000000000000000000000000002549960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008a7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a2266524d444d66467273394b3850584c62416f656442305855525357533557636a336f736e7a783767427363222c226f726967696e223a2268747470733a2f2f7369676e2e636f696e626173652e636f6d222c2263726f73734f726967696e223a66616c73657d00000000000000000000000000000000000000000000",
            Numeric.toHexString(passkeySignature)
        )

        // Step 4. Final step. Biz-specific adjustments.

        // `passkeyIndex` can be gotten by using `Biz.getPasskeySessionIndexForValidation()` view function.
        // https://github.com/trustwallet/7702-passkey-session/blob/b5c85a5c9a72c19195d1d60a1c90b3908a6a0371/src/BizPasskeySession.sol#L412
        val passkeyIndex: Byte = 0x00
        val passkeyIndexAttachedSignature = byteArrayOf(passkeyIndex) + passkeySignature
        assertEquals(
            "0x00000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000017000000000000000000000000000000000000000000000000000000000000000173f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be1f6e0bac48bae65e76b5692ebd422e773220a96e491827a067b6b8aead6971cf000000000000000000000000000000000000000000000000000000000000002549960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008a7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a2266524d444d66467273394b3850584c62416f656442305855525357533557636a336f736e7a783767427363222c226f726967696e223a2268747470733a2f2f7369676e2e636f696e626173652e636f6d222c2263726f73734f726967696e223a66616c73657d00000000000000000000000000000000000000000000",
            Numeric.toHexString(passkeyIndexAttachedSignature)
        )
    }

    @Test
    fun testBizPasskeySessionSignExecuteWithSignatureCall() {
        // Create ERC20 approve function call
        val approveFunc = EthereumAbiFunction("approve")
        approveFunc.addParamAddress("0xBC472b43BC237f733c78a581078F58A6a89c46Ec".toHexByteArray(), false)
        approveFunc.addParamUInt256("0x3039".toHexByteArray(), false) // 12345
        val approvePayload = EthereumAbi.encode(approveFunc)

        // Build the ExecuteWithSignatureInput protobuf
        val input = wallet.core.jni.proto.BizPasskeySession.ExecuteWithSignatureInput.newBuilder()
        input.apply {
            // Add execution
            addExecutions(wallet.core.jni.proto.BizPasskeySession.Execution.newBuilder().apply {
                address = "0x4B0F1812e5Df2A09796481Ff14017e6005508003" // TWT token
                amount = ByteString.copyFrom("0x00".toHexByteArray())
                payload = ByteString.copyFrom(approvePayload)
            }.build())

            // Private key
            privateKey = ByteString.copyFrom("0xefec50f00ef0c09d967f3e363ee96502ce18a1881f6ac22321aa58071d43c66f".toHexByteArray())

            // Nonce
            nonce = ByteString.copyFrom("0x00".toHexByteArray())

            // Valid after and valid until timestamps
            validAfter = 1_766_601_881L
            validUntil = 1_766_605_881L

            // Encoding hash params
            encodingHashParams = wallet.core.jni.proto.BizPasskeySession.EncodingHashParams.newBuilder().apply {
                chainId = ByteString.copyFrom("0x38".toHexByteArray()) // 56 (BSC)
                codeAddress = "0x02C8740f2C040Fd7A0d21E839796bCf8317d5Ca5"
                codeName = "PasskeySession"
                codeVersion = "v1.0.0"
                typeHash = "0xb652cbbd5eeff31f202f0ace5b19b61f379c8232b83129e309719b4c1321d038"
                domainSeparatorHash = "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472"
            }.build()
        }

        // Call the native function
        val encoded = WCBizPasskeySession.signExecuteWithSignatureCall(input.build().toByteArray())

        // Verify the result
        val expected = "0x3b0f8c1d0000000000000000000000000000000000000000000000000000000000000060000000000000000000000000694c4439000000000000000000000000694c34990000000000000000000000000000000000000000000000000000000000000180000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000bc472b43bc237f733c78a581078f58a6a89c46ec000000000000000000000000000000000000000000000000000000000000303900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004175a7828e4411403210baefbce2a1dde4dc8c88dc6655bbc0e4bd06d510ba1b6f5ca3f630400d7ccc49bc8aeda9a81dd4b85a911470a4acf07a4a28b2fc294d051c00000000000000000000000000000000000000000000000000000000000000"
        assertEquals(expected, Numeric.toHexString(encoded))
    }
}
