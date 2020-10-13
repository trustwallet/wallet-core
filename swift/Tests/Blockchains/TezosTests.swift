// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class TezosTests: XCTestCase {
    public let tezos = CoinType.tezos

    public func testAddressFromString_validAddress() {
        let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
        let address = AnyAddress(string: validAddressString, coin: .tezos)

        XCTAssertNotNil(address)
        XCTAssertEqual(address?.description, validAddressString)
    }

    public func testAddressFromPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "b177a72743f54ed4bdf51f1b55527c31bcd68c6d2cb2436d76cadd0227c99ff0")!)!
        let publicKey = privateKey.getPublicKeyEd25519()

        let address = tezos.deriveAddressFromPublicKey(publicKey: publicKey)

        XCTAssertEqual(address.description, "tz1cG2jx3W4bZFeVGBjsTxUAG8tdpTXtE8PT")
    }

    public func testSigning() {
        let privateKeyData = Data(hexString: "c6377a4cc490dc913fc3f0d9cf67d293a32df4547c46cb7e9e33c3b7b97c64d8")!
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeyEd25519()

        let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp"

        var operationList = TezosOperationList()
        operationList.branch = branch

        var revealOperationData = TezosRevealOperationData()
        revealOperationData.publicKey = publicKey.data

        var transactionOperationData = TezosTransactionOperationData()
        transactionOperationData.amount = 1
        transactionOperationData.destination = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"

        var revealOperation = TezosOperation()
        revealOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        revealOperation.fee = 1272
        revealOperation.counter = 30738
        revealOperation.gasLimit = 10100
        revealOperation.storageLimit = 257
        revealOperation.kind = .reveal
        revealOperation.revealOperationData = revealOperationData

        var transactionOperation = TezosOperation()
        transactionOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        transactionOperation.fee = 1272
        transactionOperation.counter = 30739
        transactionOperation.gasLimit = 10100
        transactionOperation.storageLimit = 257
        transactionOperation.kind = .transaction
        transactionOperation.transactionOperationData = transactionOperationData

        operationList.operations = [ revealOperation, transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = Data(hexString: "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f")!
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05"

        XCTAssertEqual(output.encoded.hexString, expected)
    }
}
