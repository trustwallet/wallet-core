package com.trustwallet.core

actual enum class MainEnum(
    @get:JvmName("value")
    val value: UInt,
) {
    one(0, "one_string"),
    two(1),
    three(2, "three_string"),
    ;

    @JvmStatic
    @JvmName("firstFunction")
    actual external fun firstFunction(first_param: Int): Bool

    @JvmStatic
    @JvmName("secondFunction")
    actual external fun secondFunction(first_param: SomeStruct): Bool

    companion object {
        @JvmStatic
        @JvmName("createFromValue")
        fun fromValue(value: UInt): MainEnum? =
            values().firstOrNull { it.value == value }
    }
}
