// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class TezosTests: XCTestCase {
    public let tezos = Tezos()
    
    public func testCoinType() {
        XCTAssertEqual(tezos.coinType, .tezos)
    }
    
    public func testAddressFromString_validAddress() {
        let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
        let address = tezos.address(string: validAddressString)
        
        XCTAssertNotNil(address)
        XCTAssertEqual(address?.description, validAddressString)
    }
    
    public func testAddressFromData_validAddress() {
        let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
        guard let validAddressData = validAddressString.data(using: .utf8) else {
            XCTFail("Couldn't decode address to data")
            return
        }
        
        let address = tezos.address(data: validAddressData)
        
        XCTAssertNotNil(address)
        XCTAssertEqual(address?.data, Base58.decode(string: validAddressString))
    }
    
    public func testAddressFromPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        
        let address = tezos.address(for: publicKey)
        
        XCTAssertEqual(address.description, "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM")
    }

    public func testSigning() {
        let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp"

        var operationList = TW_Tezos_Proto_OperationList()
        operationList.branch = branch;

        var revealOperationData = TW_Tezos_Proto_RevealOperationData();
        revealOperationData.publicKey = "edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"

        var transactionOperationData = TW_Tezos_Proto_TransactionOperationData()
        transactionOperationData.amount = 1
        transactionOperationData.destination = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"

        var revealOperation = TW_Tezos_Proto_Operation()
        revealOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        revealOperation.fee = 1272
        revealOperation.counter = 30738
        revealOperation.gasLimit = 10100
        revealOperation.storageLimit = 257
        revealOperation.kind = .reveal
        revealOperation.revealOperationData = revealOperationData

        var transactionOperation = TW_Tezos_Proto_Operation()
        transactionOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        transactionOperation.fee = 1272
        transactionOperation.counter = 30739
        transactionOperation.gasLimit = 10100
        transactionOperation.storageLimit = 257
        transactionOperation.kind = .transaction
        transactionOperation.transactionOperationData = transactionOperationData

        operationList.operations = [ revealOperation, transactionOperation ]

        let privateKeyBytes: [UInt8] = [
            46, 137, 5, 129, 155, 135, 35, 254, 44, 29, 22, 24, 96, 229, 238, 24, 48, 49, 141, 191, 73, 168, 59, 212,
            81, 207, 184, 68, 12, 40, 189, 111
        ]

        var signingInput = TW_Tezos_Proto_SigningInput()
        signingInput.operationList = operationList
        signingInput.privateKey = Data(privateKeyBytes)

        let signingOuput = TezosSigner.sign(input: signingInput);
        let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b9572100d924cb3e56c4b9f55e50735e461899a2f616a26bfb0aa05d0b356b66f517b023df330ad3621f0bf39d518131a1becd6a7b2e226ed291483af3682535d1f4530f"

        XCTAssertEqual(signingOuput.signedBytes, expected.data(using: .utf8)!)
    }
}
