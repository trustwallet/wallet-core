package com.trustwallet.core.app.helpers

import wallet.core.jni.*
import wallet.core.jni.CoinType.CARDANO
import wallet.core.jni.proto.Cardano
import wallet.core.jni.proto.Cardano.SigningInput
import wallet.core.jni.proto.Cardano.SigningOutput
import wallet.core.jni.proto.Common.SigningError

/*
public func CardanoMinAdaAmount(tokenBundle: Data) -> UInt64 {
    let tokenBundleData = TWDataCreateWithNSData(tokenBundle)
    defer {
        TWDataDelete(tokenBundleData)
    }
    return TWCardanoMinAdaAmount(tokenBundleData)
}
 */

fun CardanoMinAdaAmount(): int {
    return 0
}

