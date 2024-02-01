// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class AcalaTests: XCTestCase {

    let genesisHash = Data(hexString: "0xfc41b9bd8ef8fe53d58c7ea67c794c7ec9a73daf05e6d54b14ff6342c99ba64c")!

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .acala)
        let addressFromString = AnyAddress(string: "269ZCS3WLGydTN8ynhyhZfzJrXkePUcdhwgLQs6TWFs5wVL5", coin: .acala)!

        XCTAssertEqual(address.description, addressFromString.description)
        XCTAssertEqual(address.data, pubkey.data)
    }

    func testSigning() {
        // real key in 1p test
        let key = PrivateKey(data: Data(hexString: "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0")!)!

        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = Data(hexString: "0x707ffa05b7dc6cdb6356bd8bd51ff20b2757c3214a76277516080a10f1bc7537")!
            $0.era = PolkadotEra.with {
                $0.blockNumber = 3893613
                $0.period = 64
            }
            $0.nonce = 0
            $0.specVersion = 2170
            $0.network = CoinType.acala.ss58Prefix
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
