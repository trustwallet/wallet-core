// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class FIOTests: XCTestCase {

    func testAddressFromString() {
        let valid = "FIO8KkL3ne7pv9evoNwFMYuCFZ4xRQKY2kgGN8N9PeRnEUc42itKK"
        let invalid = "EOS7y9UXLsC4jYU3k3NFweZZPWgRpRtvxBQj5TC3cnDVPsgyKfmW1"
        let address = AnyAddress(string: valid, coin: .fio)

        XCTAssertNotNil(address)
        XCTAssertEqual(address?.description, valid)
        XCTAssertTrue(AnyAddress.isValid(string: valid, coin: .fio))

        XCTAssertNil(AnyAddress(string: invalid, coin: .fio))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .fio))
    }

    func testAddressFromKey() {
        let key = PrivateKey(data: Data(hexString: "ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854")!)!

        let address = AnyAddress(publicKey: key.getPublicKeySecp256k1(compressed: true), coin: .fio)
        XCTAssertEqual(address.description, "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o")
    }

    func testRegisterFioAddress() {
        let chainId = Data(hexString: "4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77")!
        let privateKey = PrivateKey(data: Data(hexString: "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: publicKey, coin: .fio)

        let chainParams = FIOChainParams.with {
            $0.chainID = chainId
            $0.headBlockNumber = 39881
            $0.refBlockPrefix = 4279583376
        }
        let regAddrAction = FIOAction.RegisterFioAddress.with {
            $0.fioAddress = "adam@fiotestnet"
            $0.ownerFioPublicKey = address.description
            $0.fee = 5000000000
        }
        let action = FIOAction.with {
            $0.registerFioAddressMessage = regAddrAction
        }
        let input = FIOSigningInput.with {
            $0.expiry = 1579784511
            $0.chainParams = chainParams
            $0.privateKey = privateKey.data
            $0.tpid = "rewards@wallet"
            $0.action = action
        }

        let out: FIOSigningOutput = AnySigner.sign(input: input, coin: .fio)
        XCTAssertEqual(out.error, TW_Common_Proto_SigningError.ok)
        let expectedJson: String =
"""
{"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]}
"""

        XCTAssertEqual(out.json, expectedJson)
    }

    func testAddPubAddress() {
        let chainId = Data(hexString: "4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77")!
        let privateKey = PrivateKey(data: Data(hexString: "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035")!)!

        let chainParams = FIOChainParams.with {
            $0.chainID = chainId
            $0.headBlockNumber = 11565
            $0.refBlockPrefix = 4281229859
        }
        let addAddrAction = FIOAction.AddPubAddress.with {
            $0.fioAddress = "adam@fiotestnet"
            $0.publicAddresses = [
                FIOPublicAddress.with { $0.coinSymbol = "BTC"; $0.address = "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v" },
                FIOPublicAddress.with { $0.coinSymbol = "ETH"; $0.address = "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51" },
                FIOPublicAddress.with { $0.coinSymbol = "BNB"; $0.address = "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s" }
            ]
            $0.fee = 0
        }
        let action = FIOAction.with {
            $0.addPubAddressMessage = addAddrAction
        }
        let input = FIOSigningInput.with {
            $0.expiry = 1579729429
            $0.chainParams = chainParams
            $0.privateKey = privateKey.data
            $0.tpid = "rewards@wallet"
            $0.action = action
        }

        let out: FIOSigningOutput = AnySigner.sign(input: input, coin: .fio)
        XCTAssertEqual(out.error, TW_Common_Proto_SigningError.ok)
        let expectedJson: String =
"""
{"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K3zimaMKU8cBhVRPw46KM2u7uQWaAKXrnoeYZ7MEbp6sVJcDQmQR2RtdavpUPwkAnYUkd8NqLun8H48tcxZBcTUgkiPGVJ"]}
"""

        XCTAssertEqual(out.json, expectedJson)
    }

    func testTransfer() {
        let chainId = Data(hexString: "4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77")!
        let privateKey = PrivateKey(data: Data(hexString: "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035")!)!

        let chainParams = FIOChainParams.with {
            $0.chainID = chainId
            $0.headBlockNumber = 50000
            $0.refBlockPrefix = 4000123456
        }
        let transferAction = FIOAction.Transfer.with {
            $0.payeePublicKey = "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE"
            $0.amount = 1000000000
            $0.fee = 250000000
        }
        let action = FIOAction.with {
            $0.transferMessage = transferAction
        }
        let input = FIOSigningInput.with {
            $0.expiry = 1579790000
            $0.chainParams = chainParams
            $0.privateKey = privateKey.data
            $0.tpid = "rewards@wallet"
            $0.action = action
        }

        let out: FIOSigningOutput = AnySigner.sign(input: input, coin: .fio)
        XCTAssertEqual(out.error, TW_Common_Proto_SigningError.ok)
        let expectedJson: String =
"""
{"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]}
"""

        XCTAssertEqual(out.json, expectedJson)
    }

    func testAccountNames() {
        XCTAssertEqual(
            FIOAccount(string: "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE")!.description, "hhq2g4qgycfb"
        )
        XCTAssertEqual(
            FIOAccount(string: "hhq2g4qgycfb")!.description, "hhq2g4qgycfb"
        )

        XCTAssertEqual(
            FIOAccount(string: "rewards@wallet")!.description, "rewards@wallet"
        )

        XCTAssertNil(FIOAccount(string: "asdf19s"))
        XCTAssertNil(FIOAccount(string: "0x320196ef1b137786be51aa391e78e0a2c756f46b"))
    }
}
