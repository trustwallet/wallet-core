package com.trustwallet.core.app.blockchains

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.HDWallet
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.*

class CoinAddressDerivationTests {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testDeriveAddressesFromPhrase() {
        val wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "")

        for (i in 0 .. 4) {
            CoinType.values().forEach { coin ->
                val privateKey = wallet.getKeyForCoin(coin)
                val address = coin.deriveAddress(privateKey)
                runDerivationChecks(coin, address)
            }
        }
    }

    private fun runDerivationChecks(coin: CoinType, address: String?) = when (coin) {
        BINANCE -> assertEquals("bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw", address)
        BITCOIN -> assertEquals("bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d", address)
        BITCOINCASH -> assertEquals("bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70", address)
        CALLISTO -> assertEquals("0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04", address)
        DASH -> assertEquals("XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT", address)
        ELLAISM -> assertEquals("0x1Ae593CAdb8BAC47D0394617396650474c303F9C", address)
        ETHEREUM -> assertEquals("0x8f348F300873Fd5DA36950B2aC75a26584584feE", address)
        ETHEREUMCLASSIC -> assertEquals("0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c", address)
        ETHERSOCIAL -> assertEquals("0x182dd55D97C1F8D3781CDaDFC275948Ac38Ea1fe", address)
        GOCHAIN -> assertEquals("0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2", address)
        GROESTLCOIN -> assertEquals("grs1qexwmshts5pdpeqglkl39zyl6693tmfwp0cue4j", address)
        ICON -> assertEquals("hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106", address)
        IOCOIN -> assertEquals("ieCTzUZhc8a5T8Y8suosPw7eMgTtN5PE1H", address)
        LITECOIN -> assertEquals("ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u", address)
        ONTOLOGY -> assertEquals("AHKTnybvnWo3TeY8uvNXekvYxMrXogUjeT", address)
        POANETWORK -> assertEquals("0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9", address)
        XRP -> assertEquals("rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3", address)
        TEZOS -> assertEquals("tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX", address)
        THUNDERTOKEN -> assertEquals("0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7", address)
        TOMOCHAIN -> assertEquals("0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424", address)
        TRON -> assertEquals("TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio", address)
        VECHAIN -> assertEquals("0x1a553275dF34195eAf23942CB7328AcF9d48c160", address)
        WANCHAIN -> assertEquals("0xd5CA90B928279fe5d06144136A25dEd90127Ac15", address)
        XDAI -> assertEquals("0x364d0551599B97EAf997bc06c8c40Aaf73124402", address)
        ZCASH -> assertEquals("t1YYnByMzdGhQv3W3rnjHMrJs6HH4Y231gy", address)
        ZCOIN -> assertEquals("aEd5XFChyXobvEics2ppAqgK3Bgusjxtik", address)
        NIMIQ -> assertEquals("NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H", address)
        STELLAR -> assertEquals("GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P", address)
        AION -> assertEquals("0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e", address)
        NANO -> assertEquals("nano_39gsbcishxn3n7wd17ono4otq5wazwzusqgqigztx73wbrh5jwbdbshfnumc", address)
        NEO -> assertEquals("AUa3rzbGJe7MbHf8Kra9em8oaLBL1MDYKF", address)
        THETA -> assertEquals("0x0d1fa20c218Fec2f2C55d52aB267940485fa5DA4", address)
        COSMOS -> assertEquals("cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn", address)
        DECRED -> assertEquals("DsidJiDGceqHTyqiejABy1ZQ3FX4SiWZkYG", address)
        DOGECOIN -> assertEquals("DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f", address)
        KIN -> assertEquals("GBL3MT2ICHHM5OJ2QJ44CAHGDK6MLPINVXBKOKLHGBWQDVRWTWQ7U2EA", address)
        VIACOIN -> assertEquals("via1qnmsgjd6cvfprnszdgmyg9kewtjfgqflz67wwhc", address)
        IOST -> assertEquals("EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin", address)
        LUX -> assertEquals("LYL6SZG8S6dyXRFT8Bw4FHUoVef3cWCoPi", address)
        QTUM -> assertEquals("QhceuaTdeCZtcxmVc6yyEDEJ7Riu5gWFoF", address)
        NULS -> assertEquals("NsdtNvsfmPerWk4BhcapHTB3LptF8Sbe", address)
        BRAVOCOIN -> assertEquals("BRV8g4c7Uve8NNsqDMeADLqPeFyohMQ7d9MzarSE23KogTS71BaiQ", address)
        STEEM -> assertEquals("STM6WSusVTidc1e7TbLjhqQPYctbsndTRwXHpi82gMuJyKEkJVLvg", address)
        EOS -> assertEquals("EOS6hs8sRvGSzuQtq223zwJipMzqTJpXUVjyvHPvPwBSZWWrJTJkg", address)
        IOTEX -> assertEquals("io1qw9cccecw09q7p5kzyqtuhfhvah2mhfrc84jfk", address)
        ZILLIQA -> assertEquals("0xDdb41006F7B6FA8e5FBF06A71c01F789FeBC66e8", address)
        SEMUX -> assertEquals("0xfe604170382452f77bc922bc19eb4b53504b09c2", address)
        DEXON -> assertEquals("0x6F3E6a6dDf2C2B4B32B8Bb452eA3F36B2BB489BF", address)
        ZELCASH -> assertEquals("t1UKbRPzL4WN8Rs8aZ8RNiWoD2ftCMHKGUf", address)
        ARK -> assertEquals("Ac49m5pu5YpMMNgEbSYeZUEpRMHcSK3DfV", address)
    }
}
