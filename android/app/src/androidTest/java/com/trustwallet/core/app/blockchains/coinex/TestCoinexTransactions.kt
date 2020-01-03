package com.trustwalval.core.app.blockchains.coinex

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert
import org.junit.Test
import wallet.core.jni.CoinexSigner
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Coinex

class TestCoinexTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testProposalVoteTransaction() {

        val privKey =
            PrivateKey("c4c4f50e9af57eb281f07fb5e4ebb76d7ca1132549ffaa3aba3862fe58653244".toHexByteArray())

        val proposalVoteMsg = Coinex.ProposalVoteMessage.newBuilder().apply{
            voter = "cettest178w9m7yzkagpq090t593r6gqe5gkfkvjl76znl"
            proposalId = "14"
            option = "Yes"
        }.build()

        val feeAmount = Coinex.Amount.newBuilder().apply {
            amount = 2000000
            denom = "cet"
        }.build()

        val coinexFee = Coinex.Fee.newBuilder().apply {
            gas = 100000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Coinex.SigningInput.newBuilder().apply {
            accountNumber = 24
            chainId = "coinexdex-test2006"
            memo = "ifwallet"
            sequence = 183
            proposalVoteMessage = proposalVoteMsg
            fee = coinexFee
            privateKey = ByteString.copyFrom(privKey.data())
        }.build()

        var expectJson = "{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"2000000\",\"denom\":\"cet\"}],\"gas\":\"100000\"},\"memo\":\"ifwallet\",\"msg\":[{\"type\":\"cosmos-sdk/MsgVote\",\"value\":{\"option\":\"Yes\",\"proposal_id\":\"14\",\"voter\":\"cettest178w9m7yzkagpq090t593r6gqe5gkfkvjl76znl\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AjLG1hKhMdCboR/T4fk8iWE+nam8IVzA4jIuMHO4BJvd\"},\"signature\":\"llA/STsCnnFLTJ9qneDQsOCcGXML58O22OKPhK2ycbpSWSXFSXj60KCfcQQFTKIdGhOHEVTEQ5ozayCg0PqYcg==\"}],\"type\":\"bankx/MsgSend\"}}"
        var json = CoinexSigner.sign(signingInput).json

        Assert.assertEquals(expectJson, json)


    }
}