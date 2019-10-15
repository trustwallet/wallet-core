package com.trustwallet.core.app.blockchains.nuls

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.NULSSigner
import wallet.core.jni.proto.Algorand

class TestNULSSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NULSTransactionSigning() {
 
        val signingInput = NULS.SigningInput.newBuilder()
            .set_private_key("9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b")
            .set_from_address("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H")
            .to_address("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe")
            .to_address("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe"))
            .set_amount("")
            .set_chain_id(1)
            .idassets_id(1)
            .set_nonce("0000000000000000")
            .set_remark("")
            .set_balance()
            .set_timestamp(0x5d8885f8)
            .build()

        val signer: NULSSigner(input);
        val output = signer.sign()

        assertEquals(
            Numeric.toHexString(output.encoded.toByteArray()), 
            "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a0000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120100010080969800000000000000000000000000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1d7e3463044022028019c0099e2233c7adb84bb03a9a5666ece4a5b65a026a090fa460f3679654702204df0fcb8762b5944b3aba033fa1a287ccb098150035dd8b66f52dc58d3d0843a"
        )
    }
}
