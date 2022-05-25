package com.trustwallet.core.app.helpers

import wallet.core.jni.*
import wallet.core.jni.Cardano
//import wallet.core.jni.CoinType.CARDANO
import wallet.core.jni.proto.Cardano
//import wallet.core.jni.proto.Cardano.SigningInput
//import wallet.core.jni.proto.Cardano.SigningOutput
//import wallet.core.jni.proto.Common.SigningError

/*
public func CardanoMinAdaAmount(tokenBundle: Data) -> UInt64 {
    let tokenBundleData = TWDataCreateWithNSData(tokenBundle)
    defer {
        TWDataDelete(tokenBundleData)
    }
    return TWCardanoMinAdaAmount(tokenBundleData)
}
 */

fun CardanoMinAdaAmount(tokenBundleData: ByteArray): Long {
    //Cardano.minAdaAmount(tokenBundleData);
    //minAdaAmount(tokenBundleData);
    return jni.Cardano.minAdaAmount(tokenBundleData);
}

