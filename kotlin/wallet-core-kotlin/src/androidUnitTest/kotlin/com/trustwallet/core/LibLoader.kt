package com.trustwallet.core

actual object LibLoader {
    actual fun loadLibrary() {
        throw NotImplementedError()
    }
}
