// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class PolkadotTests: XCTestCase {

    let genesisHash = Data(hexString: "0x91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3")!

    func testAddressValidation() {
        let polkadot = CoinType.polkadot
        // cosmos
        XCTAssertFalse(polkadot.validate(address: "cosmos1l4f4max9w06gqrvsxf74hhyzuqhu2l3zyf0480"))
        // Bitcoin p2sh
        XCTAssertFalse(polkadot.validate(address: "3317oFJC9FvxU2fwrKVsvgnMGCDzTZ5nyf"))
        // Kusama
        XCTAssertFalse(polkadot.validate(address: "ELmaX1aPkyEF7TSmYbbyCjmSgrBpGHv9EtpwR2tk1kmpwvG"))
        // polkadot sr25519
        XCTAssertTrue(polkadot.validate(address: "14PhJGbzPxhQbiq7k9uFjDQx3MNiYxnjFRSiVBvBBBfnkAoM"))
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0xd65ed4c1a742699b2e20c0c1f1fe780878b1b9f7d387f934fe0a7dc36f1f9008")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .polkadot)
        let addressFromString = AnyAddress(string: "12twBQPiG5yVSf3jQSBkTAKBKqCShQ5fm33KQhH3Hf6VDoKW", coin: .polkadot)!

        XCTAssertEqual(pubkey.data.hexString, "53d82211c4aadb8c67e1930caef2058a93bc29d7af86bf587fba4aa3b1515037")
        XCTAssertEqual(address.description, addressFromString.description)
        XCTAssertEqual(address.data, pubkey.data)
    }

    func testSignTransfer() {
        // real key in 1p test
        let wallet = HDWallet.test
        let key = wallet.getKey(coin: .polkadot, derivationPath: "m/44'/354'/0'")

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = Data(hexString: "0x7d5fa17b70251d0806f26156b1b698dfd09e040642fa092595ce0a78e9e84fcd")!
            $0.nonce = 1
            $0.specVersion = 28
            $0.network = CoinType.polkadot.ss58Prefix
            $0.transactionVersion = 6
            $0.privateKey = key.data
            $0.era = PolkadotEra.with {
                $0.blockNumber = 3910736
                $0.period = 64
            }
            $0.balanceCall.transfer = PolkadotBalance.Transfer.with {
                $0.toAddress = "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5"
                $0.value = Data(hexString: "0x02540be400")! // 1 DOT
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/3910744-1
        XCTAssertEqual(output.encoded.hexString, "410284008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0038ec4973ab9773dfcbf170b8d27d36d89b85c3145e038d68914de83cf1f7aca24af64c55ec51ba9f45c5a4d74a9917dee380e9171108921c3e5546e05be15206050104000500007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402")
    }

    func testSigningBond() {
        // real key in 1p test
        let wallet = HDWallet.test
        let key = wallet.getKey(coin: .polkadot, derivationPath: "m/44'/354'/0'")
        let address = CoinType.polkadot.deriveAddress(privateKey: key)

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 0
            $0.specVersion = 17
            $0.network = CoinType.polkadot.ss58Prefix
            $0.transactionVersion = 3
            $0.privateKey = key.data
            $0.stakingCall.bond = PolkadotStaking.Bond.with {
                $0.controller = address
                $0.rewardDestination = .staked
                $0.value = Data(hexString: "0x02540be400")! // 0.01 old DOT
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/985084-3
        XCTAssertEqual(output.encoded.hexString, "3902848d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa00a559867d1304cc95bac7cfe5d1b2fd49aed9f43c25c7d29b9b01c1238fa1f6ffef34b9650e42325de41e20fd502af7b074c67a9ec858bd9a1ba6d4212e3e0d0f00000007008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0700e40b540200")
    }

    func testSigningBondAndNominate() {
        // real key in 1p test
        let key = HDWallet.test.getKeyForCoin(coin: .polkadot)

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 4
            $0.specVersion = 30
            $0.network = CoinType.polkadot.ss58Prefix
            $0.transactionVersion = 7
            $0.privateKey = key.data
            $0.stakingCall.bondAndNominate = PolkadotStaking.BondAndNominate.with {
                $0.controller = "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5"
                $0.value = Data(hexString: "0x02540be400")! // 1 DOT
                $0.rewardDestination = .stash
                $0.nominators = [
                    "1zugcavYA9yCuYwiEYeMHNJm9gXznYjNfXQjZsZukF1Mpow", // Zug Capital / 12
                    "15oKi7HoBQbwwdQc47k71q4sJJWnu5opn1pqoGx4NAEYZSHs" // P2P Validator
                ]
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/4955314-2
        XCTAssertEqual("0x" + output.encoded.hexString, "0x6103840036092fac541e0e5feda19e537c679b487566d7101141c203ac8322c27e5f076a00a8b1f859d788f11a958e98b731358f89cf3fdd41a667ea992522e8d4f46915f4c03a1896f2ac54bdc5f16e2ce8a2a3bf233d02aad8192332afd2113ed6688e0d0010001a02080700007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b540201070508002c2a55b5ffdca266bd0207df97565b03255f70783ca1a349be5ed9f44589c36000d44533a4d21fd9d6f5d57c8cd05c61a6f23f9131cec8ae386b6b437db399ec3d")
    }

    func testSigningBondExtra() {
        // real key in 1p test
        let key = HDWallet.test.getKeyForCoin(coin: .polkadot)

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 5
            $0.specVersion = 30
            $0.network = CoinType.polkadot.ss58Prefix
            $0.transactionVersion = 7
            $0.privateKey = key.data
            $0.stakingCall.bondExtra = PolkadotStaking.BondExtra.with {
                $0.value = Data(hexString: "0x77359400")! // 0.2 DOT
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/4999416-1
        XCTAssertEqual("0x" + output.encoded.hexString, "0xb501840036092fac541e0e5feda19e537c679b487566d7101141c203ac8322c27e5f076a00c8268c2dfd4074f41d225e12e62e5975ff8debf0f828d31ddbfed6f7593e067fb860298eb12f50294f7ba0f82795809c84fc5cce6fcb36cde4cb1c07edbbb60900140007010300943577")
    }

    func testChillAndUnbond() {
        // real key in 1p test
        let key = PrivateKey(data: Data(hexString: "298fcced2b497ed48367261d8340f647b3fca2d9415d57c2e3c5ef90482a2266")!)!

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = Data(hexString: "0x35ba668bb19453e8da6334cadcef2a27c8d4141bfc8b49e78e853c3d73e1ecd0")!
            $0.era = PolkadotEra.with {
                $0.blockNumber = 10541373
                $0.period = 64
            }
            $0.nonce = 6
            $0.specVersion = 9200
            $0.network = CoinType.polkadot.ss58Prefix
            $0.transactionVersion = 12
            $0.privateKey = key.data
            $0.stakingCall.chillAndUnbond = PolkadotStaking.ChillAndUnbond.with {
                $0.value = Data(hexString: "0x1766444D00")! // 10.05 DOT
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/10541383-2
        XCTAssertEqual("0x" + output.encoded.hexString, "0xd10184008361bd08ddca5fda28b5e2aa84dc2621de566e23e089e555a42194c3eaf2da7900c891ba102db672e378945d74cf7f399226a76b43cab502436971599255451597fc2599902e4b62c7ce85ecc3f653c693fef3232be620984b5bb5bcecbbd7b209d50318001a02080706070207004d446617")
    }
    
    func testAcalaSigning() {
        // real key in 1p test
        let key = PrivateKey(data: Data(hexString: "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0")!)!
        
        let acalaGenesisHash = Data(hexString: "0xfc41b9bd8ef8fe53d58c7ea67c794c7ec9a73daf05e6d54b14ff6342c99ba64c")!

        let input = PolkadotSigningInput.with {
            $0.genesisHash = acalaGenesisHash
            $0.blockHash = Data(hexString: "0x707ffa05b7dc6cdb6356bd8bd51ff20b2757c3214a76277516080a10f1bc7537")!
            $0.era = PolkadotEra.with {
                $0.blockNumber = 3893613
                $0.period = 64
            }
            $0.nonce = 0
            $0.specVersion = 2170
            $0.network = 10 // Acala
            $0.transactionVersion = 2
            $0.privateKey = key.data
            $0.balanceCall.transfer = PolkadotBalance.Transfer.with {
                $0.value = Data(hexString: "0xe8d4a51000")! // 1 ACA
                $0.toAddress = "25Qqz3ARAvnZbahGZUzV3xpP1bB3eRrupEprK7f2FNbHbvsz"
                $0.callIndices = PolkadotCallIndices.with {
                    $0.custom = PolkadotCustomCallIndices.with {
                        $0.moduleIndex = 0x0a
                        $0.methodIndex = 0x00
                    }
                }
            }
            $0.multiAddress = true
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://acala.subscan.io/extrinsic/3893620-3
        XCTAssertEqual("0x" + output.encoded.hexString, "0x41028400e9590e4d99264a14a85e21e69537e4a64f66a875d38cb8f76b305f41fabe24a900dd54466dffd1e3c80b76013e9459fbdcd17805bd5fdbca0961a643bad1cbd2b7fe005c62c51c18b67f31eb9e61b187a911952fee172ef18402d07c703eec3100d50200000a0000c8c602ded977c56076ae38d98026fa669ca10d6a2b5a0bfc4086ae7668ed1c60070010a5d4e8")
    }
}
