// Copyright © 2017-2019 Binance.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class BinanceSignerTests: XCTestCase {
    func testExample() {
        let privateKey = PrivateKey(data: Data(hexString: "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832")!)!
        let publicKey = privateKey.getPublicKey(compressed: true)

        var signingInput = TW_Binance_Proto_SigningInput()
        signingInput.chainID = "Binance-Chain-Nile"
        signingInput.accountNumber = 0
        signingInput.sequence = 0
        signingInput.testNet = true

        signingInput.privateKey = privateKey.data

        var token = TW_Binance_Proto_SendOrder.Token()
        token.denom = "BNB"
        token.amount = 1

        var input = TW_Binance_Proto_SendOrder.Input()
        input.address = TendermintAddress(hrp: .binanceTest, publicKey: publicKey)!.keyHash
        input.coins = [token]

        var output = TW_Binance_Proto_SendOrder.Output()
        output.address = TendermintAddress(string: "tbnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx3f309d9")!.keyHash
        output.coins = [token]

        var sendOrder = TW_Binance_Proto_SendOrder()
        sendOrder.inputs = [input]
        sendOrder.outputs = [output]

        signingInput.sendOrder = sendOrder

        let data = BinanceSigner(input: signingInput).build()
        XCTAssertEqual(data?.hexString, "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e5021240376f64070fdb621a4cf24e24da350476260efaf4bae799cacca19b31bc0d1ce4054f09fcffe9cfabef6a8d6a24bc1814bd444720a35f035f3c26409ffad2a9e2")
    }
}
