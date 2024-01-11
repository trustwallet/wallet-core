// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import com.trustwallet.core.*
import kotlin.js.Promise

@JsExport
@JsName("WalletCoreKotlin")
object WalletCore {

    lateinit var Instance: JsWalletCore
        private set

    fun init(): Promise<JsWalletCore> =
        if (::Instance.isInitialized) {
            Promise.resolve(Instance)
        } else {
            WalletCoreExports.initWasm()
                .then { walletCore ->
                    Instance = walletCore
                    walletCore
                }
        }
}

@JsModule("@trustwallet/wallet-core")
@JsNonModule
private external object WalletCoreExports {
    fun initWasm(): Promise<JsWalletCore>
}
