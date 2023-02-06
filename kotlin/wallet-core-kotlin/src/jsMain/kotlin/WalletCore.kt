// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import kotlin.js.Promise

@JsExport
@JsName("WalletCoreKotlin")
object WalletCore {

    internal var Instance: dynamic = null

    fun init(): Promise<dynamic> =
        WalletCoreExports.initWasm()
            .then { walletCore: dynamic ->
                Instance = walletCore
                walletCore
            }
}

@JsModule("@trustwallet/wallet-core")
@JsNonModule
internal external object WalletCoreExports {
    fun initWasm(): Promise<dynamic>
}
