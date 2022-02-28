// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
}
