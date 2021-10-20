package com.trustwalval.core.app.blockchains.cosmos

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.COSMOS
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.java.AnySigner

class TestCosmosTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, COSMOS).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1
            denom = "muon"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 200
            denom = "muon"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 1037
            chainId = "gaia-13003"
            memo = ""
            sequence = 8
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, COSMOS, SigningOutput.parser())

        assertEquals(output.serialized.toByteArray().toHex(), "0x0a8c010a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e12013112650a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c1a40f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47")
        assertEquals(output.serializedBase64, "CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H")
        assertEquals(output.error, "")
    }

    @Test
    fun testSigningJSON() {
        val json = """
        {
            "accountNumber": "8733",
            "chainId": "cosmoshub-2",
            "fee": {
                "amounts": [{
                    "denom": "uatom",
                    "amount": "5000"
                }],
                "gas": "200000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575",
                    "toAddress": "cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0",
                    "amounts": [{
                        "denom": "uatom",
                        "amount": "995000"
                    }]
                }
            }]
        }
        """
        val key = "c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, COSMOS.value())
        assertEquals(result, """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"uatom"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"uatom"}],"from_address":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","to_address":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"ULEpUqNzoAnYEx2x22F3ANAiPXquAU9+mqLWoAA/ZOUGTMsdb6vryzsW6AKX2Kqj1pGNdrTcQ58Z09JPyjpgEA=="}]}}""")
    }
}
