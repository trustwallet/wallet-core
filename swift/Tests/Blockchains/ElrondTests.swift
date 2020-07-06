// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
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
                $0.gasPrice = 200000000000000
                $0.gasLimit = 500000000
                $0.data = "foo"
                $0.chainID = "m1.0"
                $0.version = 1
            }

            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "fd77f627294c2cad3c4b0c761cad70e886fa1cfd119803caa2bcbc2d5ed3518df3e7531de9daa8ab47b278ac97a0cca5797868bdaba759845ce8c2c91162c30f"
        let expectedEncoded = #"{"nonce":0,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":200000000000000,"gasLimit":500000000,"data":"foo","chainID":"m1.0","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
}
