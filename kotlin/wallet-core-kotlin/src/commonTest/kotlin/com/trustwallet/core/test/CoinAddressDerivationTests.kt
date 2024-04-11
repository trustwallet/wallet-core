package com.trustwallet.core.test

import com.trustwallet.core.CoinType
import com.trustwallet.core.CoinType.*
import com.trustwallet.core.HDWallet
import com.trustwallet.core.LibLoader
import kotlin.test.Test
import kotlin.test.assertEquals

class CoinAddressDerivationTests {

    init {
        LibLoader.loadLibrary()
    }

    @Test
    fun testDeriveAddressesFromPhrase() {
        val wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "")

        CoinType.values().forEach { coin ->
            val address = wallet.getAddressForCoin(coin)
            val expectedAddress = getExpectedAddress(coin)

            assertEquals(expectedAddress, address, "Coin = $coin")
        }
    }

    private fun getExpectedAddress(coin: CoinType): String = when (coin) {
        Binance -> "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw"
        TBinance -> "tbnb12vtaxl9952zm6rwf7v8jerq74pvaf77fkw9xhl"
        Bitcoin -> "bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d"
        BitcoinDiamond -> "1KaRW9xPPtCTZ9FdaTHduCPck4YvSeEWNn"
        BitcoinCash -> "bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70"
        BitcoinGold -> "btg1qwz9sed0k4neu6ycrudzkca6cnqe3zweq35hvtg"
        Callisto -> "0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04"
        Dash -> "XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT"
        DigiByte -> "dgb1qtjgmerfqwdffyf8ghcrkgy52cghsqptynmyswu"

        Ethereum, SmartChain, Polygon, Optimism, Zksync, Arbitrum, ArbitrumNova, ECOChain, AvalancheCChain, XDai,
        Fantom, Celo, CronosChain, SmartBitcoinCash, KuCoinCommunityChain, Boba, Metis,
        Aurora, Evmos, Moonriver, Moonbeam, KavaEvm, Klaytn, Meter, OKXChain, PolygonzkEVM, Scroll,
        ConfluxeSpace, AcalaEVM, OpBNB, Neon, Base, Linea, Greenfield, Mantle, ZenEON, MantaPacific,
        ZetaEVM, Merlin, Lightlink, Blast,
        -> "0x8f348F300873Fd5DA36950B2aC75a26584584feE"

        Ronin -> "ronin:8f348F300873Fd5DA36950B2aC75a26584584feE"
        EthereumClassic -> "0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c"
        GoChain -> "0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2"
        Groestlcoin -> "grs1qexwmshts5pdpeqglkl39zyl6693tmfwp0cue4j"
        ICON -> "hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106"
        Litecoin -> "ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u"
        Ontology -> "AHKTnybvnWo3TeY8uvNXekvYxMrXogUjeT"
        POANetwork -> "0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9"
        XRP -> "rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3"
        Tezos -> "tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX"
        ThunderCore -> "0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7"
        Viction -> "0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424"
        Tron -> "TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio"
        VeChain -> "0x1a553275dF34195eAf23942CB7328AcF9d48c160"
        Wanchain -> "0xD5ca90b928279FE5D06144136a25DeD90127aC15"
        Komodo -> "RCWJLXE5CSXydxdSnwcghzPgkFswERegyb"
        Zcash -> "t1YYnByMzdGhQv3W3rnjHMrJs6HH4Y231gy"
        Zen -> "znUmzvod1f4P9LYsBhNxjqCDQvNSStAmYEX"
        Firo -> "aEd5XFChyXobvEics2ppAqgK3Bgusjxtik"
        Nimiq -> "NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H"
        Stellar -> "GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P"
        Aion -> "0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e"
        Nano -> "nano_39gsbcishxn3n7wd17ono4otq5wazwzusqgqigztx73wbrh5jwbdbshfnumc"
        Nebulas -> "n1ZVgEidtdseYv9ogmGz69Cz4mbqmHYSNqJ"
        NEAR -> "0c91f6106ff835c0195d5388565a2d69e25038a7e23d26198f85caf6594117ec"
        Theta, ThetaFuel -> "0x0d1fa20c218Fec2f2C55d52aB267940485fa5DA4"
        Cosmos -> "cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn"
        Decred -> "DsidJiDGceqHTyqiejABy1ZQ3FX4SiWZkYG"
        Dogecoin -> "DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f"
        Kin -> "GBL3MT2ICHHM5OJ2QJ44CAHGDK6MLPINVXBKOKLHGBWQDVRWTWQ7U2EA"
        Viacoin -> "via1qnmsgjd6cvfprnszdgmyg9kewtjfgqflz67wwhc"
        Verge -> "DPb3Xz4vjB6QGLKDmrbprrtv4XzNqkADc2"
        Qtum -> "QhceuaTdeCZtcxmVc6yyEDEJ7Riu5gWFoF"
        NULS -> "NULSd6HgU8MoRnNjBgvJpa9tqvGxYdv5ne4en"
        EOS -> "EOS6hs8sRvGSzuQtq223zwJipMzqTJpXUVjyvHPvPwBSZWWrJTJkg"
        WAX -> "EOS6hs8sRvGSzuQtq223zwJipMzqTJpXUVjyvHPvPwBSZWWrJTJkg"
        IoTeX -> "io1qw9cccecw09q7p5kzyqtuhfhvah2mhfrc84jfk"
        IoTeXEVM -> "0x038B8C633873Ca0f06961100BE5d37676EADDD23"
        Zilliqa -> "zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6"
        Zelcash -> "t1UKbRPzL4WN8Rs8aZ8RNiWoD2ftCMHKGUf"
        Ravencoin -> "RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"
        Waves -> "3P63vkaHhyE9pPv9EfsjwGKqmZYcCRHys4n"
        Aeternity -> "ak_QDHJSfvHG9sDHBobaWt2TAGhuhipYjEqZEH34bWugpJfJc3GN"
        Terra, TerraV2 -> "terra1rh402g98t7sly8trzqw5cyracntlep6qe3smug"
        Monacoin -> "M9xFZzZdZhCDxpx42cM8bQHnLwaeX1aNja"
        FIO -> "FIO7MN1LuSfFgrbVHmrt9cVa2FYAs857Ppr9dzvEXoD1miKSxm3n3"
        Harmony -> "one12fk20wmvgypdkn59n4hq8e3aa5899xfx4vsu09"
        Solana -> "2bUBiBNZyD29gP1oV6de7nxowMLoDBtopMMTGgMvjG5m"
        Algorand -> "JTJWO524JXIHVPGBDWFLJE7XUIA32ECOZOBLF2QP3V5TQBT3NKZSCG67BQ"
        Acala -> "25GGezx3LWFQj6HZpYzoWoVzLsHojGtybef3vthC9nd19ms3"
        Kusama -> "G9xV2EatmrjRC1FLPexc3ddqNRRzCsAdURU8RFiAAJX6ppY"
        Polkadot -> "13nN6BGAoJwd7Nw1XxeBCx5YcBXuYnL94Mh7i3xBprqVSsFk"
        Pivx -> "D81AqC8zKma3Cht4TbVuh4jyVVyLkZULCm"
        Kava -> "kava1drpa0x9ptz0fql3frv562rcrhj2nstuz3pas87"
        Cardano -> "addr1qyr8jjfnypp95eq74aqzn7ss687ehxclgj7mu6gratmg3mul2040vt35dypp042awzsjk5xm3zr3zm5qh7454uwdv08s84ray2"
        NEO -> "AT6w7PJvwPcSqHvtbNBY2aHPDv12eW5Uuf"
        Filecoin -> "f1zzykebxldfcakj5wdb5n3n7priul522fnmjzori"
        MultiversX -> "erd1jfcy8aeru6vlx4fe6h3pc3vlpe2cnnur5zetxdhp879yagq7vqvs8na4f8"
        BandChain -> "band1624hqgend0s3d94z68fyka2y5jak6vd7u0l50r"
        SmartChainLegacy -> "0x49784f90176D8D9d4A3feCDE7C1373dAAb5b13b8"
        Oasis -> "oasis1qzcpavvmuw280dk0kd4lxjhtpf0u3ll27yf7sqps"
        THORChain -> "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65"
        IOST -> "4av8w81EyzUgHonsVWqfs15WM4Vrpgox4BYYQWhNQDVu"
        Syscoin -> "sys1qkl640se3mwpt666e3lyywnwh09e9jquvx9x8qj"
        Stratis -> "strax1q0caanaw4nkf6fzwnzq2p7yum680e57pdg05zkm"
        Bluzelle -> "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        CryptoOrg -> "cro16fdf785ejm00jf9a24d23pzqzjh2h05klxjwu8"
        Osmosis -> "osmo142j9u5eaduzd7faumygud6ruhdwme98qclefqp"
        ECash -> "ecash:qpelrdn7a0hcucjlf9ascz3lkxv7r3rffgzn6x5377"
        NativeEvmos -> "evmos13u6g7vqgw074mgmf2ze2cadzvkz9snlwstd20d"
        Nervos -> "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02wectaumxn0664yw2jd53lqk4mxg3"
        Everscale -> "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04"
        TON -> "EQDgEMqToTacHic7SnvnPFmvceG5auFkCcAw0mSCvzvKUfk9"
        Aptos -> "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
        Nebl -> "NgDVaXAwNgBwb88xLiFKomfBmPkEh9F2d7"
        Sui -> "0xada112cfb90b44ba889cc5d39ac2bf46281e4a91f7919c693bcd9b8323e81ed2"
        Hedera -> "0.0.302a300506032b657003210049eba62f64d0d941045595d9433e65d84ecc46bcdb1421de55e05fcf2d8357d5"
        Secret -> "secret1f69sk5033zcdr2p2yf3xjehn7xvgdeq09d2llh"
        NativeInjective -> "inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a"
        Agoric -> "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5"
        Stargaze -> "stars142j9u5eaduzd7faumygud6ruhdwme98qycayaz"
        Juno -> "juno142j9u5eaduzd7faumygud6ruhdwme98qxkfz30"
        Stride -> "stride142j9u5eaduzd7faumygud6ruhdwme98qn029zl"
        Axelar -> "axelar142j9u5eaduzd7faumygud6ruhdwme98q52u3aj"
        Crescent -> "cre142j9u5eaduzd7faumygud6ruhdwme98q5veur7"
        Kujira -> "kujira142j9u5eaduzd7faumygud6ruhdwme98qpvgpme"
        NativeCanto -> "canto13u6g7vqgw074mgmf2ze2cadzvkz9snlwqua5pd"
        Comdex -> "comdex142j9u5eaduzd7faumygud6ruhdwme98qhtgm0y"
        Neutron -> "neutron142j9u5eaduzd7faumygud6ruhdwme98q5mrmv5"
        Sommelier -> "somm142j9u5eaduzd7faumygud6ruhdwme98quc948e"
        FetchAI -> "fetch142j9u5eaduzd7faumygud6ruhdwme98qrera5y"
        Mars -> "mars142j9u5eaduzd7faumygud6ruhdwme98qdenqrg"
        Umee -> "umee142j9u5eaduzd7faumygud6ruhdwme98qzjhxjp"
        Coreum -> "core1rawf376jz2lnchgc4wzf4h9c77neg3zldc7xa8"
        Quasar -> "quasar142j9u5eaduzd7faumygud6ruhdwme98q78symk"
        Persistence -> "persistence142j9u5eaduzd7faumygud6ruhdwme98q7gv2ch"
        Akash -> "akash142j9u5eaduzd7faumygud6ruhdwme98qal870f"
        Noble -> "noble142j9u5eaduzd7faumygud6ruhdwme98qc8l3wa"
        Rootstock -> "0xA2D7065F94F838a3aB9C04D67B312056846424Df"
        Sei -> "sei142j9u5eaduzd7faumygud6ruhdwme98qagm0sj"
        InternetComputer -> "6f8e568160a3c8362789848dc0fa52891964473c045cc25208a305fb35b7c4ab"
        Tia -> "celestia142j9u5eaduzd7faumygud6ruhdwme98qpwmfv7"
        NativeZetaChain -> "zeta13u6g7vqgw074mgmf2ze2cadzvkz9snlwywj304"
        Dydx -> "dydx142j9u5eaduzd7faumygud6ruhdwme98qeayaky"
    }
}
