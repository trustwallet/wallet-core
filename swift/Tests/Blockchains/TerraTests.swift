// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class TerraTests: XCTestCase {

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = CosmosAddress(hrp: .terra, publicKey: publicKey)!.description

        let txAmount = CosmosAmount.with {
            $0.amount = 100000
            $0.denom = "uluna"
        }

        let sendCoinsMessage = CosmosSendCoinsMessage.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "terra1szvgdsasnffunwakp9w040unxdresk7a34cfka"
            $0.amounts = [txAmount]
            $0.typePrefix = "pay/MsgSend"
        }

        let feeAmount = CosmosAmount.with {
            $0.amount = 30000
            $0.denom = "uluna"
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [feeAmount]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 1516
            $0.chainID = "columbus-2"
            $0.memo = ""
            $0.sequence = 0
            $0.sendCoinsMessage = sendCoinsMessage
            $0.fee = fee
            $0.privateKey = privateKey.data
            $0.typePrefix = "pay/MsgSend"
        }

        let output = CosmosSigner.sign(input: signingInput)
        // swiftlint:disable:next line_length
        XCTAssertEqual(output.encoded.hexString, "a1627478a563666565a266616d6f756e7481a266616d6f756e746533303030306564656e6f6d65756c756e616367617366323030303030646d656d6f60636d736781a264747970656b7061792f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e74663130303030306564656e6f6d65756c756e616c66726f6d5f61646472657373782c7465727261316a663961616a396d79727a736e6d70647237747765636e6166747a6d6b75326d6873326866656a746f5f61646472657373782c746572726131737a7667647361736e6666756e77616b703977303430756e78647265736b3761333463666b616a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c4131337868565a6c4964616e67434d5a376762686f6f3658743363742b312f64453870764258565269576a6b697369676e61747572657858754a59732b6f623147503235574f782b57434248304c56665050347273563277456b4c422f565462675670425a4f69574761662b514a432f4a2f52386245364c706356493356376343656a58344274794c6e78446a413d3d64747970656b7061792f4d736753656e64")

        let expectedJSON: String =
        """
{
    "tx": {
        "fee": {
            "amount": [{
                "amount": "30000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "pay/MsgSend",
            "value": {
                "amount": [{
                    "amount": "100000",
                    "denom": "uluna"
                }],
                "from_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "to_address": "terra1szvgdsasnffunwakp9w040unxdresk7a34cfka"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "uJYs+ob1GP25WOx+WCBH0LVfPP4rsV2wEkLB/VTbgVpBZOiWGaf+QJC/J/R8bE6LpcVI3V7cCejX4BtyLnxDjA=="
        }],
        "type": "pay/MsgSend"
    }
}
"""

        XCTAssertEqual(expectedJSON.flatten(), output.json)
    }
}
