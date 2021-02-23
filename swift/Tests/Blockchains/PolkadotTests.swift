// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class PolkadotTests: XCTestCase {

    let genesisHash = Data(hexString: "0x91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3")!
    let privateKeyThrow2 = Data(hexString: "0x70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f")!

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
            $0.network = .polkadot
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

        // https://polkadot.subscan.io/extrinsic/0x72dd5b5a3e01e0f3e779c5fa39e53de05ee381b9138d24e2791a775a6d1ff679
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
            $0.network = .polkadot
            $0.transactionVersion = 3
            $0.privateKey = key.data
            $0.stakingCall.bond = PolkadotStaking.Bond.with {
                $0.controller = address
                $0.rewardDestination = .staked
                $0.value = Data(hexString: "0x02540be400")! // 0.01 old DOT
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/0x5ec2ec6633b4b6993d9cf889ef42c457a99676244dc361a9ae17935d331dc39a
        XCTAssertEqual(output.encoded.hexString, "3902848d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa00a559867d1304cc95bac7cfe5d1b2fd49aed9f43c25c7d29b9b01c1238fa1f6ffef34b9650e42325de41e20fd502af7b074c67a9ec858bd9a1ba6d4212e3e0d0f00000007008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0700e40b540200")
    }

    func testSigningBondAndNominate() {
        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = Data(hexString: "0x3a886617f4bbd4fe2bbe7369acae4163ed0b19ffbf061083abc5e0836ad58f77")!
            $0.nonce = 6
            $0.specVersion = 27
            $0.network = .polkadot
            $0.transactionVersion = 5
            $0.privateKey = privateKeyThrow2
            $0.era = PolkadotEra.with {
                $0.blockNumber = 3856651
                $0.period = 64
            }
            $0.stakingCall.bondAndNominate = PolkadotStaking.BondAndNominate.with {
                $0.controller = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2"
                $0.value = Data(hexString: "0x77359400")! // 0.2
                $0.rewardDestination = .stash
                $0.nominators = ["14xKzzU1ZYDnzFj7FgdtDAYSMJNARjDc2gNw4XAFDgr4uXgp", "1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih"]
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        // https://polkadot.subscan.io/extrinsic/0xc7a016f961dbf35d58feea22694e7d79ac77175a8cc40cb017bb5e87d56142ce
        XCTAssertEqual(output.encoded.hexString, "5103849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783007d549324f270eb5932b898ce5fc166c3f30942c96668f52d6cc86c7b61a8d65680cd0a979f1e0a43ef9418e6571edab6d9c391a1696abdf56db2af348862d50eb50018001a000807009dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783030094357701070508aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f610127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a")
    }
}
