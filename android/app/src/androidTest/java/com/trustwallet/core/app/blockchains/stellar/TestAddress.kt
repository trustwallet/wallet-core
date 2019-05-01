package com.trustwallet.core.app.blockchains.stellar

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.StellarAddress

class TestAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray())
        val pubkey = key.getPublicKeyEd25519()
        val address = StellarAddress(pubkey)

        assertEquals(pubkey.data().toHex(), "0x09A966BCAACC103E38896BAAE3F8C2F06C21FD47DD4F864FF0D33F9819DF5CA2".toLowerCase())
        assertEquals(address.description(), "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI")
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey("0103E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D".toHexByteArray(), PublicKeyType.ED25519)
        val address = StellarAddress(pubkey)

        assertEquals(address.description(), "GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ")
    }
}