package com.example.kmpsample
import com.trustwallet.core.*

class Sample {
    private val platform: Platform = getPlatform()

    fun greet(): String {
        var log = "Wallet Core KMP on platform: ${platform.name}!\n\n"
        val wallet = HDWallet(256, "")
        log += "Created mnemonic: ${wallet.mnemonic}\n"
        val btcAddress = wallet.getAddressForCoin(CoinType.Bitcoin)
        val ethAddress = wallet.getAddressForCoin(CoinType.Ethereum)
        log += "Bitcoin address: ${btcAddress}\n"
        log += "Ethereum address: ${ethAddress}\n"
        return log
    }
}