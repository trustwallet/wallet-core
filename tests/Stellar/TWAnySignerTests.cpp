// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "HexCoding.h"
#include "proto/Stellar.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWStellarPassphrase.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Stellar;

TEST(TWAnySingerStellar, Sign) {
    auto key = parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722");
    Proto::SigningInput input;
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(key.data(), key.size());
    input.set_operation_type(Proto::SigningInput_OperationType_PAYMENT);   
    auto& memoText = *input.mutable_memo_text();
    memoText.set_text("Hello, world!");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStellar);

    EXPECT_EQ(output.signature(), "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB");
}
