// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class NativeInjectiveTests: XCTestCase {
    
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .nativeInjective)
        let addressFromString = AnyAddress(string: "inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a", coin: .nativeInjective)!

        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .nativeInjective)

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = CosmosMessage.Send.with {
                $0.fromAddress = fromAddress.description
                $0.toAddress = "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd"
                $0.amounts = [CosmosAmount.with {
                    $0.amount = "10000000000"
                    $0.denom = "inj"
                }]
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 110000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100000000000000"
                $0.denom = "inj"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 17396
            $0.chainID = "injective-1"
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .nativeInjective)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajEzdTZnN3ZxZ3cwNzRtZ21mMnplMmNhZHp2a3o5c25sd2NydHE4YRIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASngEKfgp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXMSBAoCCAEYARIcChYKA2luahIPMTAwMDAwMDAwMDAwMDAwELDbBhpAx2vkplmzeK7n3puCFGPWhLd0l/ZC/CYkGl+stH+3S3hiCvIe7uwwMpUlNaSwvT8HwF1kNUp+Sx2m0Uo1x5xcFw==\"}")
    }
}
