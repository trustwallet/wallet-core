// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class EvmosTests: XCTestCase {

    func testAddressData() throws {
        let wallet = HDWallet(strength: 256, passphrase: "")!
        let nativeEvmos = wallet.getAddressForCoin(coin: .nativeEvmos)
        let evmos = wallet.getAddressForCoin(coin: .evmos)

        let addr1 = AnyAddress(string: nativeEvmos, coin: .nativeEvmos)
        let addr2 = AnyAddress(string: evmos, coin: .evmos)

        XCTAssertEqual(addr1?.data.hexString, addr2?.data.hexString)
    }

    func testSigningNativeTransfer() {

        let wallet = HDWallet(mnemonic: "glue blanket noodle name bring castle degree vibrant great joy usual mother pyramid cat balance swear diagram green split goat token day arm shoe", passphrase: "")!
        let privateKey = wallet.getKeyForCoin(coin: .nativeEvmos)
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)

        XCTAssertEqual(publicKey.data.hexString, "049475c9fa23ec693667baa76c4da69b49cccfdf058c4dcb27ba67cfbc9082d9ed9074786560aa698b19bb9729526b1c75934f3d4a78f7be719e4386b749b36310")

        let fromAddress = AnyAddress(publicKey: publicKey, coin: .nativeEvmos)

        print(publicKey.compressed.data.hexString)

        XCTAssertEqual(fromAddress.description, "evmos1rk39dk3wff5nps7emuhv3ntkn3nsz6z2erqfr0")

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "evmos10k9lrrruap9nu96mxwwye2f6a5wazeh33kq67z" // 1p
            $0.amounts = [CosmosAmount.with {
                $0.amount = "200000000000000" // 0.0002 Evmos
                $0.denom = "aevmos"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 140000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1400000000000000"
                $0.denom = "aevmos"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1982243
            $0.chainID = "evmos_9001-2"
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .nativeEvmos)
        // https://www.mintscan.io/evmos/txs/B05D2047086B158665EC552879270AEF40AEAAFEE7D275B63E9674E3CC4C4E55
        let expected = """
        {"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpoBCpcBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEncKLGV2bW9zMXJrMzlkazN3ZmY1bnBzN2VtdWh2M250a24zbnN6NnoyZXJxZnIwEixldm1vczEwazlscnJydWFwOW51OTZteHd3eWUyZjZhNXdhemVoMzNrcTY3ehoZCgZhZXZtb3MSDzIwMDAwMDAwMDAwMDAwMBJ7ClcKTwooL2V0aGVybWludC5jcnlwdG8udjEuZXRoc2VjcDI1NmsxLlB1YktleRIjCiEClHXJ+iPsaTZnuqdsTaabSczP3wWMTcsnumfPvJCC2e0SBAoCCAESIAoaCgZhZXZtb3MSEDE0MDAwMDAwMDAwMDAwMDAQ4MUIGkAz9vh1EutbLrLZmRA4eK72bA6bhfMX0YnhtRl5jeaL3AYmk0qdrwG9XzzleBsZ++IokJIk47cgOOyvEjl92Jhj"}
        """
        XCTAssertJSONEqual(output.serialized, expected)
        XCTAssertEqual(output.errorMessage, "")
    }
}
