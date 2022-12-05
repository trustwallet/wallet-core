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
    
    func testBlindSign() {
        // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x7efd69e7f9462774b932ce500ab51c0d0dcc004cf272e09f8ffd5804c2a84e33?network=mainnet
        let payloadJson = """
                             {
                                "function": "0x16fe2df00ea7dde4a63409201f7f4e536bde7bb7335526a35d05111e68aa322c::AnimeSwapPoolV1::swap_exact_coins_for_coins_3_pair_entry",
                                "type_arguments": [
                                             "0x1::aptos_coin::AptosCoin",
                                             "0x881ac202b1f1e6ad4efcff7a1d0579411533f2502417a19211cfc49751ddb5f4::coin::MOJO",
                                             "0xf22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa::asset::USDT",
                                             "0xf22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa::asset::USDC"
                                         ],
                                "arguments": ["1000000", "49329"],
                                 "type": "entry_function_payload"
                             }
                             """
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xb4d62afd3862116e060dd6ad9848ccb50c2bc177799819f1d29c059ae2042467?network=devnet
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let input = AptosSigningInput.with {
            $0.chainID = 1
            $0.anyEncoded = payloadJson
            $0.expirationTimestampSecs = 3664390082
            $0.gasUnitPrice = 100
            $0.maxGasAmount = 100011
            $0.sequenceNumber = 42
            $0.sender = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.privateKey = privateKeyData
        }
        let output: AptosSigningOutput = AnySigner.sign(input: input, coin: .aptos)
        let expectedRawTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f302a000000000000000216fe2df00ea7dde4a63409201f7f4e536bde7bb7335526a35d05111e68aa322c0f416e696d6553776170506f6f6c563127737761705f65786163745f636f696e735f666f725f636f696e735f335f706169725f656e747279040700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e0007881ac202b1f1e6ad4efcff7a1d0579411533f2502417a19211cfc49751ddb5f404636f696e044d4f4a4f0007f22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa05617373657404555344540007f22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa056173736574045553444300020840420f000000000008b1c0000000000000ab860100000000006400000000000000c2276ada0000000001"
        let expectedSignature = "42cd67406e85afd1e948e7ad7f5f484fb4c60d82b267c6b6b28a92301e228b983206d2b87cd5487cf9acfb0effbd183ab90123570eb2e047cb152d337152210b"
        let expectedSignedTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f302a000000000000000216fe2df00ea7dde4a63409201f7f4e536bde7bb7335526a35d05111e68aa322c0f416e696d6553776170506f6f6c563127737761705f65786163745f636f696e735f666f725f636f696e735f335f706169725f656e747279040700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e0007881ac202b1f1e6ad4efcff7a1d0579411533f2502417a19211cfc49751ddb5f404636f696e044d4f4a4f0007f22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa05617373657404555344540007f22bede237a07e121b56d91a491eb7bcdfd1f5907926a9e58338f964a01b17fa056173736574045553444300020840420f000000000008b1c0000000000000ab860100000000006400000000000000c2276ada00000000010020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c4042cd67406e85afd1e948e7ad7f5f484fb4c60d82b267c6b6b28a92301e228b983206d2b87cd5487cf9acfb0effbd183ab90123570eb2e047cb152d337152210b"
        XCTAssertEqual(output.rawTxn.hexString, expectedRawTx)
        XCTAssertEqual(output.authenticator.signature.hexString, expectedSignature)
        XCTAssertEqual(output.encoded.hexString, expectedSignedTx)
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
