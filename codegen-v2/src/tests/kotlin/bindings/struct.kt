package com.trustwallet.core

actual object MainStruct {
    @JvmStatic
    @JvmName("firstFunction")
    actual external fun firstFunction(first_param: Int): Bool

    actual val firstProperty: Bool
        @JvmName("firstProperty")
        external get
}
