// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class CardanoTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fae0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276")!)!
        let pubkey = key.getPublicKeyEd25519Extended()
        let address = AnyAddress(publicKey: pubkey, coin: .cardano)
        let addressFromString = AnyAddress(string: "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq", coin: .cardano)!

        XCTAssertEqual(pubkey.data.hexString, "fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26faf4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testDeriveAddressWallet() {
        let wallet = HDWallet(mnemonic: "cost dash dress stove morning robust group affair stomach vacant route volume yellow salute laugh", passphrase: "")!
        let privateKey = wallet.getKeyForCoin(coin: .cardano)
        XCTAssertEqual(privateKey.data.hexString, "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fae0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276")
        let address = CoinType.cardano.deriveAddress(privateKey: privateKey)
        XCTAssertEqual(address, "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq")
    }

    func testSignTransfer1() {
        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5"
            $0.transferMessage.changeAddress = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
            $0.transferMessage.amount = 7000000
            $0.ttl = 53333333
        }
        input.privateKey.append(Data(hexString: "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e")!)

        let utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")!
            $0.outPoint.outputIndex = 1
            $0.address = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
            $0.amount = 1500000
        }
        input.utxos.append(utxo1)

        let utxo2 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0")!
            $0.outPoint.outputIndex = 0
            $0.address = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
            $0.amount = 6500000
        }
        input.utxos.append(utxo2)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389")
    }

    func testSignTransferToken1() throws {
        let toToken = CardanoTokenAmount.with {
            $0.policyID = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77"
            $0.assetName = "SUNDAE"
            $0.amount = Data(hexString: "01312d00")! // 20000000
        }
        var toTokenBundle = CardanoTokenBundle();
        toTokenBundle.token.append(toToken)

        // check min ADA amount, set it
        let inputTokenAmountSerialized = try toTokenBundle.serializedData()
        let minAmount = CardanoMinAdaAmount(tokenBundle: inputTokenAmountSerialized)
        XCTAssertEqual(minAmount, 1444443)

        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5"
            $0.transferMessage.changeAddress = "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq"
            $0.transferMessage.amount = minAmount
            $0.transferMessage.useMaxAmount = false
            $0.transferMessage.tokenAmount = toTokenBundle
            $0.ttl = 53333333
        }
        input.privateKey.append(Data(hexString: "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e")!)

        var utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")!
            $0.outPoint.outputIndex = 1
            $0.address = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
            $0.amount = 8051373
        }
        let token3 = CardanoTokenAmount.with {
            $0.policyID = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77"
            $0.assetName = "CUBY"
            $0.amount = Data(hexString: "2dc6c0")! // 3000000
        }
        utxo1.tokenAmount.append(token3)
        input.utxos.append(utxo1)

        var utxo2 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")!
            $0.outPoint.outputIndex = 2
            $0.address = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
            $0.amount = 2000000
        }
        let token1 = CardanoTokenAmount.with {
            $0.policyID = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77"
            $0.assetName = "SUNDAE"
            $0.amount = Data(hexString: "04d3e8d9")! // 80996569
        }
        utxo2.tokenAmount.append(token1)
        let token2 = CardanoTokenAmount.with {
            $0.policyID = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77"
            $0.assetName = "CUBY"
            $0.amount = Data(hexString: "1e8480")! // 2000000
        }
        utxo2.tokenAmount.append(token2)
        input.utxos.append(utxo2)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76702018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a00160a5ba1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a0080a574a2581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a144435542591a004c4b40581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a03a2bbd9021a0002af5e031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290584000feb412442f8851faa59742eb2c37f3994b0d143a424367143490cf828246991e504fa8eac61c403bfa7634bd1f0adc44f3f54f6a474856701e2cbb15fb5b04f6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "dacb3a0c5b3b7fa36b49f25a0a59b941ab8a21f0db5770e9e6982ff120122649")
    }
}
