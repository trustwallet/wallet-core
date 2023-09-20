package com.trustwallet.core

actual object LibLoader {
    actual fun loadLibrary() {
        WalletCoreLibLoader.loadLibrary()
    }
}
