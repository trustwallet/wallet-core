package com.trustwallet.core.app.blockchains.bitcoin

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BitcoinFee

class TestBitcoinFee {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun P2pkhFee() {
		val satVb: Long = 19
		val tx = Numeric.hexStringToByteArray("02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000")
		var fee = BitcoinFee.calculateFee(tx, satVb)

        assertEquals(fee, 191 * satVb)
    }
}
