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
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xbb3b3c33781c27e486afa2db854fb0a5c846d0967672feb2c6c3297a2b14e1ce?network=Devnet
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let transferMsg = AptosTransferMessage.with {
            $0.to = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.amount = 1000
        }
        let input = AptosSigningInput.with {
            $0.chainID = 32
            $0.sender = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.expirationTimestampSecs = 3664390082
            $0.gasUnitPrice = 100
            $0.maxGasAmount = 3296766
            $0.sequenceNumber = 15
            $0.transfer = transferMsg
            $0.privateKey = privateKeyData
        }
        let output: AptosSigningOutput = AnySigner.sign(input: input, coin: .aptos)
        let expectedRawTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000020"
        let expectedSignature = "2ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05"
        let expectedSignedTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c402ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05"
        XCTAssertEqual(output.rawTxn.hexString, expectedRawTx)
        XCTAssertEqual(output.authenticator.signature.hexString, expectedSignature)
        XCTAssertEqual(output.encoded.hexString, expectedSignedTx)
    }
}
