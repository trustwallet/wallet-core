// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class THORSwapTests: XCTestCase {

    func testSignerEthBnb() throws {
        // prepare swap input
        let input = THORChainSwapSwapInput.with {
            $0.fromChain = .eth
            $0.fromAddress = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7"
            $0.toAsset = THORChainSwapAsset.with {
                $0.chain = .bnb
                $0.symbol = "BNB"
                $0.tokenID = ""
            }
            $0.toAddress = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx"
            $0.vaultAddress = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC"
            $0.routerAddress = "0x42A5Ed456650a09Dc10EBc6361A7480fDd61f27B"
            $0.fromAmount = "50000000000000000"
            $0.toAmountLimit = "600003"
        }

        // serialize input
        let inputSerialized = try input.serializedData()
        XCTAssertEqual(inputSerialized.hexString, "0802122a3078623966353737316332373636346266323238326439386530396437663530636563376362303161371a0708031203424e42222a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372782a2a307831303931633444653661336346303943644130304162444165443432633763334236394338334543322a3078343241354564343536363530613039446331304542633633363141373438306644643631663237423a1135303030303030303030303030303030304206363030303033")

        // invoke swap
        let outputData = THORChainSwap.buildSwap(input: inputSerialized)
        XCTAssertEqual(outputData.count, 311)

        // parse result in proto
        let outputProto = try THORChainSwapSwapOutput(serializedData: outputData)
        XCTAssertEqual(outputProto.fromChain, TW_THORChainSwap_Proto_Chain.eth)
        XCTAssertEqual(outputProto.toChain, TW_THORChainSwap_Proto_Chain.bnb)
        XCTAssertEqual(outputProto.error.code, TW_THORChainSwap_Proto_ErrorCode.ok)
        var txInput = outputProto.ethereum

        // set few fields before signing
        txInput.chainID = Data(hexString: "01")!
        txInput.nonce = Data(hexString: "03")!
        txInput.gasPrice = Data(hexString: "06FC23AC00")!
        txInput.gasLimit = Data(hexString: "013880")!
        txInput.privateKey = Data(hexString: "4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904")!

        // sign and encode resulting input
        let output: EthereumSigningOutput = AnySigner.sign(input: txInput, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "f90151038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003e535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033000025a06ae104be3201baca38315352f81fac70ca4dd47339981914e64e91149813e780a066a3f0b2c44ddf5a96a38481274f623f552a593d723237d6742185f4885c0064")
    }

    func testSignerBnbBtc() throws {
        // prepare swap input
        let input = THORChainSwapSwapInput.with {
            $0.fromChain = .bnb
            $0.fromAddress = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx"
            $0.toAsset = THORChainSwapAsset.with {
                $0.chain = .btc
                $0.symbol = "BTC"
                $0.tokenID = ""
            }
            $0.toAddress = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8"
            $0.vaultAddress = "bnb1n9esxuw8ca7ts8l6w66kdh800s09msvul6vlse"
            $0.routerAddress = ""
            $0.fromAmount = "10000000"
            $0.toAmountLimit = "10000000"
        }
        
        // serialize input
        let inputSerialized = try input.serializedData()
        XCTAssertEqual(inputSerialized.hexString, "0803122a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372781a0708011203425443222a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070382a2a626e62316e396573787577386361377473386c367736366b64683830307330396d7376756c36766c73653a08313030303030303042083130303030303030")

        // invoke swap
        let outputData = THORChainSwap.buildSwap(input: inputSerialized)
        XCTAssertEqual(outputData.count, 149)

        // parse result in proto
        let outputProto = try THORChainSwapSwapOutput(serializedData: outputData)
        XCTAssertEqual(outputProto.fromChain, TW_THORChainSwap_Proto_Chain.bnb)
        XCTAssertEqual(outputProto.toChain, TW_THORChainSwap_Proto_Chain.btc)
        XCTAssertEqual(outputProto.error.code, TW_THORChainSwap_Proto_ErrorCode.ok)
        var txInput = outputProto.binance

        // set few fields before signing
        
        txInput.chainID = "Binance-Chain-Nile"
        txInput.privateKey = Data(hexString: "bcf8b072560dda05122c99390def2c385ec400e1a93df0657a85cf6b57a715da")!

        // sign and encode resulting input
        let output: BinanceSigningOutput = AnySigner.sign(input: txInput, coin: .binance)

        XCTAssertEqual(output.encoded.hexString, "8002f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e1912404836ee8659caa86771281d3f104424d95977bdedf644ec8585f1674796fde525669a6d446f72da89ee90fb0e064473b0a2159a79630e081592c52948d03d67071a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030")
    }
}
