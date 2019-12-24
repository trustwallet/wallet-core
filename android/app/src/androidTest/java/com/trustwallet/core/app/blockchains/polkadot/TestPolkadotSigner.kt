// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.polkadot

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PolkadotSigner
import wallet.core.jni.proto.Polkadot

class TestPolkadotSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun PolkadotTransactionSigning() {
        val key = "0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115".toHexBytesInByteString()
        val hash = "0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe".toHexBytesInByteString()

        val call = Polkadot.Balance.Transfer.newBuilder().apply {
            toAddress = "FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP"
            value = "3039".toHexBytesInByteString()
        }.build()

        val signingInput = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = hash
            blockHash = hash
            nonce = 0
            specVersion = 1031
            network = Polkadot.Network.KUSAMA
            extrinsicVersion = 4
            privateKey = key
            balanceCall = Polkadot.Balance.newBuilder().apply {
                transfer = call
            }.build()
        }.build()

        val output = PolkadotSigner.sign(signingInput)
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        val expected = "0x2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0034a113577b56545c45e18969471eebe11ed434f3b2f06e2e3dc8dc137ba804caf60757787ebdeb298327e2f29d68c5520965405ef5582db0445c06e1c11a8a0e0000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0"
        assertEquals(encoded, expected)
    }
}
