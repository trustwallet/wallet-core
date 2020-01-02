// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class CardanoTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")!)!
        let pubkey = key.getPublicKeyEd25519Extended()
        let address = CardanoAddress(publicKey: pubkey)
        let addressFromString = CardanoAddress(string: "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W")!

        XCTAssertEqual(pubkey.data.hexString, "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        var input = CardanoSigningInput.with {
            $0.amount = 1000000
            $0.toAddress = "Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp"
            $0.changeAddress = "Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW"
        }
        let utxo0 = CardanoUnspentTransaction.with {
            $0.outPoint.txid = Data(hexString: "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14")!
            $0.outPoint.index = 6
            $0.amount = 15000000
            $0.address = "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx"
        }
        input.utxo.append(utxo0)
        input.privateKey.append(Data(hexString: "b8c31abcc41d931ae881be11da9e4d9242b1f01cae4e69fa29d5ba1f89f9c1549ec844c6b39c70fa6d3a254fe57c1efee1a75eb9755e0b751e96dd288deabc881ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f")!)

        let plan = CardanoSigner.planTransaction(input: input)
        XCTAssertEqual(plan.error, "")
        XCTAssertEqual(plan.fee, 167994)

        let output = CardanoSigner.sign(input: input, plan: plan)
        XCTAssertEqual(output.encoded.hexString, "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d30f46ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f58403646fec783197bdf3d83858d8303f9d72e09a5df882202716c7737e55148c1c2106c550890c0ab001f7a27cb75fabe827f81132486bec41e17861aba1a069508")
        XCTAssertEqual(output.transactionID, "4374315285f5af17c5fd37d78bcaa94534968e925c06e293e10d2348d9420fac")
        XCTAssertEqual(output.error, "")
    }
}
