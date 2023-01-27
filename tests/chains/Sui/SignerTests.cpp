// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Sui/Signer.h"
#include "Sui/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

namespace TW::Sui::tests {

TEST(SuiSigner, Transfer) {
    // Successfully broadcasted https://explorer.sui.io/transaction/rxLgxcAqgMg8gphp6eCsSGQcdZnwFYx2SRdwEhnAUC4
    Proto::SigningInput input;
    auto txMsg = "AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(txMsg);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), "AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA");
    ASSERT_EQ(result.signature(), "AIYRmHDpQesfAx3iWBCMwInf3MZ56ZQGnPWNtECFjcSq0ssAgjRW6GLnFCX24tfDNjSm9gjYgoLmn1No15iFJAtqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

TEST(SuiSigner, TransferNFT) {
    // Successfully broadcasted https://explorer.sui.io/transaction/EmnhP9swuoijxYwHMywnXDGCXfFs1QxErsYoyWy9Y15J
    Proto::SigningInput input;
    std::string unsigned_tx = R"(AAAv0f6HrJCZ/1cuDVuxh1BL12XMeHxkKeZ7Js9grhcB0u8xtTvoOepOHAAAAAAAAAAgJvcpOSvKhM+tHPgGAnp5Pmc8l3wjZhVxK4/BrLu4YAgttQCskZzd41GsNuNxHYMsbbl2aSEnoKw8oAGf/LobCM7RxGurtPZtHAAAAAAAAAAgwk74iUAH9S+cGVXQxAydItvltZ3UK2L0vg1TYgDMPfABAAAAAAAAAOgDAAAAAAAA)";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(unsigned_tx);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), unsigned_tx);
    ASSERT_EQ(result.signature(), "AIjbyuyg9YX0f8/DXB5XZBnUCOqhUrPDbU9/E/FlzwGtDS57cOL/gZwN3vTV1KiOuN0cr0kxypgJpVLKlhd8hgdqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

TEST(SuiSigner, MoveCall) {
    // Successfully broadcasted on: https://explorer.sui.io/transaction/3Gg8AcEfokDnA8m7W58ANmeCr8vkSaPWjXMp9sLMScTj
    Proto::SigningInput input;
    std::string unsigned_tx = R"(AAIAAAAAAAAAAAAAAAAAAAAAAAAAAgEAAAAAAAAAINaXMihjlCd4CQVFRPjcNb7QfYP4wGgQyl1xbplvEKUCA3N1aQh0cmFuc2ZlcgACAQCdB6Mav5rHiXD0rAWTCxS+ENwxMBsAAAAAAAAAINqDfrJUZebPjUi7xcyR3QcQSA9tOLwxThgYaZ1vMfgfABQU2gJ3ToaOYd1F/R6mXryOZdvpRi21AKyRnN3jUaw243EdgyxtuXZppM+mSjYYEQWDcV/7hFRrAE0VtRwbAAAAAAAAACC5nJxYaYJfa9rfbxSikaEFVmHGuXyCIZoZbMpxMwLebAEAAAAAAAAA0AcAAAAAAAA=)";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(unsigned_tx);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), unsigned_tx);
    ASSERT_EQ(result.signature(), "AE8394w/+KOodhLjnKgu21iW0xZur6MA4ajPh31f2xaOI7vs6JHLAHLk5ED3bfJfc5ZehmC6D4DMyrH4F0dA3A1qfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

TEST(SuiSigner, AddDelegation) {
    // Successfully broadcasted on: https://explorer.sui.io/transaction/3Gg8AcEfokDnA8m7W58ANmeCr8vkSaPWjXMp9sLMScTj
    Proto::SigningInput input;
    std::string unsigned_tx = R"(AAIAAAAAAAAAAAAAAAAAAAAAAAAAAgEAAAAAAAAAIEt/p6rXSTjdKP6wJOXyx0c2xsgJ4MJtfxe7qHC34u4UCnN1aV9zeXN0ZW0fcmVxdWVzdF9hZGRfZGVsZWdhdGlvbl9tdWxfY29pbgAEAQEAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAgIAGSkMV9AFc419O9dL1kez9tzVIOiXzAEAAAAAACAfIePlHHP/+iv++FWQW9ofkVm4S2sFwupGikSq8bNjYwAH1A26NKDn7pJfn9zWaDi1nbntMJfMAQAAAAAAIKfMZAZktdmw36jwg/jcK1TDmrHmSZ/fdkeInO3BSjfWAAkB0AcAAAAAAAAAFAej1I8mhjmcpQTQtiX2J2HZ7y2xLbUArJGc3eNRrDbjcR2DLG25dmlY2RBfTU/P+GL1qpxE8NQrwiLw/JfMAQAAAAAAICs2NJlowCmCnpJ+hja2VwZE5K6yGM/qw0MSRnn9tW2cbgAAAAAAAACghgEAAAAAAA==)";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(unsigned_tx);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), unsigned_tx);
    ASSERT_EQ(result.signature(), "AKSbUoc+F4JGG9i+A8yVYyzcD8BXNV88iSaWpoS5KXUG7ao2pxjyvfUJEYyhWXTxgQazNDnIM1xGhD7zu1GU1wRqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

} // namespace TW::Sui::tests
