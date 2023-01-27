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
