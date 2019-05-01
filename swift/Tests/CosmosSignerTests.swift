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

        let txAmount = CosmosAmount.with {
            $0.amount = 1
            $0.denom = "muon"
        }

        let sendCoinsMessage = CosmosSendCoinsMessage.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
            $0.amounts = [txAmount]
        }

        let feeAmount = CosmosAmount.with {
            $0.amount = 200
            $0.denom = "muon"
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [feeAmount]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 8
            $0.sendCoinsMessage = sendCoinsMessage
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)
        XCTAssertEqual("a264747970656a617574682f53746454786576616c7565a463666565a266616d6f756e7481a266616d6f756e74633230306564656e6f6d646d756f6e6367617366323030303030646d656d6f60636d736781a2647479706572636f736d6f732d73646b2f4d736753656e646576616c7565a366616d6f756e7481a266616d6f756e7461316564656e6f6d646d756f6e6c66726f6d5f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430326a746f5f61646472657373782d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537336a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e617475726578582f4437346d644947794942332f73517649626f4c54665339503945562f6659477267485a45322f764e6a395836654d36653537473361746c6a4e422b5041426e52773370546b353175586d6843466f70384f2f5a4a673d3d", output.encoded.hexString)

        let expectedJSON: String = """
{
  "type": "auth/StdTx",
  "value": {
    "fee": {
      "amount": [
        {
          "amount": "200",
          "denom": "muon"
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
              "amount": "1",
              "denom": "muon"
            }
          ],
          "from_address": "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
          "to_address": "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
        },
        "signature": "/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="
      }
    ]
  }
}
"""
        XCTAssertEqual(expectedJSON.flatten(), output.json)
    }

    func testStaking() {
        let txAmount = CosmosAmount.with {
            $0.amount = 10
            $0.denom = "muon"
        }

        let stakeMessage = CosmosStakeMessage.with {
            $0.delegatorAddress = "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"
            $0.validatorAddress = "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"
            $0.amount = txAmount
        }

        let feeAmount = CosmosAmount.with {
            $0.amount = 1018
            $0.denom = "muon"
        }

        let fee = CosmosFee.with {
            $0.gas = 101721
            $0.amounts = [feeAmount]
        }

        let privateKey = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)!

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 7
            $0.stakeMessage = stakeMessage
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)
        XCTAssertEqual("a264747970656a617574682f53746454786576616c7565a463666565a266616d6f756e7481a266616d6f756e7464313031386564656e6f6d646d756f6e6367617366313031373231646d656d6f60636d736781a2647479706576636f736d6f732d73646b2f4d736744656c65676174656576616c7565a366616d6f756e74a266616d6f756e746231306564656e6f6d646d756f6e7164656c656761746f725f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430327176616c696461746f725f616464726573737834636f736d6f7376616c6f706572317a6b757072383368727a6b6e33757035656c6b747a63713374756674386e78736d77647167706a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e61747572657858774976666243734c52436a7a6558586f58544b66484c4758526241416d5570304f313334485666566336706664564e4a76767a49534d485255486759636a735369466c4c79523332686569612f794c674d44744959513d3d", output.encoded.hexString)

        let expectedJSON = """
{
  "type": "auth/StdTx",
  "value": {
    "fee": {
      "amount": [
        {
          "amount": "1018",
          "denom": "muon"
        }
      ],
      "gas": "101721"
    },
    "memo": "",
    "msg": [
      {
        "type": "cosmos-sdk/MsgDelegate",
        "value": {
          "amount": {
            "amount": "10",
            "denom": "muon"
          },
          "delegator_address": "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
          "validator_address": "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
        },
        "signature": "wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="
      }
    ]
  }
}
"""
        XCTAssertEqual(expectedJSON.flatten(), output.json)
    }
}

extension String {
    func flatten() -> String {
        return components(separatedBy: .whitespacesAndNewlines).joined()
    }
}
