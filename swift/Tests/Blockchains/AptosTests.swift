// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    
    func testBlindSignWithABI() {
        // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x1ee2aa55382bf6b5a9f7a7f2b2066e16979489c6b2868704a2cf2c482f12b5ca/payload?network=mainnet
        let payloadJson = """
                             {
                                "function": "0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
                                "type_arguments": [
                                    "0x1::aptos_coin::AptosCoin"
                                ],
                                "arguments": [
                                    "0x4d61696e204163636f756e74",
                                    "10000000",
                                    false
                                ],
                                "type": "entry_function_payload"
                             }
                             """
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let input = AptosSigningInput.with {
            $0.chainID = 1
            $0.anyEncoded = payloadJson
            $0.expirationTimestampSecs = 1735902711
            $0.gasUnitPrice = 100
            $0.maxGasAmount = 50000
            $0.sequenceNumber = 69
            $0.sender = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.privateKey = privateKeyData
            $0.abi = """
                [
                    "vector<u8>",
                    "u64",
                    "bool"
                ]
            """
        }
        let output: AptosSigningOutput = AnySigner.sign(input: input, coin: .aptos)
        let expectedRawTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f304500000000000000029770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da30a636f6e74726f6c6c6572076465706f736974010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00030d0c4d61696e204163636f756e74088096980000000000010050c30000000000006400000000000000f7c577670000000001"
        let expectedSignature = "13dcf1636abd31996729ded4d3bf56e9c7869a7188df4f185cbcce42f0dc74b6e1b54d31703ee3babbea2ef72b3338b8c2866cec68cbd761ccc7f80910124304"
        let expectedSignedTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f304500000000000000029770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da30a636f6e74726f6c6c6572076465706f736974010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00030d0c4d61696e204163636f756e74088096980000000000010050c30000000000006400000000000000f7c5776700000000010020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c4013dcf1636abd31996729ded4d3bf56e9c7869a7188df4f185cbcce42f0dc74b6e1b54d31703ee3babbea2ef72b3338b8c2866cec68cbd761ccc7f80910124304"
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
    
    func testSignFungibleAssetTransfer() {
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0x475fc97bcba87907166a720676e1b2f5320e613fd13014df37dcf17b09ff0e98/balanceChange?network=mainnet
        let privateKeyData = Data(hexString: "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec")!
        let fungibleAssetTransferMsg = AptosFungibleAssetTransferMessage.with {
            $0.metadataAddress = "0x2ebb2ccac5e027a87fa0e2e5f656a3a4238d6a48d93ec9b610d570fc0aa0df12"
            $0.to = "0x2d92d71078f11d923c2b703b95a288c0e2ae63c0d29154e6278bf8004f9b4e52"
            $0.amount = 100000000
        }
        let input = AptosSigningInput.with {
            $0.chainID = 1
            $0.sender = "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30"
            $0.expirationTimestampSecs = 1736060099
            $0.gasUnitPrice = 100
            $0.maxGasAmount = 20
            $0.sequenceNumber = 74
            $0.fungibleAssetTransfer = fungibleAssetTransferMsg
            $0.privateKey = privateKeyData
        }
        let output: AptosSigningOutput = AnySigner.sign(input: input, coin: .aptos)
        let expectedRawTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f304a00000000000000020000000000000000000000000000000000000000000000000000000000000001167072696d6172795f66756e6769626c655f73746f7265087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010e66756e6769626c655f6173736574084d657461646174610003202ebb2ccac5e027a87fa0e2e5f656a3a4238d6a48d93ec9b610d570fc0aa0df12202d92d71078f11d923c2b703b95a288c0e2ae63c0d29154e6278bf8004f9b4e520800e1f5050000000014000000000000006400000000000000c32c7a670000000001"
        let expectedSignature = "2d4c5cbb710b6ef92813597054dbf8d3014529a7d85f6393f01e2a3e978c461c6aa656475b98b453ed3faebf7aa1fdd912bfc59a0c1b6fc44330793994b2e40c"
        let expectedSignedTx = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f304a00000000000000020000000000000000000000000000000000000000000000000000000000000001167072696d6172795f66756e6769626c655f73746f7265087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010e66756e6769626c655f6173736574084d657461646174610003202ebb2ccac5e027a87fa0e2e5f656a3a4238d6a48d93ec9b610d570fc0aa0df12202d92d71078f11d923c2b703b95a288c0e2ae63c0d29154e6278bf8004f9b4e520800e1f5050000000014000000000000006400000000000000c32c7a6700000000010020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c402d4c5cbb710b6ef92813597054dbf8d3014529a7d85f6393f01e2a3e978c461c6aa656475b98b453ed3faebf7aa1fdd912bfc59a0c1b6fc44330793994b2e40c"
        XCTAssertEqual(output.rawTxn.hexString, expectedRawTx)
        XCTAssertEqual(output.authenticator.signature.hexString, expectedSignature)
        XCTAssertEqual(output.encoded.hexString, expectedSignedTx)
    }
}
