// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

namespace TW {
namespace Ethereum {

// dynamic bytes

bool is_dynamic(Data) {
    return true;
}

std::size_t size(const Data& bytes) {
    return 32 + ((bytes.size() + 31) / 32) * 32;
}

void encode(const Data& bytes, Data& data) {
    encode(uint256_t(bytes.size()), data);

    const auto count = std::min(std::size_t(32), bytes.size());
    const auto padding = ((count + 31) / 32) * 32 - count;
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

std::string type_string(const Data& data) {
    return "bytes";
}

// static bytes

template <std::size_t N>
bool is_dynamic(std::array<byte, N>) {
    return false;
}

template <std::size_t N>
std::size_t size(const std::array<byte, N>& bytes) {
    return ((bytes.size() + 31) / 32) * 32;
}

template <std::size_t N>
void encode(const std::array<byte, N>& bytes, Data& data) {
    const auto count = std::min(std::size_t(32), bytes.size());
    const auto padding = ((count + 31) / 32) * 32 - count;
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

template <std::size_t N>
std::string type_string(const std::array<byte, N>& bytes) {
    return "bytes" + std::to_string(N);
}

// string

bool is_dynamic(std::string) {
    return true;
}

std::size_t size(const std::string& string) {
    return string.size();
}

void encode(const std::string& string, Data& data) {
    auto bytes = Data(string.begin(), string.end());
    encode(bytes, data);
}

std::string type_string(const std::string& data) {
    return "string";
}

}} // namespace
