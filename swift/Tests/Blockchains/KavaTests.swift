// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore


class KavaTests: XCTestCase {

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = CosmosAddress(hrp: .kava, publicKey: publicKey)!.description

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "kava1zrst72upua78pylhku9csxd5zmhsyrk7xhrdlf"
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1000000
                $0.denom = "ukava"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1000
                $0.denom = "ukava"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 12345
            $0.chainID = "kava-2"
            $0.sequence = 123
            $0.memo = ""
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON: String =
        """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "1000",
          "denom": "ukava"
        }
      ],
      "gas": "200000"
    },
    "memo": "",
    "msg": [
      {
        "type": "cosmos-sdk/MsgSend",
        "value": {
          "amount": [
            {
              "amount": "1000000",
              "denom": "ukava"
            }
          ],
          "from_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
          "to_address": "kava1zrst72upua78pylhku9csxd5zmhsyrk7xhrdlf"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "jy4Xn5PiWJLkNVuQddn00UENDyrf0VIvXVjT0dEm5H40Re3uFMWjOXxS6IGNdR3Flv1EC5kKv1W2BuvaKK6m0g=="
      }
    ]
  }
}
"""

        XCTAssertEqual(expectedJSON.flatten(), output.json)
    }
}
