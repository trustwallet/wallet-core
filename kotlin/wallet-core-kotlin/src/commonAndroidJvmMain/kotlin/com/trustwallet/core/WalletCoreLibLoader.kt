package com.trustwallet.core

import com.trustwallet.core.WalletCoreLibLoader.Linux64Path
import com.trustwallet.core.WalletCoreLibLoader.MacArm64Path
import java.io.File
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Simple helper class that extracts proper native library for current OS/arch from .jar to temporary file and loads it.
 * You can do the same by yourself, just use path constants: [MacArm64Path], [Linux64Path]
 */
object WalletCoreLibLoader {

    const val MacArm64Path = "/jni/macos-arm64/libTrustWalletCore.dylib"
    const val Linux64Path = "/jni/linux-x86_64/libTrustWalletCore.so"

    private val isLoaded = AtomicBoolean(false)

    @JvmStatic
    fun loadLibrary() {
        if (isLoaded.compareAndSet(false, true)) {
            val resLibPath = getLibResourcePath()
            val resLibStream = WalletCoreLibLoader::class.java.getResourceAsStream(resLibPath)
                ?: error("File not found: $resLibPath")

            val fileOut = File.createTempFile("libTrustWalletCore", null)
            fileOut.deleteOnExit()

            resLibStream.copyTo(fileOut.outputStream())

            @Suppress("UnsafeDynamicallyLoadedCode")
            System.load(fileOut.absolutePath)
        }
    }

    private fun getLibResourcePath(): String {
        val osNameOriginal = System.getProperty("os.name").lowercase()
        val osName = osNameOriginal.lowercase()
        val archOriginal = System.getProperty("os.arch").lowercase()
        val arch = archOriginal.lowercase()

        return when {
            osName.startsWith("mac") -> {
                when (arch) {
                    "aarch64" -> MacArm64Path
                    else -> error("Arch is not supported: $archOriginal")
                }
            }

            osName.startsWith("linux") -> {
                when (arch) {
                    "amd64", "x86_64" -> Linux64Path
                    else -> error("Arch is not supported: $archOriginal")
                }
            }

            else -> error("OS is not supported: $osNameOriginal")
        }
    }
}
