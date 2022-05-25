package com.trustwallet.core.app.helpers

import wallet.core.jni.*
//import wallet.core.jni.Cardano
//import wallet.core.jni.CoinType.CARDANO
import wallet.core.jni.proto.Cardano
//import wallet.core.jni.proto.Cardano.SigningInput
//import wallet.core.jni.proto.Cardano.SigningOutput
//import wallet.core.jni.proto.Common.SigningError

fun CardanoMinAdaAmount(tokenBundleData: ByteArray): Long {
    //Cardano.minAdaAmount(tokenBundleData);
    //minAdaAmount(tokenBundleData);
    return wallet.core.jni.Cardano.minAdaAmount(tokenBundleData);
}

