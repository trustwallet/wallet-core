package com.trustwallet.core

actual enum class MainEnum(
    @get:JvmName("value")
    val value: UInt,
) {
    one(0, "one_string"),
    two(1),
    three(2, "three_string"),
    ;

    companion object {
        @JvmStatic
        @JvmName("createFromValue")
        fun fromValue(value: UInt): MainEnum? =
            values().firstOrNull { it.value == value }
    }
}
