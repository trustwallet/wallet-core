// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class PolkadotTests: XCTestCase {

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

    func testSigningBond() {
        // https://polkadot.subscan.io/extrinsic/0x5ec2ec6633b4b6993d9cf889ef42c457a99676244dc361a9ae17935d331dc39a
        // real key in 1p test
        let wallet = HDWallet.test
        let key = wallet.getKey(derivationPath: "m/44'/354'/0'")
        print(key.data.hexString)

        let address = CoinType.polkadot.deriveAddress(privateKey: key)
        print(address)
        let genesisHash = Data(hexString: "0x91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3")!
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
                // 0.01
                $0.value = Data(hexString: "0x02540be400")!
            }
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .polkadot)

        XCTAssertEqual(output.encoded.hexString, "3902848d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa00a559867d1304cc95bac7cfe5d1b2fd49aed9f43c25c7d29b9b01c1238fa1f6ffef34b9650e42325de41e20fd502af7b074c67a9ec858bd9a1ba6d4212e3e0d0f00000007008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0700e40b540200")
    }
}
