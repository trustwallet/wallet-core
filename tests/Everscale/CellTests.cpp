// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Everscale/Cell.h"
#include "Everscale/Wallet.h"
#include "HexCoding.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Everscale;

static constexpr auto TX = "te6ccgECDgEAAyUAA7V6uRyM7ESqbjssMUQyAqYyQTlEkfDkEhWjBiC1fvKLabAAAaKsEuhQGeqOSyMlWG32ehDzoVCXMh6ugfMLr6pOPj3b6KD4DR/wAAGirA4jnSYtmdCAADR6V/lIBQQBAg8MQQYcxc1EQAMCAG/JkrcETDHn2AAAAAAAAgAAAAAAAop8XDVxQ98+QpgCzzW0U0opAulbEzfySLp3wLLoHzboQRA6FACdROMjE4gAAAAAAAAAACHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIACCcgkc5v5RyBDaeDdbY8Q+SpmX5OOkhzxFyB/ug4o/bJwJXDMKjAeOEr9OvfcJlgyx80ukSBl43/FO2DXIt1+SuAQCAeAJBgEB3wcBsWgBVyORnYiVTcdlhiiGQFTGSCcokj4cgkK0YMQWr95RbTcAIXxdqfc1KmavZDEIGsfjdBuS4lE5Ox4rJZ+Z+rauOxDRZaC8AAYx6CQAADRVgl0KBMWzOhDACAF7C04VCAAAAAAvMK5pAAAAAAAAAAAAAAAAAA7xIIAFdGVusOGq5cSrATb2hH5h5turvUDrer4E0Mf51wPlefAMAd2IAVcjkZ2IlU3HZYYohkBUxkgnKJI+HIJCtGDEFq/eUW02AMrbR14n5UXrp1deXDU5rl8kQvDfSKbnA+d09dtQe2mo8t94jbtllx/DjCgucIpvywjhJBhWNQjtWXh4dP6qiDAAAAADFszqDukuhIYKAQTQAwsB42IAQvi7U+5qVM1eyGIQNY/G6DclxKJydjxWSz8z9W1cdiGiy0F4AAAAAAAAAAAAAAAAAAALThUIAAAAAC8wrmkAAAAAAAAAAAAAAAAADvEggAV0ZW6w4arlxKsBNvaEfmHm26u9QOt6vgTQx/nXA+V58AwBY4AUk5qcKxU0Kqq8xI6zxcnOzaRMAW8AGxI8jfJSPgiVJ6AAAAAAAAAAAAAARVadlK9wDQBDgBVyORnYiVTcdlhiiGQFTGSCcokj4cgkK0YMQWr95RbTcA==";

TEST(EverscaleCell, DeserializeTransaction) {
    const auto boc = Base64::decode(TX);
    const auto root = Cell::deserialize(boc.data(), boc.size());
    ASSERT_EQ(hex(root->hash), "88a02e7bd8833d384f37d63d4d01deef9a1806937b94a313cc5e8c3cc7643032");
}

TEST(EverscaleCell, DeserializeWallet) {
    const auto cell = Cell::deserialize(Wallet::code, sizeof(Wallet::code));
    ASSERT_EQ(hex(cell->hash), "84dafa449f98a6987789ba232358072bc0f76dc4524002a5d0918b9a75d2d599");
}

TEST(EverscaleCell, SerializeTransaction) {
    const auto tx = Base64::decode(TX);
    const auto cell = Cell::deserialize(tx.data(), tx.size());
    Data data;
    cell->serialize(data);

    const auto decoded = Cell::deserialize(data.data(), data.size());
    ASSERT_EQ(hex(decoded->hash), "88a02e7bd8833d384f37d63d4d01deef9a1806937b94a313cc5e8c3cc7643032");
}

TEST(EverscaleCell, SerializeWallet) {
    const auto cell = Cell::deserialize(Wallet::code, sizeof(Wallet::code));
    Data data;
    cell->serialize(data);

    const auto decoded = Cell::deserialize(data.data(), data.size());
    ASSERT_EQ(hex(decoded->hash), "84dafa449f98a6987789ba232358072bc0f76dc4524002a5d0918b9a75d2d599");
}
