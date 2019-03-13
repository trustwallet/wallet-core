// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "Stellar/Address.h"
#include "Stellar/Signer.h"
#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWStellarMemoType.h>
#include <TrustWalletCore/TWHash.h>
#include "BinaryCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Stellar;

TEST(StellarTransaction, signWithMemoText) {
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    auto memoText = Proto::MemoText();
    memoText.set_text("Hello, world!");
    *input.mutable_memo_text() = memoText;
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB");
}

TEST(StellarTransaction, signWithMemoHash) {
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    auto memoHash = Proto::MemoHash();
    auto fromHex = parse_hex("315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");
    memoHash.set_hash(fromHex.data(), fromHex.size());
    *input.mutable_memo_hash() = memoHash;
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAMxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAECIyh1BG+hER5W+dgHDKe49X6VEYRWIjajM4Ufq3DUG/yw7Xv1MMF4eax3U0TRi7Qwj2fio/DRD3+/Ljtvip2MD");
}

TEST(StellarTransaction, signWithMemoReturn) {
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    auto memoHash = Proto::MemoHash();
    auto fromHex = parse_hex("315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3");
    memoHash.set_hash(fromHex.data(), fromHex.size());
    *input.mutable_memo_return_hash() = memoHash;
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAQxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBd77iui04quoaoWMfeJO06nRfn3Z9bptbAj7Ol44j3ApU8c9dJwVhJbQ7La4mKgIkYviEhGx3AIulFYCkokb8M");
}

TEST(StellarTransaction, signWithMemoID) {
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    auto memoId = Proto::MemoId();
    memoId.set_id(1234567890);
    *input.mutable_memo_id() = memoId;
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEAOJ8wwCizQPf6JmkCsCNZolQeqet2qN7fgLUUQlwx3TNzM0+/GJ6Qc2faTybjKy111rE60IlnfaPeMl/nyxKIB");
}