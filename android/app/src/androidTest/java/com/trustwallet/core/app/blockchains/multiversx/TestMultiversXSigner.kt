// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.multiversx

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.MultiversX

class TestMultiversXSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private var aliceBech32 = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
    private var aliceSeedHex = "0x413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9"
    private var bobBech32 = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"
    private var carolBech32 = "erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8"
   
    @Test
    fun signGenericAction() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val genericAction = MultiversX.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("0")
            .setData("foo")
            .setVersion(1)
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(50000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signGenericActionWithGuardian() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(42)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .setGuardian(carolBech32)
            .build()

        val genericAction = MultiversX.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("1000000000000000000")
            .setVersion(2)
            .setOptions(2)
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(100000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "dae30e5cddb4a1f050009f939ce2c90843770870f9e6c77366be07e5cd7b3ebfdda38cd45d04e9070037d57761b6a68cee697e6043057f9dc565a4d0e632480d"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":42,"value":"1000000000000000000","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"signature":"$expectedSignature","options":2,"guardian":"$carolBech32"}""", output.encoded)
    }

    @Test
    fun signGenericActionUndelegate() {
        // Successfully broadcasted https://explorer.multiversx.com/transactions/3301ae5a6a77f0ab9ceb5125258f12539a113b0c6787de76a5c5867f2c515d65
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(6)
            .setSender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa")
            .setReceiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r")
            .build()

        val genericAction = MultiversX.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("0")
            .setData("unDelegate@0de0b6b3a7640000")
            .setVersion(1)
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(12000000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "89f9683af92f7b835bff4e1d0dbfcff5245b3367df4d23538eb799e0ad0a90be29ac3bd3598ce55b35b35ebef68bfa5738eed39fd01adc33476f65bd1b966e0b"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":6,"value":"0","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"dW5EZWxlZ2F0ZUAwZGUwYjZiM2E3NjQwMDAw","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signGenericActionDelegate() {
        // Successfully broadcasted https://explorer.multiversx.com/transactions/e5007662780f8ed677b37b156007c24bf60b7366000f66ec3525cfa16a4564e7
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(1)
            .setSender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa")
            .setReceiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r")
            .build()

        val genericAction = MultiversX.GenericAction.newBuilder()
            .setAccounts(accounts)
            .setValue("1")
            .setData("delegate")
            .setVersion(1)
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setGenericAction(genericAction)
            .setGasPrice(1000000000)
            .setGasLimit(12000000)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "3b9164d47a4e3c0330ae387cd29ba6391f9295acf5e43a16a4a2611645e66e5fa46bf22294ca68fe1948adf45cec8cb47b8792afcdb248bd9adec7c6e6c27108"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":1,"value":"1","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"ZGVsZWdhdGU=","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signEGLDTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = MultiversX.EGLDTransfer.newBuilder()
            .setAccounts(accounts)
            .setAmount("1000000000000000000")
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setEgldTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "0f40dec9d37bde3c67803fc535088e536344e271807bb7c1aa24af3c69bffa9b705e149ff7bcaf21678f4900c4ee72741fa6ef08bf4c67fc6da1c6b0f337730e"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"1000000000000000000","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":2,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signEGLDTransferWithGuardian() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .setGuardian(carolBech32)
            .build()

        val transfer = MultiversX.EGLDTransfer.newBuilder()
            .setAccounts(accounts)
            .setAmount("1000000000000000000")
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setEgldTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "741dd0d24db4df37a050f693f8481b6e51b8dd6dfc2f01a4f90aa1af3e59c89a8b0ef9d710af33103970e353d9f0cb9fd128a2e174731cbc88265d9737ed5604"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"1000000000000000000","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"signature":"$expectedSignature","options":2,"guardian":"$carolBech32"}""", output.encoded)
    }

    @Test
    fun signESDTTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = MultiversX.ESDTTransfer.newBuilder()
            .setAccounts(accounts)
            .setTokenIdentifier("MYTOKEN-1234")
            .setAmount("10000000000000")
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setEsdtTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "dd7cdc90aa09da6034b00a99e3ba0f1a2a38fa788fad018d53bf2e706f99e1a42c80063c28e6b48a5f2574c4054986f34c8eb36b1da63a22d19cf3ea5990b306"
        val expectedData = "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA="

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":425000,"data":"$expectedData","chainID":"1","version":2,"signature":"$expectedSignature"}""", output.encoded)
    }

    @Test
    fun signESDTNFTTransfer() {
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val accounts = MultiversX.Accounts.newBuilder()
            .setSenderNonce(7)
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .build()

        val transfer = MultiversX.ESDTNFTTransfer.newBuilder()
            .setAccounts(accounts)
            .setTokenCollection("LKMEX-aab910")
            .setTokenNonce(4)
            .setAmount("184300000000000000")
            .build()

        val signingInput = MultiversX.SigningInput.newBuilder()
            .setEsdtnftTransfer(transfer)
            .setChainId("1")
            .setPrivateKey(privateKey)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.MULTIVERSX, MultiversX.SigningOutput.parser())
        val expectedSignature = "59af89d9a9ece1f35bc34323c42061cae27bb5f9830f5eb26772e680732cbd901a86caa7c3eadacd392fe1024bef4c1f08ce1dfcafec257d6f41444ccea30a0c"
        val expectedData = "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg="

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":7,"value":"0","receiver":"$aliceBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":937500,"data":"$expectedData","chainID":"1","version":2,"signature":"$expectedSignature"}""", output.encoded)
    }
}
