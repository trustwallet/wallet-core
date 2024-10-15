// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class PactusTests: XCTestCase {

   func testAddress() {
        let key = PrivateKey(data: Data(hexString: "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .pactus)
        let addressFromString = AnyAddress(string: "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr", coin: .pactus)!

        XCTAssertEqual(pubkey.data.hexString, "0x95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let transfer = PactusTransferPayload.with {
            $0.sender = "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr"
            $0.receiver = "pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra"
            $0.amount = 20000
        }
        let transaction = PactusTransactionMessage.with {
            $0.lockTime(0x00030201)
            $0.fee(1000)
            $0.memo("test")
            $0.transfer(transfer)
        }
        let input = PactusSigningInput.with {
            $0.privateKey = privateKeyData
            $0.transaction = transfer
        }

        let output: PactusSigningOutput = AnySigner.sign(input: input, coin: .Pactus)
        let expectedTransactionID = "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311"
        let expectedSignature = "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09"
        let expectedSignedData = "000101020300e807047465737401037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21a09c0150ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd0995794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa"
        XCTAssertEqual(output.transactionID.hexString, expectedTransactionID)
        XCTAssertEqual(output.signature.hexString, expectedSignature)
        XCTAssertEqual(output.signedTransactionData.hexString, expectedSiexpectedSignedDatanedTx)
    }
}
