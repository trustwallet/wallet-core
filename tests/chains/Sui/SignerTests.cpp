// Copyright © 2017-2022 Trust Wallet.
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
    input.set_unsigned_tx("AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA");
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), "AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA");
    ASSERT_EQ(result.encoded(), "AIYRmHDpQesfAx3iWBCMwInf3MZ56ZQGnPWNtECFjcSq0ssAgjRW6GLnFCX24tfDNjSm9gjYgoLmn1No15iFJAtqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

TEST(SuiSigner, TransferNFT) {
    // Successfully broadcasted https://explorer.sui.io/transaction/EmnhP9swuoijxYwHMywnXDGCXfFs1QxErsYoyWy9Y15J
    Proto::SigningInput input;
    std::string unsigned_tx = R"(AAAv0f6HrJCZ/1cuDVuxh1BL12XMeHxkKeZ7Js9grhcB0u8xtTvoOepOHAAAAAAAAAAgJvcpOSvKhM+tHPgGAnp5Pmc8l3wjZhVxK4/BrLu4YAgttQCskZzd41GsNuNxHYMsbbl2aSEnoKw8oAGf/LobCM7RxGurtPZtHAAAAAAAAAAgwk74iUAH9S+cGVXQxAydItvltZ3UK2L0vg1TYgDMPfABAAAAAAAAAOgDAAAAAAAA)";
    input.set_unsigned_tx(unsigned_tx);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(result.unsigned_tx(), unsigned_tx);
    ASSERT_EQ(result.encoded(), "AIjbyuyg9YX0f8/DXB5XZBnUCOqhUrPDbU9/E/FlzwGtDS57cOL/gZwN3vTV1KiOuN0cr0kxypgJpVLKlhd8hgdqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

} // namespace TW::Sui::tests
