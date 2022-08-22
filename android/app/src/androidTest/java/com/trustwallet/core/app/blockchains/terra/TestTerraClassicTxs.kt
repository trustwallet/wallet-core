package com.trustwallet.core.app.blockchains.terra

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.TERRA
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
import wallet.core.java.AnySigner

class TestTerraClassicTxs {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key =
            PrivateKey("1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, TERRA).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1000000"
            denom = "uluna"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"
            addAllAmounts(listOf(txAmount))
            typePrefix = "bank/MsgSend"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "3000"
            denom = "uluna"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 158
            chainId = "soju-0013"
            memo = ""
            sequence = 0
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, TERRA, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"bank/MsgSend","value":{"amount":[{"amount":"1000000","denom":"uluna"}],"from_address":"terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe","to_address":"terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"},"signature":"KPdiVsKpY12JG/VKEJVa/FpMKclxlS0qNNG6VOAypj10R5vY5UX5IgRJET1zNYnH0wvcXxfNXV+s8jtwN2UXiQ=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningWasmTerraTransferTxProtobuf() {
        val key =
            PrivateKey("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, TERRA).description()

        val wasmTransferMessage = Cosmos.Message.WasmTerraExecuteContractTransfer.newBuilder().apply {
            senderAddress = from
            contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76" // ANC
            amount = ByteString.copyFrom("0x3D090".toHexByteArray()) // 250000
            recipientAddress = "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            wasmTerraExecuteContractTransferMessage = wasmTransferMessage
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "3000"
            denom = "uluna"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 3407705
            chainId = "columbus-5"
            memo = ""
            sequence = 3
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, TERRA, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw\"}")
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun testSigningWasmTerraGenericProtobuf() {
        val key =
            PrivateKey("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, TERRA).description()

        val wasmGenericMessage = Cosmos.Message.WasmTerraExecuteContractGeneric.newBuilder().apply {
            senderAddress = from
            contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76" // ANC
            executeMsg = """{"transfer": { "amount": "250000", "recipient": "terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj" } }"""
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            wasmTerraExecuteContractGeneric = wasmGenericMessage
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "3000"
            denom = "uluna"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 3407705
            chainId = "columbus-5"
            memo = ""
            sequence = 7
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, TERRA, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Cu4BCusBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLAAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2GmJ7InRyYW5zZmVyIjogeyAiYW1vdW50IjogIjI1MDAwMCIsICJyZWNpcGllbnQiOiAidGVycmExZDcwNDhjc2FwNHd6Y3Y1em03ejZ0ZHFlbTJhZ3lwOTY0N3ZkeWoiIH0gfRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYBxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAkPsS7xlSng2LMc9KiD1soN5NLaDcUh8I9okPmsdJN3le1B7yxRGNB4aQfhaRl/8Z0r5vitRT0AWuxDasd8wcFw==\"}")
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun testSigningWasmTerraGenericWithCoinsProtobuf() {
        val key =
            PrivateKey("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, TERRA).description()

        val coins = Cosmos.Amount.newBuilder().apply {
            amount = "1000"
            denom = "uusd"
        }.build()

        val wasmGenericMessage = Cosmos.Message.WasmTerraExecuteContractGeneric.newBuilder().apply {
            senderAddress = from
            contractAddress = "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s" // ANC Market
            executeMsg = """{ "deposit_stable": {} }"""
            addCoins(coins)
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            wasmTerraExecuteContractGeneric = wasmGenericMessage
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "7000"
            denom = "uluna"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 600000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 3407705
            chainId = "columbus-5"
            memo = ""
            sequence = 9
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, TERRA, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CrIBCq8BCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBKEAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMXNlcGZqN3MwYWVnNTk2N3V4bmZrNHRoemxlcnJza3RrcGVsbTVzGhh7ICJkZXBvc2l0X3N0YWJsZSI6IHt9IH0qDAoEdXVzZBIEMTAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYCRITCg0KBXVsdW5hEgQ3MDAwEMDPJBpAGyi7f1ioY8XV6pjFq1s86Om4++CIUnd3rLHif2iopCcYvX0mLkTlQ6NUERg8nWTYgXcj6fOTO/ptgPuAtv0NWg==\"}")
        assertEquals(output.errorMessage, "")
    }
}
