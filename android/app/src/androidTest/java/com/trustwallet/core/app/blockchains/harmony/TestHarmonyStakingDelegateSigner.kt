package com.trustwallet.core.app.blockchains.harmony

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.HarmonyStakingSigner
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Harmony

class TestHarmonyStakingDelegateSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testHarmonyStakingTransactionNewValidatorSigning() {
        val desc = Harmony.Description.newBuilder()
        desc.apply {
            name = "Alice"
            identity = "alice"
            website = "alice.harmony.one"
            securityContact = "Bob"
            details = "Don't mess with me!!!"
        }
        val r = Harmony.Decimal.newBuilder()
        r.apply {
            value = ByteString.copyFrom("0x56bc75e2d63100000".toHexByteArray())
        }
        val mr = Harmony.Decimal.newBuilder()
        mr.apply {
            value = ByteString.copyFrom("0x821ab0d4414980000".toHexByteArray())
        }
        val mcr = Harmony.Decimal.newBuilder()
        mcr.apply {
            value = ByteString.copyFrom("0x4563918244f40000".toHexByteArray())
        }
        val cRate = Harmony.CommissionRate.newBuilder()
        cRate.apply {
            rate = r.build()
            maxRate = mr.build()
            maxChangeRate = mcr.build()
        }
        val newValidator = Harmony.DirectiveNewValidator.newBuilder()
        newValidator.apply {
            description = desc.build()
            commission = cRate.build()
            minSelfDelegation = ByteString.copyFrom("0xa".toHexByteArray())
            stakingAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            pubKey = ByteString.copyFrom("b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611".toHexByteArray())
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            newValidatorMessage = newValidator.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf8ec80f8a3f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e"
        val e2 = "2774206d6573732077697468206d65212121e0ca89056bc75e2d63100000ca890821ab0d4414980000c9884563"
        val e3 = "918244f400000a94ebcd16e8c1d8f493ba04e99a56474122d81a9c58b0b9486167ab9087ab818dc4ce026edb5b"
        val e4 = "f216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b622476086116402806428a00b1a797d11f7b0"
        val e5 = "dad42abd66c542fab8af0f028b7159bb70e44fe68b2e4d9f2ca07b223662bdb4e1a084f8c506095886a1f5eda0"
        val e6 = "51927fab3516ab9258efc34cd7"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3 + e4 + e5 + e6)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x0b1a797d11f7b0dad42abd66c542fab8af0f028b7159bb70e44fe68b2e4d9f2c")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x7b223662bdb4e1a084f8c506095886a1f5eda051927fab3516ab9258efc34cd7")
    }

    @Test
    fun testHarmonyStakingTransactionEditValidatorSigning() {
        val desc = Harmony.Description.newBuilder()
        desc.apply {
            name = "Alice"
            identity = "alice"
            website = "alice.harmony.one"
            securityContact = "Bob"
            details = "Don't mess with me!!!"
        }
        val rate = Harmony.Decimal.newBuilder()
        rate.apply {
            value = ByteString.copyFrom("0x56bc75e2d63100000".toHexByteArray())
        }
        val editValidator = Harmony.DirectiveEditValidator.newBuilder()
        editValidator.apply {
            description = desc.build()
            stakingAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            commissionRate = rate.build()
            minSelfDelegation = ByteString.copyFrom("0xa".toHexByteArray())
        }
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            editValidatorMessage = editValidator.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf8a401f85bf83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e"
        val e2 = "2774206d6573732077697468206d6521212194ebcd16e8c1d8f493ba04e99a56474122d81a9c58ca89056bc75e"
        val e3 = "2d631000000a02806428a071b68b38864e75af60bf05e52b53278e864dbf2eb4a33adeacaa6e1b31f21e59a01e"
        val e4 = "e06acb4d2bc22105454a79ef089fc0794ddba6e2849d9e4236180b47e973ed"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3 + e4)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x71b68b38864e75af60bf05e52b53278e864dbf2eb4a33adeacaa6e1b31f21e59")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x1ee06acb4d2bc22105454a79ef089fc0794ddba6e2849d9e4236180b47e973ed")
    }

    @Test
    fun testHarmonyStakingTransactionDelegateSigning() {
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        val delegate = Harmony.DirectiveDelegate.newBuilder()
        delegate.apply {
            delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            delegateMessage = delegate.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        val e2 = "580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc417"
        val e3 = "63dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }

    @Test
    fun testHarmonyStakingTransactionRedelegateSigning() {
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        val redelegate = Harmony.DirectiveRedelegate.newBuilder()
        redelegate.apply {
            delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            validatorSrcAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            validatorDstAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            redelegateMessage = redelegate.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf88903f84094ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba0"
        val e2 = "4e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c580a02806428"
        val e3 = "a0c479121bf1ea02fa1052a4d54743703fa6eeb16e50ff002d34fcfde736c21d75a07a1b9"
        val e4 = "dac1761ab9fb38cadcdd4b0b28aafc39d1707e913f6b758e05e09b1e517"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3 + e4)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xc479121bf1ea02fa1052a4d54743703fa6eeb16e50ff002d34fcfde736c21d75")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x7a1b9dac1761ab9fb38cadcdd4b0b28aafc39d1707e913f6b758e05e09b1e517")
    }

    @Test
    fun testHarmonyStakingTransactionUndelegateSigning() {
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        val undelegate = Harmony.DirectiveUndelegate.newBuilder()
        undelegate.apply {
            delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            undelegateMessage = undelegate.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf87304eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a"
        val e2 = "9c580a02806427a0d6af2488d3b45658f37ff6bb89f7eaa86f7c1dfce19a68697e778be28efd2320a05b9837bd"
        val e3 = "5c7041318859f9fb444a255f32f4d7e7b49f18830ba75abecdc02390"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x27")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xd6af2488d3b45658f37ff6bb89f7eaa86f7c1dfce19a68697e778be28efd2320")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x5b9837bd5c7041318859f9fb444a255f32f4d7e7b49f18830ba75abecdc02390")
    }
}

