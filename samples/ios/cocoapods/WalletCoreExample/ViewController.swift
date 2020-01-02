// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Cocoa
import TrustWalletCore

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // 'Import' a wallet
        let wallet = HDWallet(mnemonic: "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", passphrase: "")
        print("Mnemonic: ", wallet.mnemonic)

        let coin: CoinType = .ethereum
        // Get the default address
        let address = wallet.getAddressForCoin(coin: coin)
        print("Default address: ", address)

        // Signing a transaction (using EthereumSigner)
        let secretPrivateKey = wallet.getKeyForCoin(coin: coin)
        let dummyReceiverAddress = "0xC37054b3b48C3317082E7ba872d7753D13da4986"
        let signerInput = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.gasPrice = Data(hexString: "d693a400")! // decimal 3600000000
            $0.gasLimit = Data(hexString: "5208")! // decimal 21000
            $0.toAddress = dummyReceiverAddress
            $0.amount = Data(hexString: "0348bca5a16000")!
            $0.privateKey = secretPrivateKey.data
        }
        let signerOutput = EthereumSigner.sign(input: signerInput)
        print("Signed transaction:")
        print(" data:   ", signerOutput.encoded.hexString)

        // Signing a transaction (using AnySigner)
        let secretPrivateKeyHex = secretPrivateKey.data.hexString
        let chainIdB64 = "AQ=="  // base64(parse_hex("01"))
        let gasPriceB64 = "1pOkAA=="  // base64(parse_hex("d693a4")) decimal 3600000000
        let gasLimitB64 = "Ugg="  // base64(parse_hex("5208")) decimal 21000
        let amountB64 = "A0i8paFgAA=="  // base64(parse_hex("0348bca5a160"))  924400000000000
        let transaction = "{" +
            "\"chainId\":\"" + chainIdB64 +
            "\",\"gasPrice\":\"" + gasPriceB64 +
            "\",\"gasLimit\":\"" + gasLimitB64 +
            "\",\"toAddress\":\"" + dummyReceiverAddress +
            "\",\"amount\":\"" + amountB64 + "\"}"
        print("transaction: ", transaction)
        let anySignerInput = AnySigningInput.with {
            $0.coinType = coin.rawValue
            $0.transaction = transaction
            $0.privateKey = secretPrivateKeyHex
        }
        let anySignerOutput = AnySigner.sign(input: anySignerInput)
        if !anySignerOutput.success {
            print("Error:", anySignerOutput.error)
        } else {
            print("Signed transaction data:", anySignerOutput.output)
        }
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
}
