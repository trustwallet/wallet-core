package com.trustwallet.core.app.blockchains.algorand

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.*
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.Base64
import wallet.core.jni.CoinType.ALGORAND
import wallet.core.jni.proto.Algorand
import wallet.core.jni.proto.Algorand.SigningOutput

class TestAlgorandSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun algorandTransactionSigningNFTTransfer() {
        // Successfully broadcasted: https://algoexplorer.io/tx/FFLUH4QKZHG744RIQ2AZNWZUSIIH262KZ4MEWSY4RXMWN5NMOOJA
        val transaction = Algorand.AssetTransfer.newBuilder()
            .setToAddress("362T7CSXNLIOBX6J3H2SCPS4LPYFNV6DDWE6G64ZEUJ6SY5OJIR6SB5CVE")
            .setAmount(1)
            .setAssetId(989643841)
            .build()
        val signingInput = Algorand.SigningInput.newBuilder()
            .setGenesisId("mainnet-v1.0")
            .setGenesisHash(ByteString.copyFrom(Base64.decode("wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=")))
            .setNote(ByteString.copyFrom(Base64.decode("VFdUIFRPIFRIRSBNT09O")))
            .setPrivateKey("dc6051ffc7b3ec601bde432f6dea34d40fe3855e4181afa0f0524c42194a6da7".toHexBytesInByteString())
            .setFirstRound(27963950)
            .setLastRound(27964950)
            .setFee(1000)
            .setAssetTransfer(transaction)
            .build()

        val output = AnySigner.sign(signingInput, ALGORAND, SigningOutput.parser())

        assertEquals(
            output.signature,
            "nXQsDH1ilG3DIo2VQm5tdYKXe9o599ygdqikmROpZiNXAvQeK3avJqgjM5o+iByCdq6uOxlbveDyVmL9nZxxBg=="
        )
    }

    @Test
    fun AlgorandTransactionSigning() {
        val transaction = Algorand.Transfer.newBuilder()
            .setToAddress("CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4")
            .setAmount(1000000000000)
            .build()
        val signingInput = Algorand.SigningInput.newBuilder()
            .setGenesisId("mainnet-v1.0")
            .setGenesisHash(ByteString.copyFrom("c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adf".toHexByteArray()))
            .setNote(ByteString.copyFrom("68656c6c6f".toHexByteArray()))
            .setPrivateKey("d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b".toHexBytesInByteString())
            .setFirstRound(1937767)
            .setLastRound(1938767)
            .setFee(263000)
            .setTransfer(transaction)
            .build()

        val output = AnySigner.sign(signingInput, ALGORAND, SigningOutput.parser())

        assertEquals(
            Numeric.toHexString(output.encoded.toByteArray()),
            "0x82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179"
        )
    }

    @Test
    fun testSignJSON() {
        assertTrue(AnySigner.supportsJSON(ALGORAND.value()))

        val json = """
            {"genesisId":"mainnet-v1.0","genesisHash":"wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=","note":"aGVsbG8=","firstRound":"1937767","lastRound":"1938767","fee":"263000","transfer":{"toAddress":"CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4","amount":"1000000000000"}}
        """
        val key = "d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, ALGORAND.value())

        assertEquals("82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179", result)
    }

}
