// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import java.security.InvalidParameterException
import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Assert.fail
import org.junit.Test
import wallet.core.jni.*

class TestHDWallet {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    val words =
        "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal"
    val password = "TREZOR"

    @Test
    fun testCreateFromMnemonicImmutableXMainnetFromSignature() {
        // Successfully register: https://api.x.immutable.com/v1/users/0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37
        val hd = HDWallet("obscure opera favorite shuffle mail tip age debate dirt pact cement loyal", "")
        val derivationPath = Ethereum.eip2645GetPath("0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37", "starkex", "immutablex", "1")
        assertEquals(derivationPath, "m/2645'/579218131'/211006541'/2124474935'/1609799702'/1")

        // Retrieve eth private key
        val ethPrivateKey = hd.getKeyForCoin(CoinType.ETHEREUM)
        assertEquals(Numeric.toHexString(ethPrivateKey.data()), "0x03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")

        // Retrieve StarkKey DerivationPath
        val starkDerivationPath = DerivationPath(derivationPath)

        // Retrieve Stark Private key part
        val ethMsg = "Only sign this request if you’ve initiated an action with Immutable X."
        val ethSignature = EthereumMessageSigner.signMessageImmutableX(ethPrivateKey, ethMsg)
        assertEquals(ethSignature, "18b1be8b78807d3326e28bc286d7ee3d068dcd90b1949ce1d25c1f99825f26e70992c5eb7f44f76b202aceded00d74f771ed751f2fe538eec01e338164914fe001")
        val starkPrivateKey = StarkWare.getStarkKeyFromSignature(starkDerivationPath, ethSignature)
        val starkPublicKey = starkPrivateKey.getPublicKeyByType(PublicKeyType.STARKEX)
        assertEquals(Numeric.toHexString(starkPrivateKey.data()), "0x04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de")
        assertEquals(Numeric.toHexString(starkPublicKey.data()), "0x00e5b9b11f8372610ef35d647a1dcaba1a4010716588d591189b27bf3c2d5095")

        // Account register
        val ethMsgToRegister = "Only sign this key linking request from Immutable X"
        val ethSignatureToRegister = EthereumMessageSigner.signMessageImmutableX(ethPrivateKey, ethMsgToRegister)
        assertEquals(ethSignatureToRegister, "646da4160f7fc9205e6f502fb7691a0bf63ecbb74bbb653465cd62388dd9f56325ab1e4a9aba99b1661e3e6251b42822855a71e60017b310b9f90e990a12e1dc01")
        val starkMsg = "463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf"
        val starkSignature = StarkExMessageSigner.signMessage(starkPrivateKey, starkMsg)
        assertEquals(starkSignature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528")
        assertTrue(StarkExMessageSigner.verifyMessage(starkPublicKey, starkMsg, starkSignature))
    }

    @Test
    fun testCreateFromMnemonic() {
        val hd = HDWallet(words, password)
        assertEquals(hd.mnemonic(), words)
        assertEquals(Numeric.toHexString(hd.entropy()), "0xba5821e8c356c05ba5f025d9532fe0f21f65d594")
        assertEquals(Numeric.toHexString(hd.seed()), "0x7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29")
    }

    @Test
    fun testCreateFromMnemonicNoPass() {
        val hd = HDWallet(words, "")
        assertEquals(Numeric.toHexString(hd.seed()), "0x354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d")
    }

    @Test
    fun testCreateFromSpanishMnemonic() {
        val mnemonic = "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut"
        try {
            HDWallet(mnemonic, "")
            fail("Missing exception")
        } catch (ex: Exception) {
            assertTrue(ex is InvalidParameterException)
        }
        val wallet = HDWallet(mnemonic, "", false)
        val address = wallet.getAddressForCoin(CoinType.ETHEREUM)

        assertEquals(address, "0xa4531dE99E22B2166d340E7221669DF565c52024")
    }

    @Test
    fun testGenerate() {
        val hd = HDWallet(128, "")
        assertTrue(Mnemonic.isValid(hd.mnemonic()))
    }

    @Test
    fun tesCreateFromEntropy() {
        val hd = HDWallet(Numeric.hexStringToByteArray("ba5821e8c356c05ba5f025d9532fe0f21f65d594"), "TREZOR")
        assertEquals(hd.mnemonic(), words)
        assertEquals(Numeric.toHexString(hd.entropy()), "0xba5821e8c356c05ba5f025d9532fe0f21f65d594")
    }

    @Test
    fun testGetKeyForCoin() {
        val coin = CoinType.BITCOIN
        val wallet = HDWallet(words, password)
        val key = wallet.getKeyForCoin(coin)

        val address = coin.deriveAddress(key)
        assertEquals(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85")
    }

    @Test
    fun testGetKeyDerivation() {
        val coin = CoinType.BITCOIN
        val wallet = HDWallet(words, password)

        val key1 = wallet.getKeyDerivation(coin, Derivation.BITCOINSEGWIT)
        assertEquals(key1.data().toHex(), "0x1901b5994f075af71397f65bd68a9fff8d3025d65f5a2c731cf90f5e259d6aac")

        val key2 = wallet.getKeyDerivation(coin, Derivation.BITCOINLEGACY)
        assertEquals(key2.data().toHex(), "0x28071bf4e2b0340db41b807ed8a5514139e5d6427ff9d58dbd22b7ed187103a4")

        val key3 = wallet.getKeyDerivation(coin, Derivation.BITCOINTESTNET)
        assertEquals(key3.data().toHex(), "0xca5845e1b43e3adf577b7f110b60596479425695005a594c88f9901c3afe864f")
    }

    @Test
    fun testGetAddressForCoin() {
        val coin = CoinType.BITCOIN
        val wallet = HDWallet(words, password)

        val address = wallet.getAddressForCoin(coin)
        assertEquals(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85")
    }

    @Test
    fun testGetAddressDerivation() {
        val coin = CoinType.BITCOIN
        val wallet = HDWallet(words, password)

        val address1 = wallet.getAddressDerivation(coin, Derivation.BITCOINSEGWIT)
        assertEquals(address1, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85")

        val address2 = wallet.getAddressDerivation(coin, Derivation.BITCOINLEGACY)
        assertEquals(address2, "1PeUvjuxyf31aJKX6kCXuaqxhmG78ZUdL1")

        val address3 = wallet.getAddressDerivation(coin, Derivation.BITCOINTESTNET)
        assertEquals(address3, "tb1qwgpxgwn33z3ke9s7q65l976pseh4edrzfmyvl0")
    }

    @Test
    fun testDerive() {
        val wallet = HDWallet(words, password)
        val key0 = wallet.getKeyForCoin(CoinType.ETHEREUM)

        val publicKey0 = key0.getPublicKeySecp256k1(false)
        val publicKey0Data = Numeric.toHexString(publicKey0.data())

        assertEquals(
            publicKey0Data, "0x0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011")
    }

    @Test
    fun testDeriveBitcoin() {
        val wallet = HDWallet(words, password)
        val key = wallet.getKeyForCoin(CoinType.BITCOIN)
        val publicKey = key.getPublicKeySecp256k1(false)
        val publicKeyData = publicKey.data()

        assertEquals(
            Numeric.toHexString(publicKeyData), "0x047ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b9414ec07cda7b1c9a84c2e04ea2746c21afacc5e91b47427c453c3f1a4a3e983ce5")
    }

    @Test
    fun testExtendedKeys() {
        val words =
            "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
        val wallet = HDWallet(words, "")

        val xprv = wallet.getExtendedPrivateKey(Purpose.BIP44, CoinType.BITCOIN, HDVersion.XPRV)
        val xpub = wallet.getExtendedPublicKey(Purpose.BIP44, CoinType.BITCOIN, HDVersion.XPUB)

        assertEquals(xprv, "xprv9xpXFhFpqdQK3TmytPBqXtGSwS3DLjojFhTGht8gwAAii8py5X6pxeBnQ6ehJiyJ6nDjWGJfZ95WxByFXVkDxHXrqu53WCRGypk2ttuqncb")
        assertEquals(xpub, "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj")

        val yprv = wallet.getExtendedPrivateKey(Purpose.BIP49, CoinType.BITCOIN, HDVersion.YPRV)
        val ypub = wallet.getExtendedPublicKey(Purpose.BIP49, CoinType.BITCOIN, HDVersion.YPUB)
        assertEquals(yprv, "yprvAHwhK6RbpuS3dgCYHM5jc2ZvEKd7Bi61u9FVhYMpgMSuZS613T1xxQeKTffhrHY79hZ5PsskBjcc6C2V7DrnsMsNaGDaWev3GLRQRgV7hxF");
        assertEquals(ypub, "ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");

        val zprv = wallet.getExtendedPrivateKey(Purpose.BIP84, CoinType.BITCOIN, HDVersion.ZPRV)
        val zpub = wallet.getExtendedPublicKey(Purpose.BIP84, CoinType.BITCOIN, HDVersion.ZPUB)
        assertEquals(zprv, "zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE");
        assertEquals(zpub, "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    }

    @Test
    fun testPublicKeyFromX() {
        val xpub = "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj"
        val xpubAddr2 = HDWallet.getPublicKeyFromExtended(xpub, CoinType.BITCOINCASH,"m/44'/145'/0'/0/2")
        val xpubAddr9 = HDWallet.getPublicKeyFromExtended(xpub, CoinType.BITCOINCASH,"m/44'/145'/0'/0/9")

        assertEquals(
            Numeric.toHexString(xpubAddr2.data()),
            "0x0338994349b3a804c44bbec55c2824443ebb9e475dfdad14f4b1a01a97d42751b3"
        )
        assertEquals(
            Numeric.toHexString(xpubAddr9.data()),
            "0x03786c1d274f2c804ff9a57d8e7289c281d4aef15e17187ad9f9c3722d81a6ae66"
        )
    }

    @Test
    fun testInitHDWallet() {
        val hdWallet2 = HDWallet(160, "")
        assertFalse(hdWallet2.mnemonic().isNullOrBlank())
        val wordList2 = hdWallet2.mnemonic().split(" ")
        Mnemonic.isValid(wordList2.joinToString { " " })

        val hdWallet3 = HDWallet(160, "")
        assertFalse(hdWallet3.mnemonic().isNullOrBlank())
        val wordList3 = hdWallet3.mnemonic().split(" ")
        Mnemonic.isValid(wordList3.joinToString { " " })
    }

    @Test
    fun testInitHDWalletWithPhrase() {
        val hdWallet = HDWallet(words, "")
        assertEquals(hdWallet.mnemonic(), words)
    }
}
