// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Stellar/Address.h"
#include "proto/Stellar.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWStellarPassphrase.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Stellar;

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
