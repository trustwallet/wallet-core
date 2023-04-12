// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AptosTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "0x6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108", coin: .aptos)

        XCTAssertEqual(anyAddress?.description, "0x6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108")
        XCTAssertEqual(anyAddress?.coin, .aptos)

        let invalid = "MQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .aptos))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .aptos))
    }

    func testSign() {
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xb4d62afd3862116e060dd6ad9848ccb50c2bc177799819f1d29c059ae2042467?network=devnet
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let transferMsg = AptosTransferMessage.with {
            $0.to = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.amount = 1000
        }
        let input = AptosSigningInput.with {
            $0.chainID = 33
            $0.sender = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.expirationTimestampSecs = 3664390082
            $0.gasUnitPrice = 100
            $0.maxGasAmount = 3296766
            $0.sequenceNumber = 99
            $0.transfer = transferMsg
            $0.privateKey = privateKeyData
        }
        let output: AptosSigningOutput = AnySigner.sign(input: input, coin: .aptos)
        let expectedRawTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021"
        let expectedSignature = "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01"
        let expectedSignedTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01"
        XCTAssertEqual(output.rawTxn.hexString, expectedRawTx)
        XCTAssertEqual(output.authenticator.signature.hexString, expectedSignature)
        XCTAssertEqual(output.encoded.hexString, expectedSignedTx)
    }
}
