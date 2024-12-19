package com.trustwallet.core

actual object LibLoader {
    actual fun loadLibrary() {
        System.loadLibrary("TrustWalletCore")
    }
}
