package com.trustwallet.core.app.blockchains

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.HDWallet
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.BINANCE
import wallet.core.jni.CoinType.BITCOIN
import wallet.core.jni.CoinType.BITCOINCASH
import wallet.core.jni.CoinType.CALLISTO
import wallet.core.jni.CoinType.DASH
import wallet.core.jni.CoinType.ETHEREUM
import wallet.core.jni.CoinType.ETHEREUMCLASSIC
import wallet.core.jni.CoinType.GO
import wallet.core.jni.CoinType.ICON
import wallet.core.jni.CoinType.LITECOIN
import wallet.core.jni.CoinType.POA
import wallet.core.jni.CoinType.RIPPLE
import wallet.core.jni.CoinType.TEZOS
import wallet.core.jni.CoinType.THUNDERTOKEN
import wallet.core.jni.CoinType.TOMOCHAIN
import wallet.core.jni.CoinType.TRON
import wallet.core.jni.CoinType.VECHAIN
import wallet.core.jni.CoinType.WANCHAIN
import wallet.core.jni.CoinType.XDAI
import wallet.core.jni.CoinType.ZCASH
import wallet.core.jni.CoinType.ZCOIN
import wallet.core.jni.CoinType.NIMIQ
import wallet.core.jni.CoinType.STELLAR
import wallet.core.jni.CoinType.AION
import wallet.core.jni.CoinType.NEO
import wallet.core.jni.CoinType.THETA

class CoinAddressDerivationTests {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testDeriveAddressesFromPhrase() {
        val wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "")

        for (i in 0 .. 4) {
            for (coin in CoinType.values()) {
                val privateKey = wallet.getKeyForCoin(coin)
                val address = coin.deriveAddress(privateKey)

                when(coin) {
                    BINANCE -> assertEquals("tbnb12vtaxl9952zm6rwf7v8jerq74pvaf77fkw9xhl", address)
                    BITCOIN -> assertEquals("bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d", address)
                    BITCOINCASH -> assertEquals("bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70", address)
                    CALLISTO -> assertEquals("0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04", address)
                    DASH -> assertEquals("XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT", address)
                    ETHEREUM -> assertEquals("0x8f348F300873Fd5DA36950B2aC75a26584584feE", address)
                    ETHEREUMCLASSIC -> assertEquals("0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c", address)
                    GO ->  assertEquals("0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2", address)
                    ICON -> assertEquals("hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106", address)
                    LITECOIN -> assertEquals("ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u", address)
                    POA -> assertEquals("0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9", address)
                    RIPPLE -> assertEquals("rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3", address)
                    TEZOS -> assertEquals("tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX", address)
                    THUNDERTOKEN -> assertEquals("0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7", address)
                    TOMOCHAIN -> assertEquals("0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424", address)
                    TRON -> assertEquals("TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio", address)
                    VECHAIN -> assertEquals("0x1a553275dF34195eAf23942CB7328AcF9d48c160", address)
                    WANCHAIN -> assertEquals("0xd5CA90B928279fe5d06144136A25dEd90127Ac15", address)
                    XDAI -> assertEquals("0x364d0551599B97EAf997bc06c8c40Aaf73124402", address)
                    ZCASH -> assertEquals("t1YYnByMzdGhQv3W3rnjHMrJs6HH4Y231gy", address)
                    ZCOIN ->  assertEquals("aEd5XFChyXobvEics2ppAqgK3Bgusjxtik", address)
                    NIMIQ -> assertEquals("NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H", address)
                    STELLAR -> assertEquals("GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P", address)
                    AION -> assertEquals("0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e", address)
                    NEO -> assertEquals("AUa3rzbGJe7MbHf8Kra9em8oaLBL1MDYKF", address)
                    THETA -> assertEquals("0x0d1fa20c218Fec2f2C55d52aB267940485fa5DA4", address)
                }
            }
        }
    }
}