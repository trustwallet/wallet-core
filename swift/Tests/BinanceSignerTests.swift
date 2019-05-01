// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class BinanceSignerTests: XCTestCase {
    func testExample() {
        let privateKey = PrivateKey(data: Data(hexString: "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)

        var signingInput = TW_Binance_Proto_SigningInput()
        signingInput.chainID = "Binance-Chain-Nile"
        signingInput.accountNumber = 0
        signingInput.sequence = 0

        signingInput.privateKey = privateKey.data

        var token = TW_Binance_Proto_SendOrder.Token()
        token.denom = "BNB"
        token.amount = 1

        var input = TW_Binance_Proto_SendOrder.Input()
        input.address = CosmosAddress(hrp: .binance, publicKey: publicKey)!.keyHash
        input.coins = [token]

        var output = TW_Binance_Proto_SendOrder.Output()
        output.address = CosmosAddress(string: "bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5")!.keyHash
        output.coins = [token]

        var sendOrder = TW_Binance_Proto_SendOrder()
        sendOrder.inputs = [input]
        sendOrder.outputs = [output]

        signingInput.sendOrder = sendOrder

        let data = BinanceSigner.sign(input: signingInput)

        XCTAssertEqual(data.encoded.hexString, "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
    }
}
