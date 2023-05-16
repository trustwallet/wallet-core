package com.trustwallet.core

actual class MainStruct private constructor (
    private val nativeHandle: Long,
) {
    init {
        if (nativeHandle == 0L) throw IllegalArgumentException()
    }

    @Throws(IllegalArgumentException::class)
    actual constructor(string: String?) : this(create(string)))

    actual companion object {
        @JvmStatic
        @JvmName("createFromNative")
        private fun createFromNative(nativeHandle: Long) = MainStruct(nativeHandle)

        @JvmStatic
        @JvmName("withOptionalInt")
        actual external fun withOptionalInt(first_param: Int?): Bool?

        @JvmStatic
        @JvmName("withOptionalStruct")
        actual external fun withOptionalStruct(first_param: SomeStruct?): Bool?

        @JvmStatic
        @JvmName("withOptionalString")
        actual external fun withOptionalString(first_param: String?): Bool?

        @JvmStatic
        @JvmName("withOptionalEnum")
        actual external fun withOptionalEnum(first_param: SomeEnum?): Bool?
    }

    actual val withOptionalInt: Int?
        @JvmName("withOptionalInt")
        external get

    actual val withOptionalString: String?
        @JvmName("withOptionalString")
        external get

    actual val withOptionalStruct: SomeStruct?
        @JvmName("withOptionalStruct")
        external get

    actual val withOptionalEnum: SomeEnum?
        @JvmName("withOptionalEnum")
        external get
}
