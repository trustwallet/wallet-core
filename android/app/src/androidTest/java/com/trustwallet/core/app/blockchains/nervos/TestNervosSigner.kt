// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.nervos

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.NERVOS
import wallet.core.jni.proto.Nervos.*
import wallet.core.java.AnySigner

class TestNervosSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigning() {
        val key = PrivateKey("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb".toHexByteArray())

        val lockScript = Script.newBuilder().apply {
            codeHash = "9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8".toHexBytesInByteString()
            hashType = "type"
            args = "c4b50c5c8d074f063ec0a77ded0eaff0fa7b65da".toHexBytesInByteString()
        }.build()

        val signingInput = SigningInput.newBuilder().apply {
            addPrivateKey(ByteString.copyFrom(key.data()))
            byteFee = 1
            nativeTransfer = NativeTransfer.newBuilder().apply {
                toAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqdtyq04tvp02wectaumxn0664yw2jd53lqk4mxg3"
                changeAddress = "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqds6ed78yze6eyfyvd537z66ur22c9mmrgz82ama"
                amount = 10000000000
            }.build()
            addAllCell(listOf(
                Cell.newBuilder().apply {
                    capacity = 100000000000
                    outPoint = OutPoint.newBuilder().apply {
                        txHash = "71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3".toHexBytesInByteString()
                        index = 1
                    }.build()
                    lock = lockScript
                }.build(),
                Cell.newBuilder().apply {
                    capacity = 20000000000
                    outPoint = OutPoint.newBuilder().apply {
                        txHash = "71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3".toHexBytesInByteString()
                        index = 0
                    }.build()
                    lock = lockScript
                }.build()
            ))
        }.build()

        val output = AnySigner.sign(signingInput, NERVOS, SigningOutput.parser())

        assertEquals(output.transactionJson, "{\"cell_deps\":[{\"dep_type\":\"dep_group\",\"out_point\":{\"index\":\"0x0\",\"tx_hash\":\"0x71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c\"}}],\"header_deps\":[],\"inputs\":[{\"previous_output\":{\"index\":\"0x0\",\"tx_hash\":\"0x71caea2d3ac9e3ea899643e3e67dd11eb587e7fe0d8c6e67255d0959fa0a1fa3\"},\"since\":\"0x0\"}],\"outputs\":[{\"capacity\":\"0x2540be400\",\"lock\":{\"args\":\"0xab201f55b02f53b385f79b34dfad548e549b48fc\",\"code_hash\":\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":\"type\"},\"type\":null},{\"capacity\":\"0x2540be230\",\"lock\":{\"args\":\"0xb0d65be39059d6489231b48f85ad706a560bbd8d\",\"code_hash\":\"0x9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8\",\"hash_type\":\"type\"},\"type\":null}],\"outputs_data\":[\"0x\",\"0x\"],\"version\":\"0x0\",\"witnesses\":[\"0x55000000100000005500000055000000410000002a9ef2ad7829e5ea0c7a32735d29a0cb2ec20434f6fd5bf6e29cda56b28e08140156191cbbf80313d3c9cae4b74607acce7b28eb21d52ef058ed8491cdde70b700\"]}")
        assertEquals(output.transactionId, "0xf2c32afde7e72011985583873bc16c0a3c01fc01fc161eb4b914fcf84c53cdf8")
    }
}
