package com.trustwallet.core.app.blockchains.iost

import com.google.gson.Gson
import com.google.protobuf.ByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.IOSTSigner
import wallet.core.jni.proto.IOST.AccountInfo.newBuilder as accountBuilder
import wallet.core.jni.proto.IOST.Action.newBuilder as actionBuilder
import wallet.core.jni.proto.IOST.AmountLimit.newBuilder as amountLimitBuilder
import wallet.core.jni.proto.IOST.SigningInput.newBuilder as signInputBuilder
import wallet.core.jni.proto.IOST.Transaction.newBuilder as transactionBuilder

class TestIOSTSigner {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testIOSTSignerSign() {
        var secKeyBytes = hexStringToByteArray("63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4")
        var accountBuilder = accountBuilder()
        accountBuilder.name = "myname"
        accountBuilder.activeKey = ByteString.copyFrom(secKeyBytes)

        var transactionBuilder = transactionBuilder()
        transactionBuilder.time = 1550137587000000000
        transactionBuilder.expiration = transactionBuilder.time + 1000000000L * 300
        transactionBuilder.gasRatio = 1.0
        transactionBuilder.gasLimit = 1000000.0
        transactionBuilder.chainId = 1024

        var amountLimitBuilder = amountLimitBuilder()
        amountLimitBuilder.token = "*"
        amountLimitBuilder.value = "unlimited"
        transactionBuilder.addAmountLimit(amountLimitBuilder.build())


        var signInputBuilder = signInputBuilder()
        signInputBuilder.transactionTemplate = transactionBuilder.build()
        signInputBuilder.account = accountBuilder.build()
        signInputBuilder.transferAmount = "10"
        signInputBuilder.transferDestination = "admin"
        signInputBuilder.transferMemo = ""

        var outPut = IOSTSigner.sign(signInputBuilder.build())


        assertEquals(bytesToHex(outPut.transaction.getPublisherSigs(0).signature.toByteArray()),
            "e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03")
    }

    private fun bytesToHex(hashInBytes: ByteArray): String {

        val sb = StringBuilder()
        for (b in hashInBytes) {
            sb.append(String.format("%02x", b))
        }
        return sb.toString()
    }

    private fun hexStringToByteArray(s: String): ByteArray {
        val len = s.length
        val data = ByteArray(len / 2)

        var i = 0
        while (i < len) {
            data[i / 2] = ((Character.digit(s[i], 16) shl 4) + Character.digit(s[i + 1], 16)).toByte()
            i += 2
        }

        return data
    }
}