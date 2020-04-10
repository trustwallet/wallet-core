package com.trustwallet.core.app.blockchains.IoTeX

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.IOTEX
import wallet.core.jni.proto.IoTeX
import wallet.core.jni.proto.IoTeX.SigningOutput

class TestIotexSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }
    
    @Test
    fun testIotexSigningCreate() {   
        val input = IoTeX.SigningInput.newBuilder()
            .setVersion(1)
            .setNonce(0)
            .setGasLimit(1000000)
            .setGasPrice("10")
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign Create
        val create = IoTeX.Staking.Create.newBuilder().apply {
            candidateName = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj"
            stakedAmount = "100"
            stakedDuration = 10000
            autoStake = true
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeCreate = create
        }

        val sign = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytes = sign.encoded
        assertEquals(signBytes.toByteArray().toHex(), "0x0a4b080118c0843d22023130c2023e0a29696f313964307033616834673877773964376b63786671383779786537666e7238727074683573686a120331303018904e20012a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a412e8bac421bab88dcd99c26ac8ffbf27f11ee57a41e7d2537891bfed5aed8e2e026d46e55d1b856787bc1cd7c1216a6e2534c5b5d1097c3afe8e657aa27cbbb0801")
    }
    fun testIotexSigningAddDeposit() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign AddDeposit
        val adddeposit = IoTeX.Staking.AddDeposit.newBuilder().apply {
            bucketIndex = 10
            amount = "10"
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeAddDeposit = adddeposit
        }

        var signAddDeposit = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        var signBytesAddDeposit = signAddDeposit.encoded
        assertEquals(signBytesAddDeposit.toByteArray().toHex(), "0x0a1c080118c0843d22023130da020f080a120231301a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41a48ab1feba8181d760de946aefed7d815a89fd9b1ab503d2392bb55e1bb75eec42dddc8bd642f89accc3a37b3cf15a103a95d66695fdf0647b202869fdd66bcb01")
    }
    fun testIotexSigningUnstake() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign Unstake
        val unstake = IoTeX.Staking.Reclaim.newBuilder().apply {
            bucketIndex = 10
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeUnstake = unstake
        }

        val signUnstake = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesUnstake = signUnstake.encoded
        assertEquals(signBytesUnstake.toByteArray().toHex(), "0x0a18080118c0843d22023130ca020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4100adee39b48e1d3dbbd65298a57c7889709fc4df39987130da306f6997374a184b7e7c232a42f21e89b06e6e7ceab81303c6b7483152d08d19ac829b22eb81e601")
    }
    fun testIotexSigningWithdraw() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign Withdraw
        val withdraw = IoTeX.Staking.Reclaim.newBuilder().apply {
            bucketIndex = 10
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeWithdraw = withdraw
        }

        val signWithdraw = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesWithdraw = signWithdraw.encoded
        assertEquals(signBytesWithdraw.toByteArray().toHex(), "0x0a18080118c0843d22023130d2020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4152644d102186be6640d46b517331f3402e24424b0d85129595421d28503d75340b2922f5a0d4f667bbd6f576d9816770286b2ce032ba22eaec3952e24da4756b00")
    }
    fun testIotexSigningRestake() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
         // test sign Restake
         val restake = IoTeX.Staking.Restake.newBuilder().apply {
            bucketIndex = 10
            stakedDuration = 1000
            autoStake = true
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeRestake = restake
        }

        val signRestake = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesRestake = signRestake.encoded
        assertEquals(signBytesRestake.toByteArray().toHex(), "0x0a1d080118c0843d22023130e20210080a10e807180122077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41e2e763aed5b1fd1a8601de0f0ae34eb05162e34b0389ae3418eedbf762f64959634a968313a6516dba3a97b34efba4753bbed3a33d409ecbd45ac75007cd8e9101")
    }
    fun testIotexSigningChangeCandidate() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign ChangeCandidate
        val changecandidate = IoTeX.Staking.ChangeCandidate.newBuilder().apply {
            bucketIndex = 10
            candidateName = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza"
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeChangeCandidate = changecandidate
        }

        val signChangeCandidate = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesChangeCandidate = signChangeCandidate.encoded
        assertEquals(signBytesChangeCandidate.toByteArray().toHex(), "0x0a43080118c0843d22023130ea0236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41d519eb3747163b945b862989b7e82a7f8468001e9683757cb88d5ddd95f81895047429e858bd48f7d59a88bfec92de231d216293aeba1e4fbe11461d9c9fc99801")
    }
    fun testIotexSigningTransferOwnership() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign TransferOwnership
        val transfer = IoTeX.Staking.TransferOwnership.newBuilder().apply {
            bucketIndex = 10
            voterAddress = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza"
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()
        
        input.apply {
            stakeTransferOwnership = transfer
        }

        val signTransferOwnership = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesTransferOwnership = signTransferOwnership.encoded
        assertEquals(signBytesTransferOwnership.toByteArray().toHex(), "0x0a43080118c0843d22023130f20236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41fa26db427ab87a56a129196c1604f2e22c4dd2a1f99b2217bc916260757d00093d9e6dccdf53e3b0b64e41a69d71c238fbf9281625164694a74dfbeba075d0ce01")
    }
    fun testIotexSigningCandidateBasicInfo() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("10")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign CandidateBasicInfo
        val cbi = IoTeX.Staking.CandidateBasicInfo.newBuilder().apply {
            name = "test"
            operatorAddress = "io1cl6rl2ev5dfa988qmgzg2x4hfazmp9vn2g66ng"
            rewardAddress = "io1juvx5g063eu4ts832nukp4vgcwk2gnc5cu9ayd"
            }.build()
        
        input.apply {
            candidateUpdate = cbi
        }

        val signCandidateBasicInfo = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesCandidateBasicInfo = signCandidateBasicInfo.encoded
        assertEquals(signBytesCandidateBasicInfo.toByteArray().toHex(), "0x0a69080118c0843d2202313082035c0a04746573741229696f31636c36726c32657635646661393838716d677a673278346866617a6d7039766e326736366e671a29696f316a757678356730363365753474733833326e756b7034766763776b32676e6335637539617964124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4101885c9c6684a4a8f2f5bf11f8326f27be48658f292e8f55ec8a11a604bb0c563a11ebf12d995ca1c152e00f8e0f0edf288db711aa10dbdfd5b7d73b4a28e1f701")
    }
    fun testIotexSigningCandidateRegister() {   
            val input = IoTeX.SigningInput.newBuilder()
                .setVersion(1)
                .setNonce(0)
                .setGasLimit(1000000)
                .setGasPrice("1000")
                .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")))
        // test sign CandidateBasicInfo
        val cbi = IoTeX.Staking.CandidateBasicInfo.newBuilder().apply {
            name = "test"
            operatorAddress = "io10a298zmzvrt4guq79a9f4x7qedj59y7ery84he"
            rewardAddress = "io13sj9mzpewn25ymheukte4v39hvjdtrfp00mlyv"
            }.build()
        val cr = IoTeX.Staking.CandidateRegister.newBuilder().apply {
            candidate = cbi
            stakedAmount = "100"
            stakedDuration = 10000
            autoStake = false
            ownerAddress ="io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj"
            payload = ByteString.copyFrom("payload".toByteArray())
            }.build()   
        input.apply {
            candidateRegister = cr
        }

        val signCandidateRegister = AnySigner.sign(input.build(), IOTEX, SigningOutput.parser())
        val signBytesCandidateRegister = signCandidateRegister.encoded
        assertEquals(signBytesCandidateRegister.toByteArray().toHex(), "0x0aaa01080118c0843d220431303030fa029a010a5c0a04746573741229696f3130613239387a6d7a7672743467757137396139663478377165646a35397937657279383468651a29696f3133736a396d7a7065776e3235796d6865756b74653476333968766a647472667030306d6c7976120331303018904e2a29696f313964307033616834673877773964376b63786671383779786537666e7238727074683573686a32077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a417819b5bcb635e3577acc8ca757f2c3d6afa451c2b6ff8a9179b141ac68e2c50305679e5d09d288da6f0fb52876a86c74deab6a5247edc6d371de5c2f121e159400")
    }
}
