package com.trustwallet.core.app.blockchains

import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.Hash
import org.junit.Assert.assertEquals
import org.junit.Test

class TestHash {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testHashKeccak256() {
        val bytes = Hash.keccak256("Test keccak-256".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x9aeb50f48121c80b2ff73ad48b5f197d940f748d936d35c992367370c1abfb18")
    }

    @Test
    fun testSha1() {
        val bytes = Hash.sha1("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x5df9954f1ca26eabf18c663cc9258f7f1fd09f9e")
    }

    @Test
    fun testSha256() {
        val bytes = Hash.sha256("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0xf250fc8f40aeea3297c0158ec1bfa07b503805f2a822530bd63c50625bb9376b")
    }

    @Test
    fun testSha512() {
        val bytes = Hash.sha512("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x14ee17baf1fa3b1c5b3d3f232d609bc8e8c22dc1c4c8a81ac3d51468a27cc2431a54726d511f467d3420f37d5fc3694e8001990b706c4cc9239c397b4a7522e9")
    }

    @Test
    fun testKeccak512() {
        val bytes = Hash.keccak512("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x6caadef962497d4ee4769b854b00cc0eb922cbfc1c8d676bc193ae9fc8d09c9c044d9771dfd96dc362db0dec6dba593a870806de283d177a5d07e36a9aa52077")
    }

    @Test
    fun testSha3_256() {
        val bytes = Hash.sha3256("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0xc0f4cbc9992e2085fbe43a73bc1e2938f54babc0ede584d47d9df4e4511c8c62")
    }

    @Test
    fun testSha3_512() {
        val bytes = Hash.sha3512("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x4fda1bee2d0c28e5eaf7fff8cbb48eed946a6aec7090c610d71896059fc942cfef1a56b811aefe31a750cce4f27921032100a7030aa8b347b3720494a1561fb9")
    }

    @Test
    fun testBlake2b256() {
        val bytes = Hash.blake2b("Test hash".toByteArray(), 32)
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0xe45cf5595c68cb024ad6ec872ab6b7e88377015712e775f643da6af788b5347f")
    }

    @Test
    fun testRipemd160() {
        val bytes = Hash.ripemd("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, "0x687901a63dc3d0dc884232fbaee0badbda853cfa")
    }

    @Test
    fun testSha256RIPEMD() {
        val bytesExpected = Hash.ripemd(Hash.sha256("Test hash".toByteArray()))
        val expHex = Numeric.toHexString(bytesExpected)
        val bytes = Hash.sha256RIPEMD("Test hash".toByteArray())
        val hex = Numeric.toHexString(bytes)
        assertEquals(hex, expHex)
    }
}