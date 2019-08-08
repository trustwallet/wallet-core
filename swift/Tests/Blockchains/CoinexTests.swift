// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CoinexSignerTests: XCTestCase {

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = CosmosAddress(hrp: .coinex, publicKey: publicKey)!.description

        let txAmount = CoinexAmount.with {
            $0.amount = 1000000
            $0.denom = "cet"
        }

        let sendCoinsMessage = CoinexSendCoinsMessage.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "coinex1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra6me"
            $0.amounts = [txAmount]
            $0.unlockTime = 0
        }

        let feeAmount = CoinexAmount.with {
            $0.amount = 2000000
            $0.denom = "cet"
        }

        let fee = CoinexFee.with {
            $0.gas = 200000
            $0.amounts = [feeAmount]
        }

        let signingInput = CoinexSigningInput.with {
            $0.accountNumber = 474
            $0.chainID = "coinexdex-test"
            $0.memo = "ifwallet"
            $0.sequence = 0
            $0.sendCoinsMessage = sendCoinsMessage
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CoinexSigner.sign(input: signingInput)

        let expectedJSON: String =
"""
{
	"tx": {
		"fee": {
			"amount": [{
				"amount": "2000000",
				"denom": "cet"
			}],
			"gas": "100000"
		},
		"memo": "ifwallet",
		"msg": [{
			"type": "bankx/MsgSend",
			"value": {
				"amount": [{
					"amount": "1000000",
					"denom": "cet"
				}],
				"from_address": "coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr",
				"to_address": "coinex1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra6me",
				"unlock_time": "0"
			}
		}],
		"signatures": [{
			"pub_key": {
				"type": "tendermint/PubKeySecp256k1",
				"value": "AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC"
			},
			"signature": "PZZs2INLuhAsC7Qk7P0/IHHL27V2ExOGLZPtpeABs9caJXuiBwlR+ez2feQlgGvju67t+5tPLhIUoaN6EvWR8Q=="
		}],
		"type": "bankx/MsgSend"
	}
}
"""

        XCTAssertEqual(expectedJSON.flatten(), output.json)
    }

    func testStaking() {
        let txAmount = CoinexAmount.with {
            $0.amount = 1000000
            $0.denom = "cet"
        }

        let stakeMessage = CoinexStakeMessage.with {
            $0.delegatorAddress = "coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr"
            $0.validatorAddress = "coinexvaloper1pxf3sgcxpu99e4havmeus7gmmt8u3j9jpv7j4d"
            $0.amount = txAmount
        }

        let feeAmount = CoinexAmount.with {
            $0.amount = 2000000 
            $0.denom = "cet"
        }

        let fee = CoinexFee.with {
            $0.gas = 100000
            $0.amounts = [feeAmount]
        }

        let privateKey = PrivateKey(data: Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!)!

        let signingInput = CoinexSigningInput.with {
            $0.accountNumber = 474
            $0.chainID = "coinexdex-test"
            $0.memo = "ifwallet"
            $0.sequence = 1
            $0.stakeMessage = stakeMessage
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CoinexSigner.sign(input: signingInput)

        let expectedJSON = 
"""
{
	"tx": {
		"fee": {
			"amount": [{
				"amount": "2000000",
				"denom": "cet"
			}],
			"gas": "100000"
		},
		"memo": "ifwallet",
		"msg": [{
			"type": "cosmos-sdk/MsgDelegate",
			"value": {
				"amount": {
					"amount": "1000000",
					"denom": "cet"
				},
				"delegator_address": "coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr",
				"validator_address": "coinexvaloper1pxf3sgcxpu99e4havmeus7gmmt8u3j9jpv7j4d"
			}
		}],
		"signatures": [{
			"pub_key": {
				"type": "tendermint/PubKeySecp256k1",
				"value": "AkvCoxJlFT8H5w4LqwhyTmuF4hf4zWKM62KXQke7STOC"
			},
			"signature": "WTHDn9sbBCL0HMfASK8FrHPW3Yrb9ks1YpmcVb4OlRhFeTnQJPVrLHAEmeIV1D/MERPbRAy7abQFcO4M509tyw=="
		}],
		"type": "cosmos-sdk/MsgSend"
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
