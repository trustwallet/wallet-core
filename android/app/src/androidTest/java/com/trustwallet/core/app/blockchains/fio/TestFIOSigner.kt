// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.fio

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.FIO
import wallet.core.jni.proto.FIO.SigningOutput
import wallet.core.jni.proto.Common.SigningError

class TestFIOSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testRegisterFioAddress() {
        val chainId = ByteString.copyFrom("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77".toHexBytes())
        val privateKey = PrivateKey("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(publicKey, CoinType.FIO)

        val chainParams = FIO.ChainParams.newBuilder()
            .setChainId(chainId)
            .setHeadBlockNumber(39881)
            .setRefBlockPrefix(4279583376)
        val regAddrAction = FIO.Action.RegisterFioAddress.newBuilder()
            .setFioAddress("adam@fiotestnet")
            .setOwnerFioPublicKey(address.description())
            .setFee(5000000000)
        val action = FIO.Action.newBuilder()
            .setRegisterFioAddressMessage(regAddrAction)
        val input = FIO.SigningInput.newBuilder()
            .setExpiry(1579784511)
            .setChainParams(chainParams)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .setTpid("rewards@wallet")
            .setAction(action)

        val out = AnySigner.sign(input.build(), CoinType.FIO, SigningOutput.parser())
        assertEquals(SigningError.OK, out.error)
        val expectedJson =
"""{"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]}"""
        assertEquals(expectedJson, out.json)
    }

    @Test
    fun testAddPubAddress() {
        val chainId = ByteString.copyFrom("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77".toHexBytes())
        val privateKey = PrivateKey("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035".toHexByteArray())

        val chainParams = FIO.ChainParams.newBuilder()
            .setChainId(chainId)
            .setHeadBlockNumber(11565)
            .setRefBlockPrefix(4281229859)
        val addAddrAction = FIO.Action.AddPubAddress.newBuilder()
            .setFioAddress("adam@fiotestnet")
            .addPublicAddresses(FIO.PublicAddress.newBuilder().setCoinSymbol("BTC").setAddress("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"))
            .addPublicAddresses(FIO.PublicAddress.newBuilder().setCoinSymbol("ETH").setAddress("0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"))
            .addPublicAddresses(FIO.PublicAddress.newBuilder().setCoinSymbol("BNB").setAddress("bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"))
            .setFee(0)
        val action = FIO.Action.newBuilder()
            .setAddPubAddressMessage(addAddrAction)
        val input = FIO.SigningInput.newBuilder()
            .setExpiry(1579729429)
            .setChainParams(chainParams)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .setTpid("rewards@wallet")
            .setAction(action)

        val out = AnySigner.sign(input.build(), CoinType.FIO, SigningOutput.parser())
        assertEquals(SigningError.OK, out.error)
        val expectedJson =
"""{"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K3zimaMKU8cBhVRPw46KM2u7uQWaAKXrnoeYZ7MEbp6sVJcDQmQR2RtdavpUPwkAnYUkd8NqLun8H48tcxZBcTUgkiPGVJ"]}"""
        assertEquals(expectedJson, out.json)
    }

    @Test
    fun testTransfer() {
        val chainId = ByteString.copyFrom("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77".toHexBytes())
        val privateKey = PrivateKey("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035".toHexByteArray())

        val chainParams = FIO.ChainParams.newBuilder()
            .setChainId(chainId)
            .setHeadBlockNumber(50000)
            .setRefBlockPrefix(4000123456)
        val transferAction = FIO.Action.Transfer.newBuilder()
            .setPayeePublicKey("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE")
            .setAmount(1000000000)
            .setFee(250000000)
        val action = FIO.Action.newBuilder()
            .setTransferMessage(transferAction)
        val input = FIO.SigningInput.newBuilder()
            .setExpiry(1579790000)
            .setChainParams(chainParams)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .setTpid("rewards@wallet")
            .setAction(action)

        val out = AnySigner.sign(input.build(), CoinType.FIO, SigningOutput.parser())
        assertEquals(SigningError.OK, out.error)
        val expectedJson =
"""{"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]}"""
        assertEquals(expectedJson, out.json)
    }
}
