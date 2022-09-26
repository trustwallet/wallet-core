// Copyright © 2017-2022 Trust Wallet.
// Created by Clément Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BCS.h"

#include <gtest/gtest.h>

namespace TW::BCS::tests {

TEST(BCS, Integral) {
    output_stream os;
    os << uint32_t(0xAABBCCDD);
    ASSERT_EQ(os.bytes, (Data{0xDD, 0xCC, 0xBB, 0xAA}));

    os.clear();
    os << int32_t(-305419896);
    ASSERT_EQ(os.bytes, (Data{0x88, 0xA9, 0xCB, 0xED}));
}

TEST(BCS, ULEB128) {
    output_stream os;
    os << uleb128{0x00000001};
    ASSERT_EQ(os.bytes, (Data{0x01}));

    os.clear();
    os << uleb128{0x00000080};
    ASSERT_EQ(os.bytes, (Data{0x80, 0x01}));

    os.clear();
    os << uleb128{0x00004000};
    ASSERT_EQ(os.bytes, (Data{0x80, 0x80, 0x01}));

    os.clear();
    os << uleb128{0x00200000};
    ASSERT_EQ(os.bytes, (Data{0x80, 0x80, 0x80, 0x01}));

    os.clear();
    os << uleb128{0x10000000};
    ASSERT_EQ(os.bytes, (Data{0x80, 0x80, 0x80, 0x80, 0x01}));

    os.clear();
    os << uleb128{0x0000250F};
    ASSERT_EQ(os.bytes, (Data{0x8F, 0x4A}));
}

TEST(BCS, String) {
    output_stream os;
    os << std::string_view("abcd");
    ASSERT_EQ(os.bytes, (Data{0x04, 'a', 'b', 'c', 'd'}));

    os.clear();
    os << std::string_view("");
    ASSERT_EQ(os.bytes, (Data{0x00}));
}

TEST(BCS, Optional) {
    output_stream os;
    os << std::optional{0xBBCCDD};
    ASSERT_EQ(os.bytes, (Data{0x01, 0xDD, 0xCC, 0xBB, 0x00}));

    os.clear();
    os << std::optional<int>{};
    ASSERT_EQ(os.bytes, (Data{0x00}));

    os.clear();
    os << std::nullopt;
    ASSERT_EQ(os.bytes, (Data{0x00}));
}

TEST(BCS, Tuple) {
    output_stream os;
    os << std::tuple{uint16_t(1), 'a'};
    ASSERT_EQ(os.bytes, (Data{0x01, 0x00, 'a'}));

    os.clear();
    os << std::tuple{std::optional<uint32_t>{123}, std::string_view("abcd"), uint8_t(0x0E)};
    ASSERT_EQ(os.bytes, (Data{0x01, 123, 0x00, 0x00, 0x00, 0x04, 'a', 'b', 'c', 'd', 0x0E}));

    os.clear();
    os << std::tuple{};
    ASSERT_EQ(os.bytes, (Data{}));
}

TEST(BCS, Pair) {
    output_stream os;
    os << std::pair{uint16_t(1), 'a'};
    ASSERT_EQ(os.bytes, (Data{0x01, 0x00, 'a'}));

    os.clear();
    os << std::pair{std::optional<uint32_t>{123}, std::string_view("abcd")};
    ASSERT_EQ(os.bytes, (Data{0x01, 123, 0x00, 0x00, 0x00, 0x04, 'a', 'b', 'c', 'd'}));
}

struct my_struct {
    std::optional<uint32_t> first;
    std::string_view second;
    uint8_t third;
};

TEST(BCS, Struct) {
    output_stream os;
    os << my_struct{{123}, "abcd", 0x0E};
    ASSERT_EQ(os.bytes, (Data{0x01, 123, 0x00, 0x00, 0x00, 0x04, 'a', 'b', 'c', 'd', 0x0E}));
}

TEST(BCS, Variant) {
    using V = std::variant<uint32_t, char, bool>;

    output_stream os;
    os << V{uint32_t(1)};
    ASSERT_EQ(os.bytes, (Data{0x00, 0x01, 0x00, 0x00, 0x00}));

    os.clear();
    os << V{char('a')};
    ASSERT_EQ(os.bytes, (Data{0x01, 'a'}));

    os.clear();
    os << V{true};
    ASSERT_EQ(os.bytes, (Data{0x02, 0x01}));
}

TEST(BCS, Map) {
    output_stream os;
    os << std::map<char, char>{{'a', 0}, {'b', 1}, {'c', 2}};
    ASSERT_EQ(os.bytes, (Data{0x03, 'a', 0x00, 'b', 0x01, 'c', 0x02}));
}

class my_number {
private:
    int value;

public:
    explicit my_number(int value) noexcept
        : value(value) {
    }

    [[nodiscard]] auto get_value() const {
        return value;
    }
};

output_stream& operator<<(output_stream& stream, my_number n) noexcept {
    return stream << n.get_value();
}

static_assert(CustomSerializable<my_number>, "my_number does not model the CustomSerializable concept");

TEST(BCS, Custom) {
    output_stream os;
    os << my_number{0xBBCCDD};
    ASSERT_EQ(os.bytes, (Data{0xDD, 0xCC, 0xBB, 0x00}));
}

}
