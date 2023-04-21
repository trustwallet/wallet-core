// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
