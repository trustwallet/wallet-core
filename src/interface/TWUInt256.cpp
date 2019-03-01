// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWUInt256.h>

#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::uint256_t;
using boost::multiprecision::cpp_int;

struct TWUInt256 TWUInt256Zero() {
    return TWUInt256{0};
}

struct TWUInt256 TWUInt256One(void) {
    TWUInt256 result{0};

    uint256_t i = 1;
    export_bits(i, std::make_reverse_iterator(result.bytes + 32), 8, false);

    return result;
}

bool TWUInt256InitWithData(struct TWUInt256 *_Nonnull instance, TWData *_Nonnull data) {
    const auto size = TWDataSize(data);
    if (size < 32) {
        memset(instance->bytes, 0, 32);
        TWDataCopyBytes(data, 0, TWDataSize(data), instance->bytes + 32 - TWDataSize(data));
    } else if (size == 32) {
        TWDataCopyBytes(data, 0, 32, instance->bytes);
    } else {
        return false;
    }
    return true;
}

bool TWUInt256InitWithString(struct TWUInt256 *_Nonnull instance, TWString *_Nonnull string) {
    try {
        uint256_t i(TWStringUTF8Bytes(string));
        export_bits(i, std::make_reverse_iterator(instance->bytes + 32), 8, false);
    } catch (std::runtime_error e) {
        return false;
    }
    return true;
}

void TWUInt256InitWithUInt32(struct TWUInt256 *_Nonnull instance, uint32_t value) {
    uint256_t i = value;
    std::fill(instance->bytes, instance->bytes + 32, 0);
    export_bits(i, std::make_reverse_iterator(instance->bytes + 32), 8, false);
}

void TWUInt256InitWithUInt64(struct TWUInt256 *_Nonnull instance, uint64_t value) {
    uint256_t i = value;
    std::fill(instance->bytes, instance->bytes + 32, 0);
    export_bits(i, std::make_reverse_iterator(instance->bytes + 32), 8, false);
}

bool TWUInt256IsZero(struct TWUInt256 value){
    uint256_t i;
    import_bits(i, value.bytes, value.bytes + 32);
    return i == 0;
}

uint32_t TWUInt256UInt32Value(struct TWUInt256 value) {
    uint256_t i;
    import_bits(i, value.bytes, value.bytes + 32);
    return static_cast<uint32_t>(i);
}

uint64_t TWUInt256UInt64Value(struct TWUInt256 value) {
    uint256_t i;
    import_bits(i, value.bytes, value.bytes + 32);
    return static_cast<uint32_t>(i);
}

TWData *_Nonnull TWUInt256Data(struct TWUInt256 value) {
    return TWDataCreateWithBytes(value.bytes, 32);
}

TWString *_Nonnull TWUInt256Description(struct TWUInt256 value) {
    uint256_t i;
    import_bits(i, value.bytes, value.bytes + 32);
    auto string = boost::lexical_cast<std::string>(i);
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

bool TWUInt256Equal(struct TWUInt256 lhs, struct TWUInt256 rhs) {
    uint256_t i;
    uint256_t j;
    import_bits(i, lhs.bytes, lhs.bytes + 32);
    import_bits(j, rhs.bytes, rhs.bytes + 32);
    return i == j;
}

bool TWUInt256Less(struct TWUInt256 lhs, struct TWUInt256 rhs) {
    uint256_t i;
    uint256_t j;
    import_bits(i, lhs.bytes, lhs.bytes + 32);
    import_bits(j, rhs.bytes, rhs.bytes + 32);
    return i < j;
}

inline void formatPush(int n, std::string& s, int& decimals) {
    if (n > 0 || decimals <= 1) {
        s.push_back('0' + n);
    }
    if (decimals > 0 && decimals-- == 1) {
        s.push_back('.');
    }
}

TWString *_Nonnull TWUInt256Format(struct TWUInt256 value, int decimals) {
    uint256_t i;
    import_bits(i, value.bytes, value.bytes + 32);
    auto string = boost::lexical_cast<std::string>(i);
    if (decimals == 0) {
        return TWStringCreateWithUTF8Bytes(string.c_str());
    }

    if (string.size() <= decimals) {
        auto prefix = std::string("0.");
        auto padding = std::string(decimals - string.size(), '0');
        string.insert(string.begin(), padding.begin(), padding.end());
        string.insert(string.begin(), prefix.begin(), prefix.end());
    } else {
        string.insert(string.end() - decimals, '.');
    }

    auto zerosBegin = std::find_if(string.rbegin(), string.rbegin() + decimals - 1, [](auto& c){ return c != '0' && c != '.'; }).base();
    string.erase(zerosBegin, string.end());

    return TWStringCreateWithUTF8Bytes(string.c_str());
}
