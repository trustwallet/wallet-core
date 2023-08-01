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
    fun P2pkhCalculateFee() {
		val satVb = "10"
		val tx = Numeric.hexStringToByteArray("02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000")
		var fee = BitcoinFee.calculateFee(tx, satVb).toLong()

        assertEquals(fee, 191 * satVb.toLong())
    }

    @Test
    fun P2wpkhCalculateFee() {
		val satVb = "12"
		val tx = Numeric.hexStringToByteArray("020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000")
		var fee = BitcoinFee.calculateFee(tx, satVb).toLong()

        assertEquals(fee, 189 * satVb.toLong())
    }

    @Test
    // Metadata can be observed live on:
    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
    //
    // Fee/VB   19.608 sat/vByte
    // Size     235 Bytes
    // Weight   610
    fun Brc20TransferCommitCalculateFee() {
		val satVb = "19"
		val tx = Numeric.hexStringToByteArray("02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000")
		var fee = BitcoinFee.calculateFee(tx, satVb).toLong()

        assertEquals(fee, 153 * satVb.toLong()) // 153 = ceil(610/4)
    }
}
