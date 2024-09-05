// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class LiquidStakingTests: XCTestCase {
    
    func testStraderStakePolygon() throws {
        let input = LiquidStakingInput.with {
            $0.blockchain = .polygon
            $0.protocol = .strader
            $0.smartContractAddress = "0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3"
            $0.stake = LiquidStakingStake.with {
                $0.amount = "1000000000000000000"
                $0.asset = LiquidStakingAsset.with {
                    $0.stakingToken = .pol
                }
            }
        }
        let inputSerialized = try input.serializedData()
        XCTAssertEqual(inputSerialized.hexString, "0a170a00121331303030303030303030303030303030303030222a3078666432323563396536363031633964333864386639386438373331626635396566636638633065333001")
        let outputData = LiquidStaking.buildRequest(input: inputSerialized)
        XCTAssertEqual(outputData.count, 68)
        let outputProto = try LiquidStakingOutput(serializedData: outputData)
        var txInput = outputProto.ethereum
        
        txInput.chainID = Data(hexString: "89")!
        txInput.nonce = Data(hexString: "01")!
        txInput.maxFeePerGas = Data(hexString: "8fbcc8fcd8")!
        txInput.maxInclusionFeePerGas = Data(hexString: "085e42c7c0")!
        txInput.gasLimit = Data(hexString: "01c520")!
        txInput.privateKey = Data(hexString: "4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab")!
        
        let output: EthereumSigningOutput = AnySigner.sign(input: txInput, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "02f87a81890185085e42c7c0858fbcc8fcd88301c52094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e3880de0b6b3a764000084c78cf1a0c001a04bcf92394d53d4908130cc6d4f7b2491967f9d6c59292b84c1f56adc49f6c458a073e09f45d64078c41a7946ffdb1dee8e604eb76f318088490f8f661bb7ddfc54")
        // Successfully broadcasted: https://polygonscan.com/tx/0x0f6c4f7a893c3f08be30d2ea24479d7ed4bdba40875d07cfd607cf97980b7cf0
    }
}
