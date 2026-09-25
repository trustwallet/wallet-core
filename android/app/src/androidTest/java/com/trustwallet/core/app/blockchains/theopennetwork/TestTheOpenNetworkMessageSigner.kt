// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.theopennetwork

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.TONMessageSigner
import wallet.core.jni.TONWallet

class TestTheOpenNetworkMessageSigner {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun TheOpenNetworkMessageSignerSignMessage() {
        // The private key has been derived by using [ton-mnemonic](https://www.npmjs.com/package/tonweb-mnemonic/v/0.0.2)
        // from the following mnemonic:
        // document shield addict crime broom point story depend suit satisfy test chicken valid tail speak fortune sound drill seek cube cheap body music recipe
        val privateKey = PrivateKey("112d4e2e700a468f1eae699329202f1ee671d6b665caa2d92dea038cf3868c18".toHexByteArray())
        val message = "Hello world"
        val signature = TONMessageSigner.signMessage(privateKey, message)
        // The following signature has been computed by calling `window.ton.send("ton_personalSign", { data: "Hello world" });`.
        assertEquals(signature, "2490fbaa72aec0b77b19162bbbe0b0e3f7afd42cc9ef469f0494cd4a366a4bf76643300cd5991f66bce6006336742b8d1d435d541d244dcc013d428472e89504")
    }
}
