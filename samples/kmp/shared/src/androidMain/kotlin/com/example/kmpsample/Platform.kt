package com.example.kmpsample

class AndroidPlatform : Platform {
    init {
        System.loadLibrary("TrustWalletCore")
    }
    override val name: String = "Android ${android.os.Build.VERSION.SDK_INT}"
}

actual fun getPlatform(): Platform = AndroidPlatform()