// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class CardanoTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fae0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276")!)!
        let pubkey = key.getPublicKeyEd25519Cardano()
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
    
    /// Successfully broadcasted:
    /// https://cardanoscan.io/transaction/0203ce2c91f59f169a26e9ef91254639d2b7911afac9c7c0ae64539f88ba46a5
    func testSignTransferFromLegacy() throws {
        let privateKey = PrivateKey(data: Data(hexString: "98f266d1aac660179bc2f456033941238ee6b2beb8ed0f9f34c9902816781f5a9903d1d395d6ab887b65ea5e344ef09b449507c21a75f0ce8c59d0ed1c6764eba7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f4e890ca4eb6bec44bf751b5a843174534af64d6ad1f44e0613db78a7018781f5aa151d2997f52059466b715d8eefab30a78b874ae6ef4931fa58bb21ef8ce2423d46f19d0fbf75afb0b9a24e31d533f4fd74cee3b56e162568e8defe37123afc4")!)!
        let publicKey = privateKey.getPublicKeyEd25519Cardano()
        let byronAddress = Cardano.getByronAddress(publicKey: publicKey)
        
        XCTAssertEqual(
            publicKey.data.hexString,
        "d163c8c4f0be7c22cd3a1152abb013c855ea614b92201497a568c5d93ceeb41ea7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f40b5aaa6103dc10842894a1eeefc5447b9bcb9bcf227d77e57be195d17bc03263d46f19d0fbf75afb0b9a24e31d533f4fd74cee3b56e162568e8defe37123afc4"
        )

        XCTAssertEqual(
            byronAddress,
            "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8"
        )
        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = "addr1q90uh2eawrdc9vaemftgd50l28yrh9lqxtjjh4z6dnn0u7ggasexxdyyk9f05atygnjlccsjsggtc87hhqjna32fpv5qeq96ls"
            $0.transferMessage.changeAddress = "addr1qx55ymlqemndq8gluv40v58pu76a2tp4mzjnyx8n6zrp2vtzrs43a0057y0edkn8lh9su8vh5lnhs4npv6l9tuvncv8swc7t08"
            $0.transferMessage.amount = 3000000
            $0.ttl = 190000000
        }

        input.privateKey.append(privateKey.data)

        let utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "8316e5007d61fb90652cabb41141972a38b5bc60954d602cf843476aa3f67f63")!
            $0.outPoint.outputIndex = 0
            $0.address = "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8"
            $0.amount = 2500000
        }
        input.utxos.append(utxo1)

        let utxo2 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "e29392c59c903fefb905730587d22cae8bda30bd8d9aeec3eca082ae77675946")!
            $0.outPoint.outputIndex = 0
            $0.address = "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8"
            $0.amount = 1700000
        }
        input.utxos.append(utxo2)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        XCTAssertEqual(output.encoded.hexString, "83a400828258208316e5007d61fb90652cabb41141972a38b5bc60954d602cf843476aa3f67f6300825820e29392c59c903fefb905730587d22cae8bda30bd8d9aeec3eca082ae77675946000182825839015fcbab3d70db82b3b9da5686d1ff51c83b97e032e52bd45a6ce6fe7908ec32633484b152fa756444e5fc62128210bc1fd7b8253ec5490b281a002dc6c082583901a9426fe0cee6d01d1fe32af650e1e7b5d52c35d8a53218f3d0861531621c2b1ebdf4f11f96da67fdcb0e1d97a7e778566166be55f193c30f1a000f9ec1021a0002b0bf031a0b532b80a20081825820d163c8c4f0be7c22cd3a1152abb013c855ea614b92201497a568c5d93ceeb41e58406a23ab9267867fbf021c1cb2232bc83d2cdd663d651d22d59b6cddbca5cb106d4db99da50672f69a2309ca8a329a3f9576438afe4538b013de4591a6dfcd4d090281845820d163c8c4f0be7c22cd3a1152abb013c855ea614b92201497a568c5d93ceeb41e58406a23ab9267867fbf021c1cb2232bc83d2cdd663d651d22d59b6cddbca5cb106d4db99da50672f69a2309ca8a329a3f9576438afe4538b013de4591a6dfcd4d095820a7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f441a0f6")

        XCTAssertEqual(output.txID.hexString, "0203ce2c91f59f169a26e9ef91254639d2b7911afac9c7c0ae64539f88ba46a5")
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
            $0.assetNameHex = "53554e444145"
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
            "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76702018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a00160a5ba1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a0080aac9a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a244435542591a004c4b404653554e4441451a03a2bbd9021a0002aa09031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840d90dcfbd190cbe59c42094e59eeb49b3de9d80a85b786cc311f932c5c9302d1c8c6c577b22aa70ff7955c139c700ea918f8cb425c3ba43a27980e1d238e4e908f6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "201c537693b005b64a0f0528e366ec67a84be0119ed4363b547f141f2a7770c2")
    }

    func testGetStakingAddress() throws {
        let stakingAddress = Cardano.getStakingAddress(baseAddress: "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
        XCTAssertEqual(stakingAddress, "stake1u80jysjtdzqt88jt4jx93h5lumfr67d273r4vwyasfa2pxcwxllmx")
    }

    func testSignStakingRegisterAndDelegate() throws {
        let ownAddress = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
        let stakingAddress = Cardano.getStakingAddress(baseAddress: ownAddress)
        let poolIdNufi = "7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6"

        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = ownAddress
            $0.transferMessage.changeAddress = ownAddress
            $0.transferMessage.amount = 4000000 // not relevant as we use MaxAmount
            $0.transferMessage.useMaxAmount = true
            $0.ttl = 69885081
            // Register staking key, 2 ADA desposit
            $0.registerStakingKey.stakingAddress = stakingAddress
            $0.registerStakingKey.depositAmount = 2000000
            // Delegate
            $0.delegate.stakingAddress = stakingAddress
            $0.delegate.poolID = Data(hexString: poolIdNufi)!
            $0.delegate.depositAmount = 0
        }
        input.privateKey.append(Data(hexString: "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e")!)

        let utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e")!
            $0.outPoint.outputIndex = 0
            $0.address = ownAddress
            $0.amount = 4000000
        }
        let utxo2 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "9b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e")!
            $0.outPoint.outputIndex = 1
            $0.address = ownAddress
            $0.amount = 26651312
        }
        input.utxos.append(utxo1)
        input.utxos.append(utxo2)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "83a500828258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e008258209b06de86b253549b99f6a050b61217d8824085ca5ed4eb107a5e7cce4f93802e01018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a01b27ef5021a0002b03b031a042a5c99048282008200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b83028200581cdf22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b581c7d7ac07a2f2a25b7a4db868a40720621c4939cf6aefbb9a11464f1a6a100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840677c901704be027d9a1734e8aa06f0700009476fa252baaae0de280331746a320a61456d842d948ea5c0e204fc36f3bd04c88ca7ee3d657d5a38014243c37c07825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e0693258401fa21bdc62b85ca217bf08cbacdeba2fadaf33dc09ee3af9cc25b40f24822a1a42cfbc03585cc31a370ef75aaec4d25db6edcf329e40a4e725ec8718c94f220af6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "96a781fd6481b6a7fd3926da110265e8c44b53947b81daa84da5b148825d02aa")
    }

    func testSignStakingWithdraw() throws {
        let ownAddress = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"
        let stakingAddress = Cardano.getStakingAddress(baseAddress: "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")

        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = ownAddress
            $0.transferMessage.changeAddress = ownAddress
            $0.transferMessage.amount = 6000000 // not relevant as we use MaxAmount
            $0.transferMessage.useMaxAmount = true
            $0.ttl = 71678326
            // Withdraw available amount
            $0.withdraw.stakingAddress = stakingAddress
            $0.withdraw.withdrawAmount = 3468
        }
        input.privateKey.append(Data(hexString: "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e")!)

        let utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "7dfd2c579794314b1f84efc9db932a098e440ccefb874945591f1d4e85a9152a")!
            $0.outPoint.outputIndex = 0
            $0.address = ownAddress
            $0.amount = 6305913
        }
        input.utxos.append(utxo1)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "83a500818258207dfd2c579794314b1f84efc9db932a098e440ccefb874945591f1d4e85a9152a00018182583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a005da6ff021a00029f06031a0445b97605a1581de1df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b190d8ca100828258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058401ebaca2876fd17122404912a2558a98109cdf0f990a938d2917fa2c3b8c4e55e18a2cbabfa82fff03fa0d7ab8b88ca01ed18e42af3bfc4cda7f423a3aa30c00b825820e554163344aafc2bbefe778a6953ddce0583c2f8e0a0686929c020ca33e069325840777f04fa8f083fe562aecf78898aaaaac36e2cc6ca962f6ffb01e84a421cae1860496db79b2c5fb2879524c3d5121060b9ea1e693336230c6e5338e14c4c3303f6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "6dcf3956232953fc25b8355fb1ded1e912b5802090fd21434d789087d6329683")
    }

    // Successfully broadcasted:
    // https://cardanoscan.io/transaction/87ca43a36b09c0b140f0ef2b71fbdcfcf1fdc88f7aa378b861e8eed3e8974628
    func testSignNftTransfer() throws {
        let fromAddress = "addr1qy5eme9r6frr0m6q2qpncg282jtrhq5lg09uxy2j0545hj8rv7v2ntdxuv6p4s3eq4lqzg39lewgvt6fk5kmpa0zppesufzjud"
        let toAddress = "addr1qy9wjfn6nd8kak6dd8z53u7t5wt9f4lx0umll40px5hnq05avwcsq5r3ytdp36wttzv4558jaq8lvhgqhe3y8nuf5xrquju7z4"
        let coinsPerUtxoByte = "4310";

        let tokenAmount = CardanoTokenAmount.with {
            $0.policyID = "219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5e"
            $0.assetNameHex = "636f6f6c63617473736f636965747934353637"
            $0.amount = Data(hexString: "01")! // 1
        };
        var input = CardanoSigningInput.with {
            $0.transferMessage.toAddress = toAddress
            $0.transferMessage.changeAddress = fromAddress
            $0.ttl = 89130965
        }
        input.transferMessage.tokenAmount.token.append(tokenAmount);
        
        // check min ADA amount, set it
        let inputTokenAmountSerialized = try input.transferMessage.tokenAmount.serializedData()
        let minAmount = Cardano.outputMinAdaAmount(toAddress: toAddress, tokenBundle: inputTokenAmountSerialized, coinsPerUtxoByte: coinsPerUtxoByte)!;
        let minAmountInt = UInt64(minAmount)!
        XCTAssertEqual(minAmountInt, 1202490)
        input.transferMessage.amount = minAmountInt
        
        input.privateKey.append(Data(hexString: "d09831a668db6b36ffb747600cb1cd3e3d34f36e1e6feefc11b5f988719b7557a7029ab80d3e6fe4180ad07a59ddf742ea9730f3c4145df6365fa4ae2ee49c3392e19444caf461567727b7fefec40a3763bdb6ce5e0e8c05f5e340355a8fef4528dfe7502cfbda49e38f5a0021962d52dc3dee82834a23abb6750981799b75577d1ed9af9853707f0ef74264274e71b2f12e86e3c91314b6efa75ef750d9711b84cedd742ab873ef2f9566ad20b3fc702232c6d2f5d83ff425019234037d1e58")!)

        let utxo1 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b8")!
            $0.outPoint.outputIndex = 0
            $0.address = fromAddress
            $0.amount = 1202490
            $0.tokenAmount = [tokenAmount]
        }
        input.utxos.append(utxo1)
        
        let utxo2 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840")!
            $0.outPoint.outputIndex = 0
            $0.address = fromAddress
            $0.amount = 1000000
        }
        input.utxos.append(utxo2)
        
        let utxo3 = CardanoTxInput.with {
            $0.outPoint.txHash = Data(hexString: "6a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa8639167")!
            $0.outPoint.outputIndex = 0
            $0.address = fromAddress
            $0.amount = 2000000
        }
        input.utxos.append(utxo3)

        // Sign
        let output: CardanoSigningOutput = AnySigner.sign(input: input, coin: .cardano)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "83a400838258206a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa863916700825820aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b800825820ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840000182825839010ae9267a9b4f6edb4d69c548f3cba39654d7e67f37ffd5e1352f303e9d63b100507122da18e9cb58995a50f2e80ff65d00be6243cf89a186821a0012593aa1581c219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5ea153636f6f6c63617473736f6369657479343536370182583901299de4a3d24637ef4050033c214754963b829f43cbc311527d2b4bc8e36798a9ada6e3341ac239057e012225fe5c862f49b52db0f5e208731a002b1525021a0002b19b031a055007d5a1008182582088bd26e8656fa7dead846c3373588f0192da5bfb90bf5d3fb877decfb3b3fd085840da8656aca0dacc57d4c2d957fc7dff03908f6dcf60c48f1e40b3006e2fd0cfacfa4c24fa02e35a310572526586d4ce0d30bf660ba274c8efd507848cbe177d09f6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "87ca43a36b09c0b140f0ef2b71fbdcfcf1fdc88f7aa378b861e8eed3e8974628")
    }
}
