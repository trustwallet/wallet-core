// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Stellar/Address.h"
#include "proto/Stellar.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWStellarPassphrase.h>
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Stellar::tests {

TEST(TWAnySingerStellar, Sign_Payment) {
    auto key = parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722");
    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);
    input.set_private_key(key.data(), key.size());
    auto& memoText = *input.mutable_memo_text();
    memoText.set_text("Hello, world!");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    EXPECT_EQ(output.signature(), "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB");
}

TEST(TWAnySingerStellar, Sign_Payment_66b5) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(1000);
    input.set_sequence(144098454883270657);
    input.mutable_op_payment()->set_destination("GA3ISGYIE2ZTH3UAKEKBVHBPKUSL3LT4UQ6C5CUGP2IM5F467O267KI7");
    input.mutable_op_payment()->set_amount(1000000);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    // curl "https://horizon.stellar.org/transactions/66b5bca4b4293bdd85a6a559b08918482774b76bcc170b4533411f1d6422ce24"
    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAD6AH/8MgAAAABAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAANokbCCazM+6AURQanC9VJL2ufKQ8LoqGfpDOl577te8AAAAAAAAAAAAPQkAAAAAAAAAAAXfTkXUAAABAM9Nhzr8iWKzqnHknrxSVoa4b2qzbTzgyE2+WWxg6XHH50xiFfmvtRKVhzp0Jg8PfhatOb6KNheKRWEw4OvqEDw==");
}

TEST(TWAnySingerStellar, Sign_Payment_Asset_ea50) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(1000);
    input.set_sequence(144098454883270661);
    input.mutable_op_payment()->set_destination("GA3ISGYIE2ZTH3UAKEKBVHBPKUSL3LT4UQ6C5CUGP2IM5F467O267KI7");
    input.mutable_op_payment()->mutable_asset()->set_issuer("GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH");
    input.mutable_op_payment()->mutable_asset()->set_alphanum4("MOBI");
    input.mutable_op_payment()->set_amount(12000000);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    // curl "https://horizon.stellar.org/transactions/ea50884cd1288d2d5420065995d13d750d812258e0e79280c4033a434e625c99
    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAD6AH/8MgAAAAFAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAANokbCCazM+6AURQanC9VJL2ufKQ8LoqGfpDOl577te8AAAABTU9CSQAAAAA8cTArnmXa4wEQJxDHOw5SwBaDVjBfAP5lRMNZkRtlZAAAAAAAtxsAAAAAAAAAAAF305F1AAAAQEuWZZvKZuF6SMuSGIyfLqx5sn5O55+Kd489uP4g9jZH4UE7zZ4ME0+74I0BU8YDsYOmmxcfp/vdwTd+n3oGCQw=");
}

TEST(TWAnySingerStellar, Sign_Change_Trust_ad9c) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270659);
    input.mutable_op_change_trust()->mutable_asset()->set_issuer("GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH");
    input.mutable_op_change_trust()->mutable_asset()->set_alphanum4("MOBI");
    input.mutable_op_change_trust()->set_valid_before(1613336576);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    // curl "https://horizon.stellar.org/transactions/ad9cd0f3d636096b6502ccae07adbcf2cd3c0da5393fc2b07813dbe90ecc0d7b"
    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAADAAAAAQAAAAAAAAAAAAAAAGApkAAAAAAAAAAAAQAAAAAAAAAGAAAAAU1PQkkAAAAAPHEwK55l2uMBECcQxzsOUsAWg1YwXwD+ZUTDWZEbZWR//////////wAAAAAAAAABd9ORdQAAAEAnfyXyaNQX5Bq3AEQVBIaYd+cLib+y2sNY7DF/NYVSE51dZ6swGGElz094ObsPefmVmeRrkGsSc/fF5pmth+wJ");
}

TEST(TWAnySingerStellar, Sign_Change_Trust_2) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270659);
    input.mutable_op_change_trust()->mutable_asset()->set_issuer("GDUKMGUGDZQK6YHYA5Z6AY2G4XDSZPSZ3SW5UN3ARVMO6QSRDWP5YLEX");
    input.mutable_op_change_trust()->mutable_asset()->set_alphanum4("USD");
    input.mutable_op_change_trust()->set_valid_before(1613336576);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAADAAAAAQAAAAAAAAAAAAAAAGApkAAAAAAAAAAAAQAAAAAAAAAGAAAAAVVTRAAAAAAA6KYahh5gr2D4B3PgY0blxyy+Wdyt2jdgjVjvQlEdn9x//////////wAAAAAAAAABd9ORdQAAAEDMZtN05ZsZB4OKOZSFkQvuRqDIvMME3PYMTAGJPQlO6Ee0nOtaRn2q0uf0IhETSSfqcsK5asAZzNj07tG0SPwM");
}

TEST(TWAnySingerStellar, Sign_Create_Claimable_Balance_1f1f84) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270687);
    input.mutable_op_create_claimable_balance()->set_amount(90000000);
    input.mutable_op_create_claimable_balance()->add_claimants();
    input.mutable_op_create_claimable_balance()->mutable_claimants(0)->set_account("GC6CJDAY54D3O4RHEH33LUTBKDZGVOTR6NHBOTL4PIWI2CDKVRSZZJGJ");
    input.mutable_op_create_claimable_balance()->mutable_claimants(0)->set_predicate(Proto::Predicate_unconditional);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    // https://stellar.expert/explorer/public/tx/1f1f849ff2560901c91226f2fc866ef4ed1c67d672262c1f5829abe2348ac638
    // curl -X POST -F "tx=AAAAAMpF..Bg==" "https://horizon.stellar.org/transactions"
    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAAfAAAAAAAAAAAAAAABAAAAAAAAAA4AAAAAAAAAAAVdSoAAAAABAAAAAAAAAAC8JIwY7we3cich97XSYVDyarpx804XTXx6LI0IaqxlnAAAAAAAAAAAAAAAAXfTkXUAAABAgms/HPhEP/EYtVr5aWwhKJsn3pIVEZGFnTD2Xd/VPVsn8qogI7RYyjyBxSFPiLAljgGsPaUMfU3WFvyJCWNwBg==");
}

TEST(TWAnySingerStellar, Sign_Claim_Claimable_Balance_c1fb3c) {
    auto key = parse_hex("3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9");
    PrivateKey privKey = PrivateKey(key);
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519);
    Address addr = Address(pubKey);
    EXPECT_EQ(addr.string(), "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");

    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL");
    input.set_fee(10000);
    input.set_sequence(144098454883270689);
    const Data balanceIdHash = parse_hex("9c7b794b7b150f3e4c6dcfa260672bbe0c248b360129112e927e0f7ee2f9faf8");
    input.mutable_op_claim_claimable_balance()->set_balance_id(balanceIdHash.data(), balanceIdHash.size());
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    // https://stellar.expert/explorer/public/tx/c1fb3cf348aeb72bb2e1030c1d7f7f9c6c6d1bbab071b3e7c7c1cadafa795e8e
    // curl -X POST -F "tx=AAAAAMpF..DQ==" "https://horizon.stellar.org/transactions"
    EXPECT_EQ(output.signature(), "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAAhAAAAAAAAAAAAAAABAAAAAAAAAA8AAAAAnHt5S3sVDz5Mbc+iYGcrvgwkizYBKREukn4PfuL5+vgAAAAAAAAAAXfTkXUAAABAWL7dKkR1JuPZGFbDTRDgGBHW/vLPMWNRkAew+wPfGiCnZhpJJDcyX197EDDZMsJ7ungPUyhczRaeQOwZKx4DDQ==");

    { // negative test: hash wrong size
        const Data invalidBalanceIdHash = parse_hex("010203");
        input.mutable_op_claim_claimable_balance()->set_balance_id(invalidBalanceIdHash.data(), invalidBalanceIdHash.size());
        ANY_SIGN(input, TWCoinTypeStellar);
        EXPECT_EQ(output.signature(), "AAAAAXfTkXUAAABAFCywEfLs3q5Tv9eZCIcjhkJR0s8J4Us9G5YjVKUSaMoUz/AadC8dM2oQSLhpC5wjrNBi7hevg7jlkPx5/4AJCQ==");
    }
}

} // namespace TW::Stellar::tests
