// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Everscale/Cell.h"
#include "Everscale/Wallet.h"
#include "HexCoding.h"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Everscale {

// All hashes could be verified using https://ever.bytie.moe/visualizer

static constexpr auto TX = "te6ccgECDgEAAyUAA7V6uRyM7ESqbjssMUQyAqYyQTlEkfDkEhWjBiC1fvKLabAAAaKsEuhQGeqOSyMlWG32ehDzoVCXMh6ugfMLr6pOPj3b6KD4DR/wAAGirA4jnSYtmdCAADR6V/lIBQQBAg8MQQYcxc1EQAMCAG/JkrcETDHn2AAAAAAAAgAAAAAAAop8XDVxQ98+QpgCzzW0U0opAulbEzfySLp3wLLoHzboQRA6FACdROMjE4gAAAAAAAAAACHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIACCcgkc5v5RyBDaeDdbY8Q+SpmX5OOkhzxFyB/ug4o/bJwJXDMKjAeOEr9OvfcJlgyx80ukSBl43/FO2DXIt1+SuAQCAeAJBgEB3wcBsWgBVyORnYiVTcdlhiiGQFTGSCcokj4cgkK0YMQWr95RbTcAIXxdqfc1KmavZDEIGsfjdBuS4lE5Ox4rJZ+Z+rauOxDRZaC8AAYx6CQAADRVgl0KBMWzOhDACAF7C04VCAAAAAAvMK5pAAAAAAAAAAAAAAAAAA7xIIAFdGVusOGq5cSrATb2hH5h5turvUDrer4E0Mf51wPlefAMAd2IAVcjkZ2IlU3HZYYohkBUxkgnKJI+HIJCtGDEFq/eUW02AMrbR14n5UXrp1deXDU5rl8kQvDfSKbnA+d09dtQe2mo8t94jbtllx/DjCgucIpvywjhJBhWNQjtWXh4dP6qiDAAAAADFszqDukuhIYKAQTQAwsB42IAQvi7U+5qVM1eyGIQNY/G6DclxKJydjxWSz8z9W1cdiGiy0F4AAAAAAAAAAAAAAAAAAALThUIAAAAAC8wrmkAAAAAAAAAAAAAAAAADvEggAV0ZW6w4arlxKsBNvaEfmHm26u9QOt6vgTQx/nXA+V58AwBY4AUk5qcKxU0Kqq8xI6zxcnOzaRMAW8AGxI8jfJSPgiVJ6AAAAAAAAAAAAAARVadlK9wDQBDgBVyORnYiVTcdlhiiGQFTGSCcokj4cgkK0YMQWr95RbTcA==";
static constexpr auto BIG_TX = "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";

Cell::Ref deserialize(const std::string& data) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    Data boc(It(begin(data)), It(end(data)));
    return Cell::deserialize(boc.data(), boc.size());
}

TEST(EverscaleCell, DeserializeTransaction) {
    const auto tx = deserialize(TX);
    ASSERT_EQ(hex(tx->hash), "88a02e7bd8833d384f37d63d4d01deef9a1806937b94a313cc5e8c3cc7643032");

    const auto bigTx = deserialize(BIG_TX);
    ASSERT_EQ(hex(bigTx->hash), "37f29d9f3aa5c7cc783962d861c08705f245f5e5bfedd208dfe08d02e80a47d8");
}

TEST(EverscaleCell, DeserializeWallet) {
    const auto cell = Cell::deserialize(Wallet::code.data(), Wallet::code.size());
    ASSERT_EQ(hex(cell->hash), "84dafa449f98a6987789ba232358072bc0f76dc4524002a5d0918b9a75d2d599");
}

TEST(EverscaleCell, SerializeTransaction) {
    const auto cell = deserialize(TX);
    Data data;
    cell->serialize(data);

    const auto decoded = Cell::deserialize(data.data(), data.size());
    ASSERT_EQ(hex(decoded->hash), "88a02e7bd8833d384f37d63d4d01deef9a1806937b94a313cc5e8c3cc7643032");
}

TEST(EverscaleCell, SerializeWallet) {
    const auto cell = Cell::deserialize(Wallet::code.data(), Wallet::code.size());
    Data data;
    cell->serialize(data);

    const auto decoded = Cell::deserialize(data.data(), data.size());
    ASSERT_EQ(hex(decoded->hash), "84dafa449f98a6987789ba232358072bc0f76dc4524002a5d0918b9a75d2d599");
}

TEST(EverscaleCell, EmptyCell) {
    const auto EMPTY_CELL = "96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7";

    const auto cell1 = deserialize("te6ccgEBAQEAAgAAAA==");
    ASSERT_EQ(hex(cell1->hash), EMPTY_CELL);

    // With index
    const auto cell2 = deserialize("te6ccoEBAQEAAwACAAA=");
    ASSERT_EQ(hex(cell2->hash), EMPTY_CELL);

    // With d2 > 0 && d2%8 != 0 but without end flag (computeBitLen should just return 0)
    const auto cell3 = deserialize("te6ccgEBAQEAAwAABQAAAA==");
    ASSERT_EQ(hex(cell3->hash), EMPTY_CELL);

    // With `storeHashes` (provided hash should be skipped)
    const auto cell4 = deserialize("te6ccgEBAQEAJAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    ASSERT_EQ(hex(cell4->hash), EMPTY_CELL);
}

TEST(EverscaleCell, InvalidCell) {
    // unexpected eof
    ASSERT_THROW(deserialize("te4="), std::runtime_error);
    // unknown magic
    ASSERT_THROW(deserialize("aGVsbG8gd29ybGQK"), std::runtime_error);
    // refSize > 4
    ASSERT_THROW(deserialize("te6ccgUCAAAAAAEAAAAAAQAAAAAAFD4AAAAAAAO3etQ="), std::runtime_error);
    // unsupported root count
    ASSERT_THROW(deserialize("te6ccgECdRIAFD4AA7d61A=="), std::runtime_error);
    // root count is greater than cell count
    ASSERT_THROW(deserialize("te6ccgECAAEAFD4AA7d61A=="), std::runtime_error);
    // absent cells are not supported
    ASSERT_THROW(deserialize("te6ccgECAQESFD4AA7d61A=="), std::runtime_error);
    // non-zero level is not supported
    ASSERT_THROW(deserialize("te6ccgECAQEAFD4AY7d61FeCHf8yG3+VsHLEilQ6UuYy8wcpWi1+/Q=="), std::runtime_error);
    // exotic cells are not supported
    ASSERT_THROW(deserialize("te6ccgECAQEAFD4AC7d61FeCHf8yG3+VsHLEilQ6UuYy8wcpWi1+/Q=="), std::runtime_error);
    // absent cells are not supported
    ASSERT_THROW(deserialize("te6ccgECAQEAFD4AF7d61FeCHf8yG3+VsHLEilQ6UuYy8wcpWi1+/Q=="), std::runtime_error);
    // invalid ref count
    ASSERT_THROW(deserialize("te6ccgEBAQEAAwAFAA=="), std::runtime_error);
    // invalid child index
    ASSERT_THROW(deserialize("te6ccgEBAQEAJAABAP8="), std::runtime_error);
    // invalid child index (reference to itself)
    ASSERT_THROW(deserialize("te6ccgEBAQEAAgABAAA="), std::runtime_error);
    // invalid root index
    ASSERT_THROW(deserialize("te6ccgEBAQEAAwEAAA"), std::runtime_error);
    // child cell not found
    ASSERT_THROW(deserialize("te6ccgEBAQEAAgABAAE="), std::runtime_error);
}

} // namespace TW::Everscale
