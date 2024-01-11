// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class NervosTests: XCTestCase {

    func testDerive() throws {
        let wallet = HDWallet(mnemonic: "disorder wolf eager ladder fence renew dynamic idea metal camera bread obscure", passphrase: "")!
        let address = wallet.getAddressForCoin(coin: .nervos)

        XCTAssertEqual(address, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqga4k4agxexsd3zdq0wvrlyumfz7n5r7fsjxtnw8")
    }

    func testAddress() throws {

        let key = PrivateKey(data: Data(hexString: "8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .nervos)
        let addressFromString = AnyAddress(string: "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras980hksatlslfaktks7epf25", coin: .nervos)!

        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() throws {

        let string = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8furras980hksatlslfaktks7epf25"
        let address = NervosAddress(string: string)!
        let lockScript = NervosScript.with {
            $0.codeHash = address.codeHash
            $0.hashType = address.hashType
            $0.args = address.args
        }

        let input = NervosSigningInput.with {
            $0.nativeTransfer = NervosNativeTransfer.with {
                $0.toAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02wectaumxn0664yw2jd53lqk4mxg3"
                $0.changeAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama"
                $0.amount = 10000000000
            }
            $0.byteFee = 1
            $0.cell = [
                NervosCell.with {
                    $0.capacity = 100000000000
                    $0.outPoint = NervosOutPoint.with {
                        $0.txHash = Data(hexString: "71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3")!
                        $0.index = 1
                    }
                    $0.lock = lockScript
                },
                NervosCell.with {
                    $0.capacity = 20000000000
                    $0.outPoint = NervosOutPoint.with {
                        $0.txHash = Data(hexString: "71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3")!
                        $0.index = 0
                    }
                    $0.lock = lockScript
                }
            ]
            $0.privateKey = [
                Data(hexString: "8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb")!
            ]
        }

        let output: NervosSigningOutput = AnySigner.sign(input: input, coin: .nervos)
        let json = """
        {
            "cell_deps": [
                {
                    "dep_type": "dep_group",
                    "out_point": {
                        "index": "0x0",
                        "tx_hash": "0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"
                    }
                }
            ],
            "header_deps": [],
            "inputs": [
                {
                    "previous_output": {
                        "index": "0x0",
                        "tx_hash": "0x71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3"
                    },
                    "since": "0x0"
                }
            ],
            "outputs": [
                {
                    "capacity": "0x2540be400",
                    "lock": {
                        "args": "0xab201f55b02f53b385f79b34dfad548e549b48fc",
                        "code_hash": "0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8",
                        "hash_type": "type"
                    },
                    "type": null
                },
                {
                    "capacity": "0x2540be230",
                    "lock": {
                        "args": "0xb0d65be39059d6489231b48f85ad706a560bbd8d",
                        "code_hash": "0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8",
                        "hash_type": "type"
                    },
                    "type": null
                }
            ],
            "outputs_data": ["0x", "0x"],
            "version": "0x0",
            "witnesses": [
                "0x55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5bf6e29cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700"
            ]
        }
        """

        XCTAssertEqual(output.transactionID, "0xf2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8")
        XCTAssertEqual(output.error, CommonSigningError.ok)
        XCTAssertJSONEqual(output.transactionJson, json)
    }
}
