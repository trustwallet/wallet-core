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
    
    public func testSigningFA12() {
        let privateKeyData = Data(hexString: "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6")!
        
        let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp"
        var operationList = TezosOperationList()
        operationList.branch = branch
        
        let transactionOperationData = TezosTransactionOperationData.with {
            $0.amount = 0
            $0.destination = "KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5"
            $0.parameters.fa12Parameters.entrypoint = "transfer";
            $0.parameters.fa12Parameters.from = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP";
            $0.parameters.fa12Parameters.to   = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP";
            $0.parameters.fa12Parameters.value = "123";
        }
        
        let transactionOperation = TezosOperation.with {
            $0.source = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.fee = 100000
            $0.counter = 2993172
            $0.gasLimit = 100000
            $0.storageLimit = 0
            $0.kind = .transaction
            $0.transactionOperationData = transactionOperationData
        }
        
        operationList.operations = [ transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = privateKeyData
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb012914d768155fba2df319a81136e8e3e573b9cadb1676834490c90212615d271da029b6b0531e290e9063bcdb40bea43627af048b18e036f02be2b6b22fc8b307"

        XCTAssertEqual(output.encoded.hexString, expected)
    }
    
    public func testSigningFA2() {
        let privateKeyData = Data(hexString: "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6")!
        
        let branch = "BKvEAX9HXfJZWYfTQbR1C7B3ADoKY6a1aKVRF7qQqvc9hS8Rr3m"
        var operationList = TezosOperationList()
        operationList.branch = branch
        
        let transferInfos = TezosTxs.with{
            $0.to = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.tokenID = "0"
            $0.amount = "10"
        }
        
        let transactionOperationData = TezosTransactionOperationData.with {
            $0.amount = 0
            $0.destination = "KT1DYk1XDzHredJq1EyNkDindiWDqZyekXGj"
            $0.parameters.fa2Parameters.entrypoint = "transfer";
            $0.parameters.fa2Parameters.txsObject = [TezosTxObject.with{
                    $0.from = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
                    $0.txs = [transferInfos]
                }]
        }
        
        
        
        let transactionOperation = TezosOperation.with {
            $0.source = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.fee = 100000
            $0.counter = 2993173
            $0.gasLimit = 100000
            $0.storageLimit = 0
            $0.kind = .transaction
            $0.transactionOperationData = transactionOperationData
        }
        
        operationList.operations = [ transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = privateKeyData
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "1b1f9345dc9f77bd24b09034d1d2f9a28f02ac837f49db54b8d68341f53dc4b76c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a552d24710d6c59383286700c6c2917b25a6c1fa8b587e593c289dd47704278796792f1e522c1623845ec991e292b0935445e6994850bd03f035a006c5ed93806"

        XCTAssertEqual(output.encoded.hexString, expected)
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
