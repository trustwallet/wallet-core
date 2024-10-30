// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class PactusTests: XCTestCase {
    var privateKey: PrivateKey!

    override func setUp() {
        super.setUp()
        privateKey = PrivateKey(data: Data(hexString: "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6")!)!
    }

   func testAddress() {
        let pubkey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .pactus)
        let addressFromString = AnyAddress(string: "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr", coin: .pactus)!

        XCTAssertEqual(pubkey.data.hexString, "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testTransferSign() {
        let input = PactusSigningInput.with {
            $0.privateKey = privateKey.data
            $0.transaction = PactusTransactionMessage.with {
                $0.lockTime = 0x24a32300
                $0.fee = 10000000
                $0.memo = "wallet-core"
                $0.transfer = PactusTransferPayload.with {
                    $0.sender = "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr"
                    $0.receiver = "pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra"
                    $0.amount = 200000000
                }
            }
        }

        let output: PactusSigningOutput = AnySigner.sign(input: input, coin: .pactus)

        let expectedTransactionID = "1b6b7226f7935a15f05371d1a1fefead585a89704ce464b7cc1d453d299d235f"
        let expectedSignature = "4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda8506"
        let expectedSignedData = "000124a3230080ade2040b77616c6c65742d636f726501037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df218084af5f4ed8fee3d8992e82660dd05bbe8608fc56ceabffdeeee61e3213b9b49d33a0fc8dea6d79ee7ec60f66433f189ed9b3c50b2ad6fa004e26790ee736693eda850695794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa"
        XCTAssertEqual(output.transactionID.hexString, expectedTransactionID)
        XCTAssertEqual(output.signature.hexString, expectedSignature)
        XCTAssertEqual(output.signedTransactionData.hexString, expectedSignedData)
    }

    func testBondSign() {
        let input = PactusSigningInput.with {
            $0.privateKey = privateKey.data
            $0.transaction = PactusTransactionMessage.with {
                $0.lockTime = 0xc1b02300
                $0.fee = 10000000
                $0.memo = "wallet-core"
                $0.bond = PactusBondPayload.with {
                    $0.sender = "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr"
                    $0.receiver = "pc1p9y5gmu9l002tt60wak9extgvwm69rq3a9ackrl"
                    $0.stake = 1000000000
                    $0.public_key = "public1pnz75msstqdrq5eguvcwanug0zauhqjw2cc4flmez3qethnp68y64ehc4k69amapj7x4na2uda0snqz4yxujgx3jsse4f64fgy7jkh0xauvhrc5ts09vfk48g85t0js66hvajm6xruemsvlxqv3xvkyur8v9v0mtn"
                }
            }
        }

        let output: PactusSigningOutput = AnySigner.sign(input: input, coin: .pactus)

        let expectedTransactionID = "d194b445642a04ec78ced4448696e50b733f2f0b517a23871882c0eefaf1c28f"
        let expectedSignature = "0d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac79d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce7006300"
        let expectedSignedData = "0001c1b0230080ade2040b77616c6c65742d636f726502037098338e0b6808119dfd4457ab806b9c2059b89b0129288df0bf7bd4b5e9eeed8b932d0c76f451823d6098bd4dc20b03460a651c661dd9f10f17797049cac62a9fef228832bbcc3a39355cdf15b68bddf432f1ab3eab8debe1300aa43724834650866a9d552827a56bbcdde32e3c517079589b54e83d16f9435abb3b2de8c3e677067cc0644ccb13833b8094ebdc030d7bc6d94927534b89e2f53bcfc9fc849e0e2982438955eda55b4338328adac79d4ee3216d143f0e1629764ab650734f8ba188e716d71f9eff65e39ce700630095794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa"
        XCTAssertEqual(output.transactionID.hexString, expectedTransactionID)
        XCTAssertEqual(output.signature.hexString, expectedSignature)
        XCTAssertEqual(output.signedTransactionData.hexString, expectedSignedData)
    }
}
