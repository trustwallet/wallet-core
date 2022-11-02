// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class HederaTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "0.0.1377988", coin: .hedera)

        XCTAssertEqual(anyAddress?.description, "0.0.1377988")
        XCTAssertEqual(anyAddress?.coin, .hedera)

        let invalid = "0.0.a"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .hedera))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .hedera))
    }

    func testSignSimpleTransfer() {
        // Successfully broadcasted: https://hashscan.io/testnet/transaction/0.0.48694347-1667222879-749068449?t=1667222891.440398729&p=1
        let privateKeyData = Data(hexString: "e87a5584c0173263e138db689fdb2a7389025aaae7cb1a18a1017d76012130e8")!
        
        let transfer = HederaTransferMessage.with {
            $0.amount = 100000000
            $0.from = "0.0.48694347"
            $0.to = "0.0.48462050"
        }
        
        let transactionID = HederaTransactionID.with {
            $0.accountID = "0.0.48694347"
            $0.transactionValidStart = HederaTimestamp.with {
                $0.seconds = 1667222879
                $0.nanos = 749068449
            }
        }
        
        let body = HederaTransactionBody.with {
            $0.memo = ""
            $0.nodeAccountID = "0.0.9"
            $0.transactionFee = 100000000
            $0.transactionValidDuration = 120
            $0.transactionID = transactionID
            $0.transfer = transfer
        }
        
        let input = HederaSigningInput.with {
            $0.privateKey = privateKeyData
            $0.body = body
        }
        
        let output: HederaSigningOutput = AnySigner.sign(input: input, coin: .hedera)
        let expectedEncoded = "0a440a150a0c08df9aff9a0610a1c197e502120518cb889c17120218091880c2d72f22020878721e0a1c0a0c0a0518e2f18d17108084af5f0a0c0a0518cb889c1710ff83af5f12660a640a205d3a70d08b2beafb72c7a68986b3ff819a306078b8c359d739e4966e82e6d40e1a40612589c3b15f1e3ed6084b5a3a5b1b81751578cac8d6c922f31731b3982a5bac80a22558b2197276f5bae49b62503a4d39448ceddbc5ef3ba9bee4c0f302f70c"
        XCTAssertEqual(output.encoded.hexString, expectedEncoded)
    }
}
