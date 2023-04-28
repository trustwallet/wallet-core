// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AlgorandTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a6c4394041e64fe93d889386d7922af1b9a87f12e433762759608e61434d6cf7")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .algorand)
        let addressFromString = AnyAddress(string: "ADIYK65L3XR5ODNNCUIQVEET455L56MRKJHRBX5GU4TZI2752QIWK4UL5A", coin: .algorand)!

        XCTAssertEqual(pubkey.data.hexString, "00d1857babdde3d70dad15110a9093e77abef991524f10dfa6a727946bfdd411")
        XCTAssertEqual(address.description, addressFromString.description)
    }
    
    func testSignNFTTransfer() {
        // Successfully broadcasted: https://algoexplorer.io/tx/FFLUH4QKZHG744RIQ2AZNWZUSIIH262KZ4MEWSY4RXMWN5NMOOJA
        let round: UInt64 = 27963950
        let transaction = AlgorandAssetTransfer.with {
            $0.toAddress = "362T7CSXNLIOBX6J3H2SCPS4LPYFNV6DDWE6G64ZEUJ6SY5OJIR6SB5CVE"
            $0.amount = 1
            $0.assetID = 989643841
        }
        let input = AlgorandSigningInput.with {
            $0.genesisID = "mainnet-v1.0"
            $0.genesisHash = Data(base64Encoded: "wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=")!
            $0.note = "TWT TO THE MOON".data(using: .utf8)!
            $0.privateKey = Data(hexString: "dc6051ffc7b3ec601bde432f6dea34d40fe3855e4181afa0f0524c42194a6da7")!
            $0.firstRound = round
            $0.lastRound = round + 1000
            $0.fee = 1000
            $0.assetTransfer = transaction
        }
        let output: AlgorandSigningOutput = AnySigner.sign(input: input, coin: .algorand)

        XCTAssertEqual(output.signature, "nXQsDH1ilG3DIo2VQm5tdYKXe9o599ygdqikmROpZiNXAvQeK3avJqgjM5o+iByCdq6uOxlbveDyVmL9nZxxBg==")
    }

    func testSign() {
        let round: UInt64 = 1937767
        let transaction = AlgorandTransfer.with {
            $0.toAddress = "CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4"
            $0.amount = 1000000000000
        }
        let input = AlgorandSigningInput.with {
            $0.genesisID = "mainnet-v1.0"
            $0.genesisHash = Data(base64Encoded: "wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=")!
            $0.note = "hello".data(using: .utf8)!
            $0.privateKey = Data(hexString: "d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b")!
            $0.firstRound = round
            $0.lastRound = round + 1000
            $0.fee = 263000
            $0.transfer = transaction
        }
        let output: AlgorandSigningOutput = AnySigner.sign(input: input, coin: .algorand)

        XCTAssertEqual(output.encoded.hexString, "82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179")
    }

    func testSignVoteTx() {
        // manual vote tx is 0 amount + note

        let round: UInt64 = 18426344
        let transaction = AlgorandTransfer.with {
            $0.toAddress = "57QZ4S7YHTWPRAM3DQ2MLNSVLAQB7DTK4D7SUNRIEFMRGOU7DMYFGF55BY"
            $0.amount = 0
        }
        let note = """
        af/gov1:j{"com":1000000}
        """
        let input = AlgorandSigningInput.with {
            $0.genesisID = "mainnet-v1.0"
            $0.genesisHash = Data(base64Encoded: "wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=")!
            $0.note = note.data(using: .utf8)!
            $0.privateKey = Data(hexString: "d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b")!
            $0.firstRound = round
            $0.lastRound = round + 1000
            $0.fee = 1000
            $0.transfer = transaction
        }
        let output: AlgorandSigningOutput = AnySigner.sign(input: input, coin: .algorand)

        // real key is 1p, posted by: echo '<raw_tx>' | xxd -r -p | curl -X POST --data-binary @- <endpoint>
        // https://algoexplorer.io/tx/OHYNQA7X5LHUKWEM6ZMUT6RCVOZUELXSYELV7CHQFQBDI3XEM4NQ
        XCTAssertEqual(output.encoded.hexString, "82a3736967c440aad1e2d80fbdfc4dc5def13e1dc9f39c9261df9d5c6664478b951d28c3a688c4a261894d8c9bd686f5b2355f2edd54fd611eeaba8a871cc05af728a18598ed04a374786e89a3666565cd03e8a26676ce011929e8a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce01192dd0a46e6f7465c41861662f676f76313a6a7b22636f6d223a313030303030307da3726376c420efe19e4bf83cecf8819b1c34c5b65558201f8e6ae0ff2a36282159133a9f1b30a3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179")
    }

    func testSignJSON() {
        let json = """
            {"genesisId":"mainnet-v1.0","genesisHash":"wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=","note":"aGVsbG8=","firstRound":"1937767","lastRound":"1938767","fee":"263000","transfer":{"toAddress":"CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4","amount":"1000000000000"}}
        """
        let key = Data(hexString: "d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b")!
        let result = AnySigner.signJSON(json, key: key, coin: .algorand)

        XCTAssertEqual(result, "82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179")
    }
}
