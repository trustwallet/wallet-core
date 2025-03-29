package com.trustwallet.core.app.blockchains

import com.trustwallet.core.app.utils.toHexByteArray
import wallet.core.jni.CoinType
import wallet.core.jni.Curve
import wallet.core.jni.PublicKey
import wallet.core.jni.Purpose
import wallet.core.jni.Derivation

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PublicKeyType


class TestCoinType {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testCoinType() {
        assertEquals(CoinType.BITCOIN.value(), 0)
        assertEquals(CoinType.LITECOIN.value(), 2)
        assertEquals(CoinType.TRON.value(), 195)
        assertEquals(CoinType.ETHEREUM.value(), 60)
        assertEquals(CoinType.THUNDERCORE.value(), 1001)
        assertEquals(CoinType.WANCHAIN.value(), 5718350)
        assertEquals(CoinType.CALLISTO.value(), 820)
        assertEquals(CoinType.ETHEREUMCLASSIC.value(), 61)
        assertEquals(CoinType.GOCHAIN.value(), 6060)
        assertEquals(CoinType.ONTOLOGY.value(), 1024)
        assertEquals(CoinType.POANETWORK.value(), 178)
        assertEquals(CoinType.VECHAIN.value(), 818)
        assertEquals(CoinType.ICON.value(), 74)
        assertEquals(CoinType.VICTION.value(), 889)
        assertEquals(CoinType.TEZOS.value(), 1729)
        assertEquals(CoinType.QTUM.value(), 2301)
        assertEquals(CoinType.NEBULAS.value(), 2718)
        assertEquals(CoinType.PACTUS.value(), 21888)
    }

    @Test
    fun testCoinPurpose() {
        assertEquals(Purpose.BIP84, CoinType.BITCOIN.purpose())
        assertEquals(Purpose.BIP44, CoinType.PACTUS.purpose())
    }

    @Test
    fun testCoinCurve() {
        assertEquals(Curve.SECP256K1, CoinType.BITCOIN.curve())
        assertEquals(Curve.ED25519, CoinType.PACTUS.curve())
    }

    @Test
    fun testDerivationPathBitcoin() {
        var res = CoinType.createFromValue(CoinType.BITCOIN.value()).derivationPath().toString()
        assertEquals(res, "m/84'/0'/0'/0/0")
        res = CoinType.createFromValue(CoinType.BITCOIN.value()).derivationPathWithDerivation(Derivation.BITCOINLEGACY).toString()
        assertEquals(res, "m/44'/0'/0'/0/0")
        res = CoinType.createFromValue(CoinType.BITCOIN.value()).derivationPathWithDerivation(Derivation.BITCOINTAPROOT).toString()
        assertEquals(res, "m/86'/0'/0'/0/0")
        res = CoinType.createFromValue(CoinType.SOLANA.value()).derivationPathWithDerivation(Derivation.SOLANASOLANA).toString()
        assertEquals(res, "m/44'/501'/0'/0'")
    }

    @Test
    fun testDeriveAddressFromPublicKeyAndDerivationBitcoin() {
        val publicKey = PublicKey("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798".toHexByteArray(), PublicKeyType.SECP256K1)

        val address = CoinType.BITCOIN.deriveAddressFromPublicKeyAndDerivation(publicKey, Derivation.BITCOINSEGWIT)
        assertEquals(address, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4")
    }

    @Test
    fun testDerivationPathPactus() {
        var res = CoinType.createFromValue(CoinType.PACTUS.value()).derivationPath().toString()
        assertEquals(res, "m/44'/21888'/3'/0'")
        res = CoinType.createFromValue(CoinType.PACTUS.value()).derivationPathWithDerivation(Derivation.PACTUSMAINNET).toString()
        assertEquals(res, "m/44'/21888'/3'/0'")
        res = CoinType.createFromValue(CoinType.PACTUS.value()).derivationPathWithDerivation(Derivation.PACTUSTESTNET).toString()
        assertEquals(res, "m/44'/21777'/3'/0'")
    }

    @Test
    fun testDeriveAddressFromPublicKeyAndDerivationPactus() {
        val publicKey = PublicKey("95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa".toHexByteArray(), PublicKeyType.ED25519)

        val mainnet_address = CoinType.PACTUS.deriveAddressFromPublicKeyAndDerivation(publicKey, Derivation.PACTUSMAINNET)
        assertEquals(mainnet_address, "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr")

        val testnet_address = CoinType.PACTUS.deriveAddressFromPublicKeyAndDerivation(publicKey, Derivation.PACTUSTESTNET)
        assertEquals(testnet_address, "tpc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymzqkcrg")
    }
}
