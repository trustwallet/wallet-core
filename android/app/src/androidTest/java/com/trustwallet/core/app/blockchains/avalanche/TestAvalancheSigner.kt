// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.avalanche

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.proto.Avalanche

class TestAvalancheSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun AvalancheTransactionSigning() {
        val privKeyBytes = Numeric.hexStringToByteArray("56289e99c94b6912bfc12adc093c9b51124f0dc54ac7a766b2bc5ccf558d8027")
        val key = PrivateKey(privKeyBytes)
        val blockchainID = ByteString.copyFrom(Numeric.hexStringToByteArray("d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf"))
        val pubkey = key.getPublicKeySecp256k1(true)
        val pubkeyBytes = ByteString.copyFrom(pubkey.data())
        val spendableAddresses = arrayOf(pubkeyBytes, pubkeyBytes, pubkeyBytes, pubkeyBytes, pubkeyBytes, pubkeyBytes, pubkeyBytes, pubkeyBytes).asIterable()
        val netID = 12345
        val assetID = ByteString.copyFrom(Numeric.hexStringToByteArray("dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db"))
        val txID = ByteString.copyFrom(Numeric.hexStringToByteArray("f1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"))
        val memo = ByteString.copyFrom(Numeric.hexStringToByteArray("deadbeef"))
        val amount = 1000.toLong()
        val locktime = 0.toLong()
        val threshold = 1


        val coreInputOne = Avalanche.SECP256K1TransferInput.newBuilder()
                .setAmount(123456789)
                .addAddressIndices(3)
                .addAddressIndices(7)
                .build()
        val wrappedInputOne = Avalanche.TransactionInput.newBuilder()
                .setSecpTransferInput(coreInputOne)
                .build()
        val inputOne = Avalanche.TransferableInput.newBuilder()
                .setUtxoIndex(5)
                .setTxId(txID)
                .setAssetId(assetID)
                .addAllSpendableAddresses(spendableAddresses)
                .setInput(wrappedInputOne)
                .build()

        val coreInputTwo = Avalanche.SECP256K1TransferInput.newBuilder()
                .setAmount(123456789)
                .addAddressIndices(3)
                .addAddressIndices(7)
                .build()
        val wrappedInputTwo = Avalanche.TransactionInput.newBuilder()
                .setSecpTransferInput(coreInputTwo)
                .build()
        val inputTwo = Avalanche.TransferableInput.newBuilder()
                .setUtxoIndex(5)
                .setTxId(txID)
                .setAssetId(assetID)
                .addAllSpendableAddresses(spendableAddresses)
                .setInput(wrappedInputTwo)
                .build()


        val transfer = Avalanche.SimpleTransferTx.newBuilder()
                .addInputs(inputOne)
                .addInputs(inputTwo)
                .setTypeId(0)
                .setNetworkId(netID)
                .setBlockchainId(blockchainID)
                .setAmount(180000000)
                .setFee(1200000)
                .addToAddresses(pubkeyBytes)
                .addChangeAddresses(pubkeyBytes)
                .setUseMaxAmount(false)
                .setOutputTypeId(0)
                .setAssetId(assetID)
                .setMemo(memo)
                .setLocktime(locktime)
                .setThreshold(threshold)
                .build()

        val signingInput = Avalanche.SigningInput.newBuilder()
                .addPrivateKeys(ByteString.copyFrom(privKeyBytes))
                .setSimpleTransaction(transfer)
                .build()

        val output = AnySigner.sign(signingInput, CoinType.AVALANCHEXCHAIN, Avalanche.SigningOutput.parser())
        assertEquals(
            "0x00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000003eab5aa000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db00000007000000000aba9500000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf0100000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf01",
            Numeric.toHexString(output.encoded.toByteArray())
        )
    }
}
