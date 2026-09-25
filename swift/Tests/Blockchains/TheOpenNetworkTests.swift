// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class TheOpenNetworkTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let data = Data(hexString: "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8")
        let privateKey = PrivateKey(data: data!)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromPublicKey() {
        let data = Data(hexString: "f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41")
        let publicKey = PublicKey(data: data!, type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromRawString() {
        let addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromBounceableString() {
        let addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromUserFriendlyString() {
        let addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressToBounceable() {
        let addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        let address = TONAddressConverter.toUserFriendly(address: addressString, bounceable: true, testnet: false)
        XCTAssertEqual(address, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testGenerateJettonAddress() {
        let mainAddress = "UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr"
        let mainAddressBoc = TONAddressConverter.toBoc(address: mainAddress)
        XCTAssertEqual(mainAddressBoc, "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU")

        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

        // Parse the `get_wallet_address` RPC response.
        let jettonAddressBocEncoded = "te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq"
        let jettonAddress = TONAddressConverter.fromBoc(boc: jettonAddressBocEncoded)
        XCTAssertEqual(jettonAddress, "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H")
    }
    
    func testBuildV4R2StateInit() {
        let publicKeyData = Data(hexString: "f229a9371fa7c2108b3d90ea22c9be705ff5d0cfeaee9cbb9366ff0171579357")!
        let publicKey = PublicKey(data: publicKeyData, type: .ed25519)!
        let stateInit = TONWallet.buildV4R2StateInit(publicKey: publicKey, workchain: 0, walletId: 0x29a9a317)!
        XCTAssertEqual(stateInit, "te6cckECFgEAAwQAAgE0AQIBFP8A9KQT9LzyyAsDAFEAAAAAKamjF/IpqTcfp8IQiz2Q6iLJvnBf9dDP6u6cu5Nm/wFxV5NXQAIBIAQFAgFIBgcE+PKDCNcYINMf0x/THwL4I7vyZO1E0NMf0x/T//QE0VFDuvKhUVG68qIF+QFUEGT5EPKj+AAkpMjLH1JAyx9SMMv/UhD0AMntVPgPAdMHIcAAn2xRkyDXSpbTB9QC+wDoMOAhwAHjACHAAuMAAcADkTDjDQOkyMsfEssfy/8ICQoLAubQAdDTAyFxsJJfBOAi10nBIJJfBOAC0x8hghBwbHVnvSKCEGRzdHK9sJJfBeAD+kAwIPpEAcjKB8v/ydDtRNCBAUDXIfQEMFyBAQj0Cm+hMbOSXwfgBdM/yCWCEHBsdWe6kjgw4w0DghBkc3RyupJfBuMNDA0CASAODwBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgBwgQEI1xj6ANM/yFQgR4EBCPRR8qeCEG5vdGVwdIAYyMsFywJQBs8WUAT6AhTLahLLH8s/yXP7AAIAbIEBCNcY+gDTPzBSJIEBCPRZ8qeCEGRzdHJwdIAYyMsFywJQBc8WUAP6AhPLassfEss/yXP7AAAK9ADJ7VQAeAH6APQEMPgnbyIwUAqhIb7y4FCCEHBsdWeDHrFwgBhQBMsFJs8WWPoCGfQAy2kXyx9SYMs/IMmAQPsABgCKUASBAQj0WTDtRNCBAUDXIMgBzxb0AMntVAFysI4jghBkc3Rygx6xcIAYUAXLBVADzxYj+gITy2rLH8s/yYBA+wCSXwPiAgEgEBEAWb0kK29qJoQICga5D6AhhHDUCAhHpJN9KZEM5pA+n/mDeBKAG3gQFImHFZ8xhAIBWBITABG4yX7UTQ1wsfgAPbKd+1E0IEBQNch9AQwAsjKB8v/ydABgQEI9ApvoTGACASAUFQAZrc52omhAIGuQ64X/wAAZrx32omhAEGuQ64WPwEXtMkg=")
    }
    
    func testMessageSigner() {
        // The private key has been derived by using [ton-mnemonic](https://www.npmjs.com/package/tonweb-mnemonic/v/0.0.2)
        // from the following mnemonic:
        // document shield addict crime broom point story depend suit satisfy test chicken valid tail speak fortune sound drill seek cube cheap body music recipe
        let privateKeyData = Data(hexString: "112d4e2e700a468f1eae699329202f1ee671d6b665caa2d92dea038cf3868c18")!
        let privateKey = PrivateKey(data: privateKeyData)!
        let message = "Hello world"
        let signature = TONMessageSigner.signMessage(privateKey: privateKey, message: message)!
        // The following signature has been computed by calling `window.ton.send("ton_personalSign", { data: "Hello world" });`.
        XCTAssertEqual(signature, "2490fbaa72aec0b77b19162bbbe0b0e3f7afd42cc9ef469f0494cd4a366a4bf76643300cd5991f66bce6006336742b8d1d435d541d244dcc013d428472e89504")
    }

    func testSign() {
        let privateKeyData = Data(hexString: "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0")!

        let transfer = TheOpenNetworkTransfer.with {
            $0.dest = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
            $0.amount = Data(hexString: "0A")!
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.bounceable = true
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.messages = [transfer]
            $0.privateKey = privateKeyData
            $0.sequenceNumber = 6
            $0.expireAt = 1671132440
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        let expectedString = "te6cckEBBAEArQABRYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4MAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAgFiYgAzffHi4B365BPJfIJk/F+URKU1UekJ6g4QK02ypVb22YhQAAAAAAAAAAAAAAAAAQMAAA08Nzs="

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testJettonTransferSign() {
        let privateKeyData = Data(hexString: "c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee")!
        
        let jettonTransfer = TheOpenNetworkJettonTransfer.with {
            $0.jettonAmount = Data(hexString: "1DCD6500")! //500 * 1000 * 1000
            $0.toOwner = "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8"
            $0.responseAddress = "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk"
            $0.forwardAmount = Data(hexString: "01")!;
        }
        
        let transfer = TheOpenNetworkTransfer.with {
            $0.dest = "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja"
            $0.amount = Data(hexString: "05F5E100")! //100 * 1000 * 1000
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.comment = "test comment"
            $0.bounceable = true
            $0.jettonTransfer = jettonTransfer
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.messages = [transfer]
            $0.privateKey = privateKeyData
            $0.sequenceNumber = 1
            $0.expireAt = 1787693046
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
        let expectedString = "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c="

        XCTAssertEqual(output.encoded, expectedString)
    }
    
    func testTransferCustomPayloadSign() {
        let privateKeyData = Data(hexString: "5525e673087587bc0efd7ab09920ef7d3c1bf6b854a661430244ca59ab19e9d1")!
        
        // Doge chatbot contract payload to be deployed.
        // Docs: https://docs.ton.org/develop/dapps/ton-connect/transactions#smart-contract-deployment
        let dogeChatbotStateInit = "te6cckEBBAEAUwACATQBAgEU/wD0pBP0vPLICwMAEAAAAZDrkbgQAGrTMAGCCGlJILmRMODQ0wMx+kAwi0ZG9nZYcCCAGMjLBVAEzxaARfoCE8tqEssfAc8WyXP7AO4ioYU="
        // Doge chatbot's address after the contract is deployed.
        let dogeChatbotDeployingAddress = "0:3042cd5480da232d5ac1d9cbe324e3c9eb58f167599f6b7c20c6e638aeed0335"
        
        // The comment has nothing to do with Doge chatbot.
        // It's just used to attach the following ASCII comment to the transaction:
        // "This transaction deploys Doge Chatbot contract"
        let commentPayload = "te6cckEBAQEANAAAZAAAAABUaGlzIHRyYW5zYWN0aW9uIGRlcGxveXMgRG9nZSBDaGF0Ym90IGNvbnRyYWN0v84vSg=="
        
        let transfer = TheOpenNetworkTransfer.with {
            $0.dest = dogeChatbotDeployingAddress
            // 0.069 TON
            $0.amount = Data(hexString: "041CDB40")! //69_000_000
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.bounceable = false
            $0.stateInit = dogeChatbotStateInit
            $0.customPayload = commentPayload
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.messages = [transfer]
            $0.privateKey = privateKeyData
            $0.sequenceNumber = 4
            $0.expireAt = 1721939714
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // Successfully broadcasted: https://tonviewer.com/transaction/f4b7ed2247b1adf54f33dd2fd99216fbd61beefb281542d0b330ccea9b8d0338
        let expectedString = "te6cckECCAEAATcAAUWIAfq4NsPLegfou/MPhtHE9YuzV3gnI/q6jm3MRJh2PtpaDAEBnPbyCSsWrOZpEjb7ZFxz5yYi+an6M6Lnq7rI7TFWdDS76LEtGBrVVrhMGziwxuy6LCVtsMBikI7RPVQ89FCIAAYpqaMXZqK3AgAAAAQAAwICaUIAGCFmqkBtEZatYOzl8ZJx5PWseLOsz7W+EGNzHFd2gZqgIObaAAAAAAAAAAAAAAAAAAPAAwQCATQFBgBkAAAAAFRoaXMgdHJhbnNhY3Rpb24gZGVwbG95cyBEb2dlIENoYXRib3QgY29udHJhY3QBFP8A9KQT9LzyyAsHABAAAAGQ65G4EABq0zABgghpSSC5kTDg0NMDMfpAMItGRvZ2WHAggBjIywVQBM8WgEX6AhPLahLLHwHPFslz+wAa2r/S"

        XCTAssertEqual(output.encoded, expectedString)
    }
}
