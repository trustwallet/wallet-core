// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
        let privateKeyData = Data(hexString: "542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4")!

        let transfer = EverscaleTransfer.with {
            $0.bounce = false
            $0.behavior = EverscaleMessageBehavior.simpleTransfer
            $0.amount = 100000000
            $0.expiredAt = 1680770631
            $0.to = "0:db18a67f4626f15ac0537a18445937f685f9b30184f0d7b28be4bdeb92d2fd90"
            $0.encodedContractData = "te6ccgEBAQEAKgAAUAAAAAFLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupw="
        }

        let input = EverscaleSigningInput.with {
            $0.transfer = transfer
            $0.privateKey = privateKeyData
        }

        let output: EverscaleSigningOutput = AnySigner.sign(input: input, coin: .everscale)

        // Link to the message: https://everscan.io/messages/73807b0a3ca2d8564c023dccd5b9da222a270f68338c6fc2c064dda376a2c59d
        let expectedString = "te6ccgICAAIAAQAAAKoAAAHfiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrAImASIQKH2jIwoA65IGC6aua4gAA4fFo/Nuxgb3sIRELhZnSXIS7IsE2E4D+8hk3EWGVZX+ICqlN/ka9DvXduhaXUlsUyF0MjgAAAAIHAABAGhCAG2MUz+jE3itYCm9DCIsm/tC/NmAwnhr2UXyXvXJaX7IIC+vCAAAAAAAAAAAAAAAAAAA"

        XCTAssertEqual(output.encoded, expectedString)
    }
}
