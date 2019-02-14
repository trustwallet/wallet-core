package com.trustwallet.core.app.utils

import kotlin.experimental.and

// TODO: Move to native
object Numeric {

    fun containsHexPrefix(input: String): Boolean {
        return input.length > 1 && input[0] == '0' && input[1] == 'x'
    }

    fun cleanHexPrefix(input: String): String {
        return if (containsHexPrefix(input)) {
            input.substring(2)
        } else {
            input
        }
    }

    fun hexStringToByteArray(input: String): ByteArray {
        val cleanInput = cleanHexPrefix(input)

        val len = cleanInput.length

        if (len == 0) {
            return byteArrayOf()
        }

        val data: ByteArray
        val startIdx: Int
        if (len % 2 != 0) {
            data = ByteArray(len / 2 + 1)
            data[0] = Character.digit(cleanInput.get(0), 16).toByte()
            startIdx = 1
        } else {
            data = ByteArray(len / 2)
            startIdx = 0
        }

        var i = startIdx
        while (i < len) {
            data[(i + 1) / 2] =
                ((Character.digit(cleanInput.get(i), 16) shl 4) + Character.digit(cleanInput.get(i + 1), 16)).toByte()
            i += 2
        }
        return data
    }

    fun toHexString(input: ByteArray?, offset: Int, length: Int, withPrefix: Boolean): String {
        val stringBuilder = StringBuilder()
        if (withPrefix) {
            stringBuilder.append("0x")
        }
        for (i in offset until offset + length) {
            stringBuilder.append(String.format("%02x", input!![i] and 0xFF.toByte()))
        }

        return stringBuilder.toString()
    }

    fun toHexString(input: ByteArray?): String {
        return toHexString(input, 0, input!!.size, true)
    }
}