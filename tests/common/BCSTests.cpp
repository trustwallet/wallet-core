// Copyright © 2017-2023 Trust Wallet.
// Created by Clément Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BCS.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::BCS::tests {

TEST(BCS, Integral) {
    Serializer os;
    os << uint32_t(0xAABBCCDD);
    ASSERT_EQ(os.bytes, parse_hex("0xDDCCBBAA"));

    os.clear();
    os << int32_t(-305419896);
    ASSERT_EQ(os.bytes, parse_hex("0x88A9CBED"));
}

TEST(BCS, ULEB128) {
    Serializer os;
    os << uleb128{0x00000001};
    ASSERT_EQ(os.bytes, parse_hex("0x01"));

    os.clear();
    os << uleb128{0x00000080};
    ASSERT_EQ(os.bytes, parse_hex("0x8001"));

    os.clear();
    os << uleb128{0x00004000};
    ASSERT_EQ(os.bytes, parse_hex("0x808001"));

    os.clear();
    os << uleb128{0x00200000};
    ASSERT_EQ(os.bytes, parse_hex("0x80808001"));

    os.clear();
    os << uleb128{0x10000000};
    ASSERT_EQ(os.bytes, parse_hex("0x8080808001"));

    os.clear();
    os << uleb128{0x0000250F};
    ASSERT_EQ(os.bytes, parse_hex("0x8F4A"));
}

TEST(BCS, String) {
    Serializer os;
    os << std::string_view("abcd");
    ASSERT_EQ(os.bytes, parse_hex("0x0461626364"));

    os.clear();
    os << std::string_view("");
    ASSERT_EQ(os.bytes, parse_hex("0x00"));
}

TEST(BCS, Optional) {
    Serializer os;
    os << std::optional{0xBBCCDD};
    ASSERT_EQ(os.bytes, parse_hex("0x01DDCCBB00"));

    os.clear();
    os << std::optional<int>{};
    ASSERT_EQ(os.bytes, parse_hex("0x00"));

    os.clear();
    os << std::nullopt;
    ASSERT_EQ(os.bytes, parse_hex("0x00"));
}

TEST(BCS, Tuple) {
    Serializer os;
    os << std::tuple{uint16_t(1), 'a'};
    ASSERT_EQ(os.bytes, parse_hex("0x010061"));

    os.clear();
    os << std::tuple{std::optional<uint32_t>{123}, std::string_view("abcd"), uint8_t(0x0E)};
    ASSERT_EQ(os.bytes, parse_hex("0x017b00000004616263640e"));

    os.clear();
    os << std::tuple{};
    ASSERT_EQ(os.bytes, (Data{}));
}

TEST(BCS, Pair) {
    Serializer os;
    os << std::pair{uint16_t(1), 'a'};
    ASSERT_EQ(os.bytes, parse_hex("0x010061"));

    os.clear();
    os << std::pair{std::optional<uint32_t>{123}, std::string_view("abcd")};
    ASSERT_EQ(os.bytes, parse_hex("0x017b0000000461626364"));
}

struct my_struct {
    std::optional<uint32_t> first;
    std::string_view second;
    uint8_t third;
};

TEST(BCS, Struct) {
    Serializer os;
    os << my_struct{{123}, "abcd", 0x0E};
    ASSERT_EQ(os.bytes, parse_hex("0x017b00000004616263640e"));
}

TEST(BCS, Variant) {
    using V = std::variant<uint32_t, char, bool>;

    Serializer os;
    os << V{uint32_t(1)};
    ASSERT_EQ(os.bytes, parse_hex("0x0001000000"));

    os.clear();
    os << V{char('a')};
    ASSERT_EQ(os.bytes, parse_hex("0x0161"));

    os.clear();
    os << V{true};
    ASSERT_EQ(os.bytes, parse_hex("0x0201"));
}

TEST(BCS, Map) {
    Serializer os;
    os << std::map<char, char>{{'a', 0}, {'b', 1}, {'c', 2}};
    ASSERT_EQ(os.bytes, parse_hex("0x03610062016302"));
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

Serializer& operator<<(Serializer& stream, my_number n) noexcept {
    return stream << n.get_value();
}

static_assert(CustomSerializable<my_number>, "my_number does not model the CustomSerializable concept");

TEST(BCS, Custom) {
    Serializer os;
    os << my_number{0xBBCCDD};
    ASSERT_EQ(os.bytes, parse_hex("0xDDCCBB00"));
}

TEST(BCS, Vector) {
    Serializer os;
    os << std::vector<uint8_t>{1};
    ASSERT_EQ(os.bytes, parse_hex("0101"));
}

}
