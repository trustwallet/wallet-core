// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class AgoricTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .agoric)
        let addressFromString = AnyAddress(string: "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5", coin: .agoric)!

        XCTAssertEqual(pubkey.data.hexString, "03df9a5e4089f89d45913fb2b856de984c7e8bf1344cc6444cc9705899a48c939d")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .agoric)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "agoric1cqvwa8jr6pmt45jndanc8lqmdsxjkhw0yertc0"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1"
                $0.denom = "ubld"
            }]
        }


        let fee = CosmosFee.with {
            $0.gas = 100000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "2000"
                $0.denom = "ubld"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 62972
            $0.chainID = "agoric-3"
            $0.sequence = 1
            $0.messages = [
                CosmosMessage.with {
                    $0.sendCoinsMessage = sendCoinsMessage
                }
            ]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .agoric)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWFnb3JpYzE4enZ2Z2s2ajNlcTV3ZDdtcXhjY2d0MjBnejJ3OTRjeTg4YWVrNRItYWdvcmljMWNxdndhOGpyNnBtdDQ1am5kYW5jOGxxbWRzeGpraHcweWVydGMwGgkKBHVibGQSATESZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA9+aXkCJ+J1FkT+yuFbemEx+i/E0TMZETMlwWJmkjJOdEgQKAggBGAESEgoMCgR1YmxkEgQyMDAwEKCNBhpAenbGO4UBK610dwSY6l5pl58qwHW1OujQ/9vF9unQdrA1SE0b/2mZxnevy5y3u6pJfBffWUfCx68PcVEu7D3EYQ==\"}")
    }
}
