// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class EverscaleTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(hexString: "15d126cb1a84acdbcd1d9c3f6975968c2beb18cc43c95849d4b0226e1c8552aa")!)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .everscale)
        XCTAssertEqual(address.description, "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04")
    }

    func testAddressFromPublicKey() {
        let publicKey = PublicKey(data: Data(hexString: "a0303f8fc89a3c2124f5dc6f3ab9a9cb246b7d1e24897eaf5e63eeee20085db0")!, type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .everscale)
        XCTAssertEqual(address.description, "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04")
    }

    func testAddressFromString() {
        let addressString = "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04"
        let address = AnyAddress(string: addressString, coin: .everscale)
        XCTAssertEqual(address!.description, "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04")
    }

    func testSign() throws {
        let privateKeyData = Data(hexString: "5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f")!

        let transfer = EverscaleTransfer.with {
            $0.bounce = false
            $0.flags = 3
            $0.amount = 1000000000
            $0.expiredAt = 1659026078
        }

        let input = EverscaleSigningInput.with {
            $0.transfer = transfer
            $0.privateKey = privateKeyData
        }

        let output: EverscaleSigningOutput = AnySigner.sign(input: input, coin: .everscale)

        let expectedString = "te6ccgICAAIAAQAAANoAAAHfiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYAIyIRhBmub4CthfowsK/+nfwhiZ3IQvYenZ/V6xgpkf/hCkNtlh1+9IxezU0xS9A0e1mZD9f1QzBKNMJzMGmqAKXUlsUxcV1PAAAAAAHAABAGhCABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYIdzWUAAAAAAAAAAAAAAAAAAA"

        XCTAssertEqual(output.encoded, expectedString)
    }
}
