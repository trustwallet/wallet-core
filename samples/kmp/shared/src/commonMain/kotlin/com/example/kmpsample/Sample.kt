package com.example.kmpsample

import com.trustwallet.core.CoinType
import com.trustwallet.core.HDWallet

class Sample {
    private val platform: Platform = getPlatform()

    fun greet(): String {
        var log = "Wallet Core KMP on platform: ${platform.name}!\n\n"
        val wallet = HDWallet(256, "")
        log += "Created mnemonic: ${wallet.mnemonic}\n\n"
        val btcAddress = wallet.getAddressForCoin(CoinType.Bitcoin)
        log += "Bitcoin address: ${btcAddress}\n\n"
        val ethAddress = wallet.getAddressForCoin(CoinType.Ethereum)
        log += "Ethereum address: ${ethAddress}\n\n"
        return log
    }
}