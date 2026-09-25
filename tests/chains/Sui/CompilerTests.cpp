// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Sui.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include <gtest/gtest.h>

namespace TW::Sui::tests {

TEST(SuiCompiler, PreHashAndCompile) {
    // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
    auto expectedData = parse_hex("000000000002000810270000000000000020259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b50150202000101000001010300000000010100d575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d500106f2c2c8c1d8964df1019d6616e9705719bebabd931da2755cb948ceb7e68964ec020000000000002060456ec667f5cd10467680ebf950ed329205175dacd946bb236aeed57c8617cfd575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d500100000000000000d00700000000000000");
    auto expectedHash = TW::Hash::blake2b(expectedData, 32);

    Proto::SigningInput input;
    auto txMsg = "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(txMsg);

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    auto preImageHashesData = TransactionCompiler::preImageHashes(TWCoinTypeSui, inputStrData);
    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    preSigningOutput.ParseFromArray(preImageHashesData.data(), static_cast<int>(preImageHashesData.size()));

    EXPECT_EQ(data(preSigningOutput.data()), expectedData);
    EXPECT_EQ(data(preSigningOutput.data_hash()), expectedHash);

    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    auto signature = privateKey.sign(expectedHash, TWCurveED25519);

    auto outputData = TransactionCompiler::compileWithSignatures(TWCoinTypeSui, inputStrData, {signature}, {publicKey});
    Proto::SigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    EXPECT_EQ(output.error(), Common::Proto::OK);
    EXPECT_EQ(output.signature(), "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

}
