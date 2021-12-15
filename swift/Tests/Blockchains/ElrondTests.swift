// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class ElrondTests: XCTestCase {

    let aliceBech32 = "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"
    let aliceSeedHex = "1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf"
    let alicePubKeyHex = "fd691bb5e85d102687d81079dffce842d4dc328276d2d4c60d8fd1c3433c3293"
    let bobBech32 = "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: aliceSeedHex)!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .elrond)
        let addressFromString = AnyAddress(string: aliceBech32, coin: .elrond)!

        XCTAssertEqual(pubkey.data.hexString, alicePubKeyHex)
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.transaction = ElrondTransactionMessage.with {
                $0.nonce = 0
                $0.value = "0"
                $0.sender = aliceBech32
                $0.receiver = bobBech32 
                $0.gasPrice = 1000000000
                $0.gasLimit = 50000
                $0.data = "foo"
                $0.chainID = "1"
                $0.version = 1
            }

            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00"
        let expectedEncoded = #"{"nonce":0,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testEGLDTransfer() {
        let networkConfig = ElrondNetworkConfig()
        let factory = ElrondTransactionFactory(networkConfig: networkConfig)

        let transaction: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createEGLDTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            amount: "1000000000000000000"
        ))

        XCTAssertEqual(transaction.sender, aliceBech32)
        XCTAssertEqual(transaction.receiver, bobBech32)
        XCTAssertEqual(transaction.data, "")
        XCTAssertEqual(transaction.value, "1000000000000000000")
        XCTAssertEqual(transaction.gasPrice, 1000000000)
        XCTAssertEqual(transaction.gasLimit, 50000)
        XCTAssertEqual(transaction.chainID, "1")
        XCTAssertEqual(transaction.version, 1)
    }

    func testESDTTransfer() {
        let networkConfig = ElrondNetworkConfig()
        let factory = ElrondTransactionFactory(networkConfig: networkConfig)

        let transaction: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createESDTTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            tokenIdentifier: "MYTOKEN-1234",
            amount: "10000000000000"
        ))

        XCTAssertEqual(transaction.sender, aliceBech32)
        XCTAssertEqual(transaction.receiver, bobBech32)
        XCTAssertEqual(transaction.data, "ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000")
        XCTAssertEqual(transaction.value, "0")
        XCTAssertEqual(transaction.gasPrice, 1000000000)
        XCTAssertEqual(transaction.gasLimit, 425000)
        XCTAssertEqual(transaction.chainID, "1")
        XCTAssertEqual(transaction.version, 1)
    }

    func testESDTNFTTransfer() {
        let networkConfig = ElrondNetworkConfig()
        let factory = ElrondTransactionFactory(networkConfig: networkConfig)

        let transaction: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createESDTNFTTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            collection: "LKMEX-aab910",
            nonce: 4,
            quantity: "184300000000000000"
        ))

        XCTAssertEqual(transaction.sender, aliceBech32)
        XCTAssertEqual(transaction.receiver, aliceBech32)
        XCTAssertEqual(transaction.data, "ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@c70cf50b238372fffaf7b7c5723b06b57859d424a2da621bcc1b2f317543aa36")
        XCTAssertEqual(transaction.value, "0")
        XCTAssertEqual(transaction.gasPrice, 1000000000)
        XCTAssertEqual(transaction.gasLimit, 937500)
        XCTAssertEqual(transaction.chainID, "1")
        XCTAssertEqual(transaction.version, 1)
    }

    func testTransfersWithChangedNetworkConfig() {
        let networkConfig = ElrondNetworkConfig()
        networkConfig.setChainId(value: "T")
        networkConfig.setMinGasPrice(value: 1500000000)
        networkConfig.setMinGasLimit(value: 60000)
        networkConfig.setGasPerDataByte(value: 2000)
        networkConfig.setGasCostESDTTransfer(value: 300000)
        networkConfig.setGasCostESDTNFTTransfer(value: 300000)

        let factory = ElrondTransactionFactory(networkConfig: networkConfig)

        let tx1: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createEGLDTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            amount: "1000000000000000000"
        ))

        let tx2: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createESDTTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            tokenIdentifier: "MYTOKEN-1234",
            amount: "10000000000000"
        ))

        let tx3: ElrondTransactionMessage = Marshalizer.unmarshalProto(factory.createESDTNFTTransfer(
            sender: aliceBech32,
            receiver: bobBech32,
            collection: "LKMEX-aab910",
            nonce: 4,
            quantity: "184300000000000000"
        ))

        XCTAssertEqual(tx1.gasPrice, 1500000000)
        XCTAssertEqual(tx1.gasLimit, 60000)
        XCTAssertEqual(tx1.chainID, "T")

        XCTAssertEqual(tx2.gasPrice, 1500000000)
        XCTAssertEqual(tx2.gasLimit, 560000)
        XCTAssertEqual(tx2.chainID, "T")

        XCTAssertEqual(tx3.gasPrice, 1500000000)
        XCTAssertEqual(tx3.gasLimit, 1110000)
        XCTAssertEqual(tx3.chainID, "T")
    }
}
