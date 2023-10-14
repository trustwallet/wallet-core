// Copyright ? 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.neo

import com.trustwallet.core.app.utils.Numeric
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.proto.NEO
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import wallet.core.jni.CoinType

class TestNEOSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    fun addInput(builder: NEO.SigningInput.Builder, hash: String, index: Int, assetID: String, value: Long) {
        builder.addInputs(NEO.TransactionInput.newBuilder()
                .setPrevHash(hash.toHexBytesInByteString())
                .setPrevIndex(index)
                .setAssetId(assetID)
                .setValue(value)
                .build())
    }

    fun prepareInputs(builder: NEO.SigningInput.Builder) {
        val NEO_ASSET_ID = "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"
        val GAS_ASSET_ID = "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60"

        addInput(builder, "c61508268c5d0343af1875c60e569493100824dbdba108b31789e0e33bcb50fb",  1, GAS_ASSET_ID,  98899890000)
        addInput(builder, "4eb2f96937a0d4dc96b77ba69a29e1de9574cbd62b16d881f1ee2061a291d70b",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "3fee0109d155dcfab272176117306b45b176914c88e8c379933c246a9e29ea0b",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "6ea9ce8c578bfeeecdf281f498e2a764689df3b93d6855a3cc45bd6b5213c426",  0, NEO_ASSET_ID,  400000000)
        addInput(builder, "f75ad3cbd277d83ee240e08f99a97ffd7e42a82a868e0f7043414f6d6147262b",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "054734e98f442b3e73a940ca8f594859ece1c7ddac14130b0e2f5e2799b85931",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "8b0c42d448912fc28c674fdcf8e21e4667d7d2133666168eaa0570488a9c5036",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  1, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  2, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  3, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  4, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  5, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  6, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  7, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  8, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  9, NEO_ASSET_ID,  500000000)
        addInput(builder, "cf83bce600626b6077e136581c1aecc78a0bbb7d7649b1f580b6be881087ec40",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "9bd7572ba8df685e262369897d24f7217b42be496b9eed16e16a889dd83b394e",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "b4ee250397dde2f1001d782d3c803c38992447d3b351cdc9bf20cfaa2cbf995b",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "e1019ca259a1615f77263324156a70007b76cb4f26b01b2956b8f85e6842ac62",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "bd379df2aca526ac600919aaba0e59d4a1ad4e2f22d18966063cf45e431d016f",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "164c3f843b9b7bfa6a7376a1548f343acb5cdfa0193b8f31e8c9a647ea63ea7d",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "4acec74a76161eafe70e0791b1f504b5ba1d175fd4f340d5bf56804e25505e92",  0, NEO_ASSET_ID,  300000000)
        addInput(builder, "895c6629a71c84cbdc8956abea9ca2d9d215e909e6173b1a1a96289186a67796",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "54828143c4c3a0e1b09102e4ed29220b141089c2bc4200b1042eeb12e5e49296",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "5345e4abc86f7ace47112f5a91c129175833bafcaf9f1e1bcbbaf4d019c1c69d",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "c83e19d0d4210df97b3bc7768dc7184ae3acfc1b5b3ac9b05d2be0fe5a636b9f",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "3456b03f5cb688ce26ab1d09b7a15799136c8c886ca7c3c6bcb2363e61bb1bb1",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "048f73d6cc82d9d92b08044eccef66c78a0c22e836988ed25d6f7ffe24fb5b38",  10, NEO_ASSET_ID,  34000000000)
        addInput(builder, "e5a7887521b8b3aaf2d5426617ddabe8ef8ea3eab31c80a977c3b8f339df5be0",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "1455e9dd3cd6a04d81cd47acc07a7335212029ebbdcd0abc3e52c33f8b77f6eb",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "da711260085211b5573801d0dfe064235c69e61a55f9c15449ac55cc02b9adee",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "04486cfed371103dd51a89205b2c8bcc45ad887c49a768a62465f35810437bef",  0, NEO_ASSET_ID,  500000000)
        addInput(builder, "a5f27055a442db0e65103561900456d37af4233267960daded870c1ab2219ef4",  0, NEO_ASSET_ID,  500000000)
    }

    @Test
    fun NEOTransactionSigning() {
        val NEO_ASSET_ID = "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"
        val GAS_ASSET_ID = "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60"

        val input = NEO.SigningInput.newBuilder().apply {
            privateKey = "F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128".toHexBytesInByteString()
            fee = 12345
            gasAssetId = GAS_ASSET_ID
            gasChangeAddress = "AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV"
        }
        prepareInputs(input)
        val output = NEO.TransactionOutput.newBuilder()
                .setAssetId(NEO_ASSET_ID)
                .setAmount(25000000000)
                .setToAddress("Ad9A1xPbuA5YBFr1XPznDwBwQzdckAjCev")
                .setChangeAddress("AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV")
                .build()
        input.addOutputs(output)

        val plan = AnySigner.plan(input.build(), CoinType.NEO,NEO.TransactionPlan.parser())
        input.setPlan(plan)
        val result = AnySigner.sign(input.build(), CoinType.NEO, NEO.SigningOutput.parser()).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        //  https://testnet-explorer.o3.network/transactions/0x7b138c753c24f474d0f70af30a9d79756e0ee9c1f38c12ed07fbdf6fc5132eaf
        assertEquals(
                "8000001efb50cb3be3e08917b308a1dbdb2408109394560ec67518af43035d8c260815c601000bd791a26120eef181d8162bd6cb7495dee1299aa67bb796dcd4a03769f9b24e00000bea299e6a243c9379c3e8884c9176b1456b3017611772b2fadc55d10901ee3f000026c413526bbd45cca355683db9f39d6864a7e298f481f2cdeefe8b578ccea96e00002b2647616d4f4143700f8e862aa8427efd7fa9998fe040e23ed877d2cbd35af700003159b899275e2f0e0b1314acddc7e1ec5948598fca40a9733e2b448fe9344705000036509c8a487005aa8e16663613d2d767461ee2f8dc4f678cc22f9148d4420c8b0000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040100385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040200385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040300385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040400385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040500385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040600385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040700385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040800385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f04090040ec871088beb680f5b149767dbb0b8ac7ec1a1c5836e177606b6200e6bc83cf00004e393bd89d886ae116ed9e6b49be427b21f7247d896923265e68dfa82b57d79b00005b99bf2caacf20bfc9cd51b3d3472499383c803c2d781d00f1e2dd970325eeb4000062ac42685ef8b856291bb0264fcb767b00706a15243326775f61a159a29c01e100006f011d435ef43c066689d1222f4eada1d4590ebaaa190960ac26a5acf29d37bd00007dea63ea47a6c9e8318f3b19a0df5ccb3a348f54a176736afa7b9b3b843f4c160000925e50254e8056bfd540f3d45f171dbab504f5b191070ee7af1e16764ac7ce4a00009677a6869128961a1a3b17e609e915d2d9a29ceaab5689dccb841ca729665c8900009692e4e512eb2e04b10042bcc28910140b2229ede40291b0e1a0c3c44381825400009dc6c119d0f4bacb1b1e9faffcba33581729c1915a2f1147ce7a6fc8abe4455300009f6b635afee02b5db0c93a5b1bfcace34a18c78d76c73b7bf90d21d4d0193ec80000b11bbb613e36b2bcc6c3a76c888c6c139957a1b7091dab26ce88b65c3fb056340000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040a00039b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500ba1dd205000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc50083064905000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ace72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c605013cf0617000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac014140dc261ac093a87640441bf0c3ad4a55ec727932b9175f600618bb5275f31aacf122956bc88746dc666759a2d67f120fe3ce1659f916d22a91e0b02421d3bddbd1232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac",
                hex)

        // TODO uncomment when nist256p1 Rust implementation is enabled.
        // assertEquals(
        //         "8000001efb50cb3be3e08917b308a1dbdb2408109394560ec67518af43035d8c260815c601000bd791a26120eef181d8162bd6cb7495dee1299aa67bb796dcd4a03769f9b24e00000bea299e6a243c9379c3e8884c9176b1456b3017611772b2fadc55d10901ee3f000026c413526bbd45cca355683db9f39d6864a7e298f481f2cdeefe8b578ccea96e00002b2647616d4f4143700f8e862aa8427efd7fa9998fe040e23ed877d2cbd35af700003159b899275e2f0e0b1314acddc7e1ec5948598fca40a9733e2b448fe9344705000036509c8a487005aa8e16663613d2d767461ee2f8dc4f678cc22f9148d4420c8b0000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040100385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040200385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040300385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040400385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040500385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040600385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040700385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040800385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f04090040ec871088beb680f5b149767dbb0b8ac7ec1a1c5836e177606b6200e6bc83cf00004e393bd89d886ae116ed9e6b49be427b21f7247d896923265e68dfa82b57d79b00005b99bf2caacf20bfc9cd51b3d3472499383c803c2d781d00f1e2dd970325eeb4000062ac42685ef8b856291bb0264fcb767b00706a15243326775f61a159a29c01e100006f011d435ef43c066689d1222f4eada1d4590ebaaa190960ac26a5acf29d37bd00007dea63ea47a6c9e8318f3b19a0df5ccb3a348f54a176736afa7b9b3b843f4c160000925e50254e8056bfd540f3d45f171dbab504f5b191070ee7af1e16764ac7ce4a00009677a6869128961a1a3b17e609e915d2d9a29ceaab5689dccb841ca729665c8900009692e4e512eb2e04b10042bcc28910140b2229ede40291b0e1a0c3c44381825400009dc6c119d0f4bacb1b1e9faffcba33581729c1915a2f1147ce7a6fc8abe4455300009f6b635afee02b5db0c93a5b1bfcace34a18c78d76c73b7bf90d21d4d0193ec80000b11bbb613e36b2bcc6c3a76c888c6c139957a1b7091dab26ce88b65c3fb056340000385bfb24fe7f6f5dd28e9836e8220c8ac766efcc4e04082bd9d982ccd6738f040a00039b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500ba1dd205000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc50083064905000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ace72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c605013cf0617000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac014140dc261ac093a87640441bf0c3ad4a55ec727932b9175f600618bb5275f31aacf1dd6a943678b9239a98a65d2980edf01beed0a0b4904573f31309a6a128a54980232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac",
        //         hex)
    }
}
