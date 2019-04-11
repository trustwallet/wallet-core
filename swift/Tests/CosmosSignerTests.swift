// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CosmosSignerTests: XCTestCase {

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = CosmosAddress(hrp: .cosmos, publicKey: publicKey)!.description

        var sendCoinsMessage = TW_Cosmos_Proto_SendCoinsMessage()
        sendCoinsMessage.fromAddress = fromAddress
        sendCoinsMessage.toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
        var coin = TW_Cosmos_Proto_Coin()
        coin.amount = 1
        coin.denom = "muon"
        sendCoinsMessage.amount = [coin]

        var feeCoin = TW_Cosmos_Proto_Coin()
        feeCoin.amount = 1
        feeCoin.denom = "muon"

        var fee = TW_Cosmos_Proto_Fee()
        fee.gas = 200000
        fee.amount = [feeCoin]

        var signingInput = TW_Cosmos_Proto_SigningInput()
        signingInput.accountNumber = 1037
        signingInput.chainID = "gaia-13002"
        signingInput.memo = ""
        signingInput.sequence = 2
        signingInput.message = sendCoinsMessage
        signingInput.fee = fee
        signingInput.privateKey = privateKey.data

        let data = CosmosSigner.sign(input: signingInput)
        XCTAssertEqual("a264747970656a617574682f53746454786576616c7565a463666565a266616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6367617366323030303030646d656d6f60636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6c66726f6d5f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430326a746f5f61646472657373782d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537336a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e6174757265785855436e497337676c376f705078596c7268514771733235795350496b506769764f312f524f504e55663435312f6335516472465835756e62585377666a7657394838306332314a597a4e724e6e7148754755435355773d3d", data.encoded.hexString)
    }
}
