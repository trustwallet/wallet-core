package com.trustwallet.core.app.blockchains.harmony

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Harmony
import wallet.core.jni.proto.Harmony.SigningOutput
import wallet.core.jni.CoinType.HARMONY

class TestHarmonyStakingDelegateSigner {

    val oneAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
    val privateKeyData = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
    val pubKeyData = ByteString.copyFrom("b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611".toHexByteArray())

    init {
        System.loadLibrary("TrustWalletCore")
    }

    fun createValidator(): Harmony.DirectiveCreateValidator.Builder {
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
            value = ByteString.copyFrom("0x1".toHexByteArray())
            precision = ByteString.copyFrom("0x1".toHexByteArray())
        }
        val mr = Harmony.Decimal.newBuilder()
        mr.apply {
            value = ByteString.copyFrom("0x9".toHexByteArray())
            precision = ByteString.copyFrom("0x1".toHexByteArray())
        }
        val mcr = Harmony.Decimal.newBuilder()
        mcr.apply {
            value = ByteString.copyFrom("0x5".toHexByteArray())
            precision = ByteString.copyFrom("0x2".toHexByteArray())
        }
        val cRate = Harmony.CommissionRate.newBuilder()
        cRate.apply {
            rate = r.build()
            maxRate = mr.build()
            maxChangeRate = mcr.build()
        }
        val pubKey = pubKeyData
        val createValidator = Harmony.DirectiveCreateValidator.newBuilder()
        createValidator.apply {
            validatorAddress = oneAddress
            description = desc.build()
            commissionRates = cRate.build()
            minSelfDelegation = ByteString.copyFrom("0xa".toHexByteArray())
            maxTotalDelegation = ByteString.copyFrom("0x0bb8".toHexByteArray())
            addAllSlotPubKeys(listOf(pubKey))
            amount = ByteString.copyFrom("0x64".toHexByteArray())
        }
        return createValidator
    }

    @Test
    fun testHarmonyStakingTransactionCreateValidatorSigning() {
        val createValidator = createValidator();
        val staking = Harmony.StakingMessage.newBuilder()
        staking.apply {
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
            createValidatorMessage = createValidator.build()
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = privateKeyData
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            stakingMessage = staking.build()
        }
        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())

        val expected = "0xf8ed80f8a494ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121ddc988016345785d8a0000c9880c7d713b49da0000c887b1a2bc2ec500000a820bb8f1b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b622476086116402806428a0476e8a0fe478e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28a03b2be18e73b2f99d7e2691485a0e166f28e62815079c126e68f876dc97339f8f"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x476e8a0fe478e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x3b2be18e73b2f99d7e2691485a0e166f28e62815079c126e68f876dc97339f8f")
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
            value = ByteString.copyFrom("0x1".toHexByteArray())
            precision = ByteString.copyFrom("0x1".toHexByteArray())
        }
        val editValidator = Harmony.DirectiveEditValidator.newBuilder()
        editValidator.apply {
            validatorAddress = oneAddress
            description = desc.build()
            commissionRate = rate.build()
            minSelfDelegation = ByteString.copyFrom("0xa".toHexByteArray())
            maxTotalDelegation = ByteString.copyFrom("0x0bb8".toHexByteArray())
            slotKeyToRemove = pubKeyData
            slotKeyToAdd = pubKeyData
        }
        val staking = Harmony.StakingMessage.newBuilder()
        staking.apply {
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
            editValidatorMessage = editValidator.build()
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = privateKeyData
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            stakingMessage = staking.build()
        }

        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())
        val expected = "0xf9010801f8bf94ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c988016345785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b6224760861102806427a05e54b55272f6bf5ffeca10d85976749d6b844cc9f30ba3285b9ab8a82d53e3e3a03ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x27")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x5e54b55272f6bf5ffeca10d85976749d6b844cc9f30ba3285b9ab8a82d53e3e3")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x3ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013")
    }

    @Test
    fun testHarmonyStakingTransactionDelegateSigning() {
        val delegate = Harmony.DirectiveDelegate.newBuilder()
        delegate.apply {
            delegatorAddress = oneAddress
            validatorAddress = oneAddress
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        val staking = Harmony.StakingMessage.newBuilder()
        staking.apply {
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
            delegateMessage = delegate.build()
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = privateKeyData
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            stakingMessage = staking.build()
        }
        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())
        val expected = "0xf87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }

    @Test
    fun testHarmonyStakingTransactionUndelegateSigning() {
        val undelegate = Harmony.DirectiveUndelegate.newBuilder()
        undelegate.apply {
            delegatorAddress = oneAddress
            validatorAddress = oneAddress
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        val staking = Harmony.StakingMessage.newBuilder()
        staking.apply {
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
            undelegateMessage = undelegate.build()
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = privateKeyData
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            stakingMessage = staking.build()
        }
        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())
        val expected = "0xf87303eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c580a02806428a05bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2a05202c4b51652d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x5bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x5202c4b51652d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7")
    }

    @Test
    fun testHarmonyStakingTransactionCollectRewardsSigning() {
        val cRewards = Harmony.DirectiveCollectRewards.newBuilder()
        cRewards.apply {
            delegatorAddress = oneAddress
        }
        val staking = Harmony.StakingMessage.newBuilder()
        staking.apply {
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
            collectRewards = cRewards.build()
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = privateKeyData
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            stakingMessage = staking.build()
        }

        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())
        val expected = "0xf85d04d594ebcd16e8c1d8f493ba04e99a56474122d81a9c5802806428a04c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625a055c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x4c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x55c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d")
    }
}
