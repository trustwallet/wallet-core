// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.elrond

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Elrond

class TestElrondSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private var aliceBech32 = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
    private var aliceSeedHex = "0x413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9"
    private var bobBech32 = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"
   
    @Test
    fun signGenericAction() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val genericAction = Elrond.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("0")
            .setData("foo")
            .setVersion(1)
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(50000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signGenericActionUndelegate() {
        // Successfully broadcasted https://explorer.elrond.com/transactions/3301ae5a6a77f0ab9ceb5125258f12539a113b0c6787de76a5c5867f2c515d65
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(6)
            .setSender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa")
            .setReceiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r")
            .build()

        val genericAction = Elrond.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("0")
            .setData("unDelegate@0de0b6b3a7640000")
            .setVersion(1)
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(12000000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "89f9683af92f7b835bff4e1d0dbfcff5245b3367df4d23538eb799e0ad0a90be29ac3bd3598ce55b35b35ebef68bfa5738eed39fd01adc33476f65bd1b966e0b"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":6,"value":"0","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"dW5EZWxlZ2F0ZUAwZGUwYjZiM2E3NjQwMDAw","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signGenericActionDelegate() {
        // Successfully broadcasted https://explorer.elrond.com/transactions/e5007662780f8ed677b37b156007c24bf60b7366000f66ec3525cfa16a4564e7
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(1)
            .setSender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa")
            .setReceiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r")
            .build()

        val genericAction = Elrond.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("1")
            .setData("delegate")
            .setVersion(1)
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(12000000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "3b9164d47a4e3c0330ae387cd29ba6391f9295acf5e43a16a4a2611645e66e5fa46bf22294ca68fe1948adf45cec8cb47b8792afcdb248bd9adec7c6e6c27108"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":1,"value":"1","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"ZGVsZWdhdGU=","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signEGLDTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = Elrond.EGLDTransfer.newBuilder()
            .setAccounts(accounts)
            .setAmount("1000000000000000000")
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setEgldTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "7e1c4c63b88ea72dcf7855a54463b1a424eb357ac3feb4345221e512ce07c7a50afb6d7aec6f480b554e32cf2037082f3bc17263d1394af1f3ef240be53c930b"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"1000000000000000000","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signESDTTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = Elrond.ESDTTransfer.newBuilder()
            .setAccounts(accounts)
            .setTokenIdentifier("MYTOKEN-1234")
            .setAmount("10000000000000")
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setEsdtTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "9add6d9ac3f1a1fddb07b934e8a73cad3b8c232bdf29d723c1b38ad619905f03e864299d06eb3fe3bbb48a9f1d9b7f14e21dc5eaffe0c87f5718ad0c4198bb0c"
        val expectedData = "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA="

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":425000,"data":"$expectedData","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signESDTNFTTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = Elrond.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = Elrond.ESDTNFTTransfer.newBuilder()
            .setAccounts(accounts)
            .setTokenCollection("LKMEX-aab910")
            .setTokenNonce(4)
            .setAmount("184300000000000000")
            .build()

        val signingInput = Elrond.SigningInput.newBuilder()
            .setEsdtnftTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "cc935685d5b31525e059a16a832cba98dee751983a5a93de4198f6553a2c55f5f1e0b4300fe9077376fa754546da0b0f6697e66462101a209aafd0fc775ab60a"
        val expectedData = "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg="

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$aliceBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":937500,"data":"$expectedData","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }
}
