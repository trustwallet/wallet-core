// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class PolymeshTests: XCTestCase {
    let genesisHash = Data(hexString: "0x6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063")!
    // Private key for testing.  DO NOT USE, since this is public.
    let testKey1 = Data(hexString: "0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4")!

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .polymesh)
        let addressFromString = AnyAddress(string: "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys", coin: .polymesh)!

        XCTAssertEqual(pubkey.data.hexString, "4bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSignTransfer() {
        // https://polymesh.subscan.io/extrinsic/0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04

        // Step 1: Prepare input.
        let blockHash = Data(hexString: "77d32517dcc7b74501096afdcff3af72008a2c489e17083f56629d195e5c6a1d")!

        let input = PolymeshSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = blockHash
            $0.nonce = 1
            $0.specVersion = 7000005
            $0.network = CoinType.polymesh.ss58Prefix
            $0.transactionVersion = 7
            $0.privateKey = testKey1
            $0.era = PolkadotEra.with {
                $0.blockNumber = 16102106
                $0.period = 64
            }
            $0.runtimeCall = PolymeshRuntimeCall.with {
                $0.balanceCall.transfer = PolymeshBalance.Transfer.with {
                    $0.toAddress = "2CpqFh8VnwJAjenw4xSUWCaaJ2QwGdhnCikoSEczMhjgqyj7"
                    $0.value = Data(hexString: "0x0F4240")! // 1.0 POLYX
                }
            }
        }
        let output: PolymeshSigningOutput = AnySigner.sign(input: input, coin: .polymesh)

        XCTAssertEqual(output.encoded.hexString, "390284004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210ba501040005000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00")
    }
}
