// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.nativezetachain

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos

class TestNativeZetaChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NativeZetaChainTransactionSigning() {
        val key = PrivateKey("8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(false)
        val from = AnyAddress(publicKey, CoinType.NATIVEZETACHAIN).description()

        val transferAmount = Cosmos.Amount.newBuilder().apply {
            // 0.3 ZETA
            amount = "300000000000000000"
            denom = "azeta"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = Cosmos.Message.Send.newBuilder().apply {
                fromAddress = from
                toAddress = "zeta1cscf4ldnkkz7f0wpveur6dpd0d6p2zxnsuu70y"
                addAllAmounts(listOf(transferAmount))
            }.build()
        }.build()

        val transferFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = Cosmos.SigningMode.Protobuf
            accountNumber = 2726346
            chainId = "athens_7001-1"
            sequence = 2
            fee = transferFee
            privateKey = ByteString.copyFrom(key.data())
            txHasher = Cosmos.TxHasher.Keccak256
            signerInfo = Cosmos.SignerInfo.newBuilder().apply {
                // Zetachain requires a compressed public key to sign a transaction,
                // however an uncompressed public key is used to generate address.
                publicKeyType = Cosmos.SignerPublicKeyType.Secp256k1
                jsonType = "ethermint/PubKeyEthSecp256k1"
                protobufType = "/ethermint.crypto.v1.ethsecp256k1.PubKey"
            }.build()
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.NATIVEZETACHAIN, Cosmos.SigningOutput.parser())

        // Successfully broadcasted (testnet):
        // https://explorer.zetachain.com/cosmos/tx/A2FC8816657856ED274C4418C3CAEAEE645561275F6C63AB5F8B1DCFB37341A0
        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpoBCpcBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEncKK3pldGExNHB5MzZzeDU3dWQ4MnQ5eXJrczl6Nmhkc3JwbjV4NmtteHMwbmUSK3pldGExY3NjZjRsZG5ra3o3ZjB3cHZldXI2ZHBkMGQ2cDJ6eG5zdXU3MHkaGwoFYXpldGESEjMwMDAwMDAwMDAwMDAwMDAwMBJhClkKTwooL2V0aGVybWludC5jcnlwdG8udjEuZXRoc2VjcDI1NmsxLlB1YktleRIjCiECho5+FjRBfbKt/Z/jggW/oP6gGJin/TBWXRP3BWo3wGUSBAoCCAEYAhIEEMCaDBpAgGvqca0w2N9wnHnnxS9HiVud4aQ9lNCumzgNIW6wOR4kvPScacGS1G3kwCw7wyI2NJL8M1eVYjafFIt2FpKl3w==\"}")
    }
}
