// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class KusamaTests: XCTestCase {

    func testAddressValidation() {
        let kusama = CoinType.kusama
        // polkadot sr25519
        XCTAssertFalse(kusama.validate(address: "14PhJGbzPxhQbiq7k9uFjDQx3MNiYxnjFRSiVBvBBBfnkAoM"))
        // cosmos
        XCTAssertFalse(kusama.validate(address: "cosmos1l4f4max9w06gqrvsxf74hhyzuqhu2l3zyf0480"))
        // Bitcoin p2sh
        XCTAssertFalse(kusama.validate(address: "3317oFJC9FvxU2fwrKVsvgnMGCDzTZ5nyf"))

        XCTAssertTrue(kusama.validate(address: "ELmaX1aPkyEF7TSmYbbyCjmSgrBpGHv9EtpwR2tk1kmpwvG"))
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x85fca134b3fe3fd523d8b528608d803890e26c93c86dc3d97b8d59c7b3540c97")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .kusama)
        let addressFromString = AnyAddress(string: "HewiDTQv92L2bVtkziZC8ASxrFUxr6ajQ62RXAnwQ8FDVmg", coin: .kusama)!

        XCTAssertEqual(pubkey.data.hexString, "e0b3fcccfe0283cc0f8c105c68b5690aab8c5c1692a868e55eaca836c8779085")
        XCTAssertEqual(address.description, addressFromString.description)
        XCTAssertEqual(address.data.hexString, pubkey.data.hexString)
    }

    func testSigningTransfer() {
        // https://kusama.subscan.io/extrinsic/0x9211b8f6500c78f4771d18289c6187ec59c2b1fb28e8324ee32a1f9a3303be7e
        // real key in 1p test
        let wallet = HDWallet.test
        let key = wallet.getKey(coin: .kusama, derivationPath: "m/44'/434'/0'")

        let genesisHash = Data(hexString: "0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe")!
        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 1
            $0.specVersion = 2019
            $0.balanceCall = PolkadotBalance.with {
                $0.transfer = PolkadotBalance.Transfer.with {
                    $0.toAddress = "CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY"
                    // 10000000000 ~ 0.01 KSM
                    $0.value = Data(hexString: "0x02540be400")!
                }
            }
            $0.network = CoinType.kusama.ss58Prefix
            $0.transactionVersion = 2
            $0.privateKey = key.data
        }
        let output: PolkadotSigningOutput = AnySigner.sign(input: input, coin: .kusama)

        XCTAssertEqual(output.encoded.hexString, "350284f41296779fd61a5bed6c2f506cc6c9ea93d6aeb357b9c69717193f434ba24ae700cd78b46eff36c433e642d7e9830805aab4f43eef70067ef32c8b2a294c510673a841c5f8a6e8900c03be40cfa475ae53e6f8aa61961563cb7cc0fa169ef9630d00040004000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc720700e40b5402")
    }
}
