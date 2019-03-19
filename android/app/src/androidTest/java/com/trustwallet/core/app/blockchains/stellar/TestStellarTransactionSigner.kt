package com.trustwallet.core.app.blockchains.stellar

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.StellarPassphrase
import wallet.core.jni.StellarSigner
import wallet.core.jni.proto.Stellar

class TestStellarTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testStellarTransactionSigning() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            passphrase = StellarPassphrase.STELLAR.toString()
            operationType = Stellar.SigningInput.OperationType.PAYMENT
            memoVoid = memoVoidBuilder.build()
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==")
    }

    @Test
    fun testStellarTransactionSigningMemoHash() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            passphrase = StellarPassphrase.STELLAR.toString()
            operationType = Stellar.SigningInput.OperationType.PAYMENT
            memoHash = Stellar.MemoHash.newBuilder().setHash(ByteString.copyFrom("315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3".toHexByteArray())).build()
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAMxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAECIyh1BG+hER5W+dgHDKe49X6VEYRWIjajM4Ufq3DUG/yw7Xv1MMF4eax3U0TRi7Qwj2fio/DRD3+/Ljtvip2MD")
    }

    @Test
    fun testStellarTransactionSigningMemoReturn() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            passphrase = StellarPassphrase.STELLAR.toString()
            operationType = Stellar.SigningInput.OperationType.PAYMENT
            memoReturnHash = Stellar.MemoHash.newBuilder().setHash(ByteString.copyFrom("315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3".toHexByteArray())).build()
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAQxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBd77iui04quoaoWMfeJO06nRfn3Z9bptbAj7Ol44j3ApU8c9dJwVhJbQ7La4mKgIkYviEhGx3AIulFYCkokb8M")
    }

    @Test
    fun testStellarTransactionSigningMemoId() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            passphrase = StellarPassphrase.STELLAR.toString()
            operationType = Stellar.SigningInput.OperationType.PAYMENT
            memoId = Stellar.MemoId.newBuilder().setId(1234567890).build()
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEAOJ8wwCizQPf6JmkCsCNZolQeqet2qN7fgLUUQlwx3TNzM0+/GJ6Qc2faTybjKy111rE60IlnfaPeMl/nyxKIB")
    }

    @Test
    fun testStellarTransactionSigningCreateAccount() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            passphrase = StellarPassphrase.STELLAR.toString()
            operationType = Stellar.SigningInput.OperationType.CREATE_ACCOUNT
            memoId = Stellar.MemoId.newBuilder().setId(1234567890).build()
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAAAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAmJaAAAAAAAAAAAEZ31yiAAAAQNgqNDqbe0X60gyH+1xf2Tv2RndFiJmyfbrvVjsTfjZAVRrS2zE9hHlqPQKpZkGKEFka7+1ElOS+/m/1JDnauQg=")
    }
}