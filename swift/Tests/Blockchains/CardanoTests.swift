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
            "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca99d021a000298a3031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403c1cbde706d10550f5f966a5071e9e01e18f7b555626ed7616a214bd951d29d0ff0458edf5c555eefe4d4280302160149fc6e7e56a4298a368b7ab357643190df6")

        let txid = output.txID
        XCTAssertEqual(txid.hexString, "efcf8ec01fb8cd32bc5289c609c470b473cc79bc60b0667e1d68dc3962df1082")
    }
}
