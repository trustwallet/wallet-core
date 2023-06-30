package com.trustwallet.core.app.blockchains.cosmos

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.COSMOS
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
import wallet.core.java.AnySigner

class TestCosmosTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAuthStakingTransaction() {
        val key =
            PrivateKey("c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc".toHexByteArray())

        val stakeAuth = Cosmos.Message.StakeAuthorization.newBuilder().apply {
            allowList = Cosmos.Message.StakeAuthorization.Validators.newBuilder().apply {
                addAddress("cosmosvaloper1gjtvly9lel6zskvwtvlg5vhwpu9c9waw7sxzwx")
            }.build()
            authorizationType = Cosmos.Message.AuthorizationType.DELEGATE
        }.build()

        val authStakingMsg = Cosmos.Message.AuthGrant.newBuilder().apply {
            grantee = "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf"
            granter = "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd"
            grantStake = stakeAuth
            expiration = 1692309600
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            authGrant = authStakingMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "2418"
            denom = "uatom"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 96681
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 1290826
            chainId = "cosmoshub-4"
            memo = ""
            sequence = 5
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, COSMOS, SigningOutput.parser())

        assertEquals(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjoSNgo0Y29zbW9zdmFsb3BlcjFnanR2bHk5bGVsNnpza3Z3dHZsZzV2aHdwdTljOXdhdzdzeHp3eCABEgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQIFyfuijGKf87Hz61ZqxasfLI1PZnNge4RDq/tRyB/tZI6p80iGRqHecoV6+84EQkc9GTlNRQOSlApRCsivT9XI=\"}"
        )
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun testRemoveAuthStakingTransaction() {
        val key =
            PrivateKey("c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc".toHexByteArray())

        val removeAuthStakingMsg = Cosmos.Message.AuthRevoke.newBuilder().apply {
            grantee = "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf"
            granter = "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd"
            msgTypeUrl = "/cosmos.staking.v1beta1.MsgDelegate"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            authRevoke = removeAuthStakingMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "2194"
            denom = "uatom"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 87735
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 1290826
            chainId = "cosmoshub-4"
            memo = ""
            sequence = 4
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, COSMOS, SigningOutput.parser())

        assertEquals(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CqoBCqcBCh8vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnUmV2b2tlEoMBCi1jb3Ntb3MxM2swcTBsN2xnMmtyMzJrdnQ3bHkyMzZwcGxkeTh2OWR6d2gzZ2QSLWNvc21vczFmczdsdTI4aHg1bTlha203cnAwYzI0MjJjbjhyMmY3Z3VydWpoZhojL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuTXNnRGVsZWdhdGUSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAQSEwoNCgV1YXRvbRIEMjE5NBC3rQUaQI7K+W7MMBoD6FbFZxRBqs9VTjErztjWTy57+fvrLaTCIZ+eBs7CuaKqfUZdSN8otjubSHVTQID3k9DpPAX0yDo=\"}"
        )
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun testSigningTransaction() {
        val key =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, COSMOS).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1"
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
            amount = "200"
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

        assertEquals(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H\"}"
        )
        assertEquals(output.errorMessage, "")
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
        val key =
            "c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, COSMOS.value())
        assertEquals(
            result,
            """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"uatom"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"uatom"}],"from_address":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","to_address":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"ULEpUqNzoAnYEx2x22F3ANAiPXquAU9+mqLWoAA/ZOUGTMsdb6vryzsW6AKX2Kqj1pGNdrTcQ58Z09JPyjpgEA=="}]}}"""
        )
    }
}
