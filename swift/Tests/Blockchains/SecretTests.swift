// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class SecretTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "87201512d132ef7a1e57f9e24905fbc24300bd73f676b5716182be5f3e39dada")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .secret)
        let addressFromString = AnyAddress(string: "secret18mdrja40gfuftt5yx6tgj0fn5lurplezyp894y", coin: .secret)!

        XCTAssertEqual(pubkey.data.hexString, "02466ac5d28cb4fab6c349060c6c1619e8d301e7741fb6b33cc1edac25f45d8646")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "87201512d132ef7a1e57f9e24905fbc24300bd73f676b5716182be5f3e39dada")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .secret)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "secret1rnq6hjfnalxeef87rmdeya3nu9dhpc7k9pujs3"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100000"
                $0.denom = "uscrt"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 25000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "2500"
                $0.denom = "uscrt"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 265538
            $0.chainID = "secret-4"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .secret)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLXNlY3JldDE4bWRyamE0MGdmdWZ0dDV5eDZ0Z2owZm41bHVycGxlenlwODk0eRItc2VjcmV0MXJucTZoamZuYWx4ZWVmODdybWRleWEzbnU5ZGhwYzdrOXB1anMzGg8KBXVzY3J0EgYxMDAwMDASZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAkZqxdKMtPq2w0kGDGwWGejTAed0H7azPMHtrCX0XYZGEgQKAggBGAESEwoNCgV1c2NydBIEMjUwMBCowwEaQOcHd2gHpa5WKZ/5RRerEtrHlyDlojIEzUGhC9xMFgs7UQMWy+kTTN+NRf7zQ8rx3cPkIKeZhv0u1KRc8uRCc4o=\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
}
