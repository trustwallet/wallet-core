package com.trustwallet.core

actual class MainStruct private constructor (
    private val nativeHandle: Long,
) {
    init {
        if (nativeHandle == 0L) throw IllegalArgumentException()
    }

    actual constructor(string: String) : this(create(string)))

    actual companion object {
        @JvmStatic
        @JvmName("createFromNative")
        private fun createFromNative(nativeHandle: Long) = MainStruct(nativeHandle)

        @JvmStatic
        @JvmName("firstFunction")
        actual external fun firstFunction(first_param: Int): Bool
    }

    actual val firstProperty: Bool
        @JvmName("firstProperty")
        external get
}
