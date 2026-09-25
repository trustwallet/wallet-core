// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class DecredTests: XCTestCase {

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let dprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .decred, version: .dprv)
        let dpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .decred, version: .dpub)

        XCTAssertEqual(dprv, "dprv3oggQ2FQ1chcr18hbW7Aur5x8SxQdES3FGa4WqeTZnFY88SNMzLdB7LkZLroF4bGAqWS8sDm3w4DKyYV7sDKfC6JMSVHnVJdpDLgHioq1vq")
        XCTAssertEqual(dpub, "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP")
    }

    func testDeriveFromDpub() {
        let dpub = "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP"
        let pubkey0 = HDWallet.getPublicKeyFromExtended(
            extended: dpub,
            coin: .decred,
            derivationPath: DerivationPath(purpose: .bip44, coin: CoinType.decred.slip44Id, account: 0, change: 0, address: 0).description
        )!

        XCTAssertEqual(AnyAddress(publicKey: pubkey0, coin: .decred).description, "DsksmLD2wDoA8g8QfFvm99ASg8KsZL8eJFd")
    }

    func testSign() {
        // https://mainnet.decred.org/tx/bcc5228e9d956918984d1853c31d7edcd862f8a7fca20ded114d93f8a74ad32a
        let key = PrivateKey(data: Data(hexString: "ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764")!)!

        let txHash = Data.reverse(hexString: "5015d14dcfd78998cfa13e0325798a74d95bbe75f167a49467303f70dde9bffd")
        let utxoAddress = CoinType.decred.deriveAddress(privateKey: key)
        let script = BitcoinScript.lockScriptForAddress(address: utxoAddress, coin: .decred)

        let amount = Int64(10_000_000)

        let utxo = BitcoinUnspentTransaction.with {
            $0.amount = 39_900_000
            $0.script = script.data
            $0.outPoint.hash = txHash
            $0.outPoint.index = 0
        }

        let input = BitcoinSigningInput.with {
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .decred)
            $0.amount = amount
            $0.byteFee = 1
            $0.toAddress = "Dsesp1V6DZDEtcq2behmBVKdYqKMdkh96hL"
            $0.changeAddress = "DsUoWCAxprdGNtKQqambFbTcSBgH1SHn9Gp"
            $0.utxo = [utxo]
            $0.coinType = CoinType.decred.rawValue
            $0.privateKey = [key.data]
            $0.plan = BitcoinTransactionPlan.with {
                $0.amount = amount
                $0.availableAmount = utxo.amount
                $0.fee = 100_000
                $0.change = 29_800_000
                $0.utxos = [utxo]
            }
        }

        let output: DecredSigningOutput = AnySigner.sign(input: input, coin: .decred)

        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)
        XCTAssertEqual(output.encoded.hexString, "0100000001fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd1155000000000000000000002809698000000000000001976a914989b1aecabf1c24e213cc0f2d8a22ffee25dd4e188ac40b6c6010000000000001976a9142a194fc92e27fef9cc2b057bc9060c580cbb484888ac000000000000000001000000000000000000000000ffffffff6a47304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d0121026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6")
    }

    func testSignV2() {
        // https://dcrdata.decred.org/tx/0934163f403cf9d256447890fed972e1f8b66309ecd41dec8a4dcfb657906a68
        let privateKeyData = Data(hexString: "99ed469e6b7d9f188962940d9d0f9fd8582c6c37e52394348f177ff0526b8a03")!
        let privateKey = PrivateKey(data: privateKeyData)!
        let senderAddress = CoinType.decred.deriveAddress(privateKey: privateKey)
        let toAddress = "Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH"

        let txHash = Data.reverse(hexString: "c5cc3b1fc20c9e43a7d1127ba7e4802d04c16515a7eaaad58a1bc388acacfeae")
        let dustAmount = Int64(546)

        let utxo = BitcoinV2Input.with {
            $0.outPoint = UtxoOutPoint.with {
                $0.hash = txHash
                $0.vout = 0
            }
            $0.value = 100_000_000
            $0.sighashType = BitcoinSigHashType.all.rawValue
            $0.receiverAddress = senderAddress
        }

        // 0.1 DCR to another address.
        let out0 = BitcoinV2Output.with {
            $0.value = 10_000_000
            $0.toAddress = toAddress
        }
        
        // 0.05 DCR to self.
        let out1 = BitcoinV2Output.with {
            $0.value = 5_000_000
            $0.toAddress = senderAddress
        }
        
        // Send remaining amount to self by my address.
        let changeOut = BitcoinV2Output.with {
            $0.toAddress = senderAddress
        }
        
        let signingInput = BitcoinV2SigningInput.with {
            $0.builder = BitcoinV2TransactionBuilder.with {
                $0.version = .v1
                $0.inputs = [utxo]
                $0.outputs = [out0, out1]
                $0.changeOutput = changeOut
                $0.inputSelector = .selectDescending
                $0.fixedDustThreshold = dustAmount
                $0.feePerVb = 10
            }
            $0.privateKeys = [privateKeyData]
            $0.chainInfo = BitcoinV2ChainInfo.with {
                $0.p2PkhPrefix = 63
                $0.p2ShPrefix = 26
            }
        }
        
        let legacySigningInput = BitcoinSigningInput.with {
            $0.signingV2 = signingInput
            $0.coinType = CoinType.decred.rawValue
        }
        
        let output: DecredSigningOutput = AnySigner.sign(input: legacySigningInput, coin: .decred)
        XCTAssertEqual(output.error, .ok)
        let outputV2 = output.signingResultV2
        XCTAssertEqual(outputV2.error, .ok)
        XCTAssertEqual(outputV2.encoded.hexString, "0100000001aefeacac88c31b8ad5aaeaa71565c1042d80e4a77b12d1a7439e0cc21f3bccc50000000000ffffffff03809698000000000000001976a914f90f06478396b97df24c012b922f953fa894676188ac404b4c000000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988acf6f310050000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac00000000000000000100e1f5050000000000000000ffffffff6b483045022100c5464db9df8196c563db40ba1f7650291c755713ad87920a70c38c15db17bc3d02201c6bc9e4f8e43fdd548b0be387dd7baf7270bced8c9da4148128a655bed52e1a01210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b");
        XCTAssertEqual(outputV2.txid.hexString, "686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f163409")
    }
}
