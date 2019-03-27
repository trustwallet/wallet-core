// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"
#include "../../uint256.h"

namespace TW {
namespace Ethereum {

static constexpr std::size_t encodedIntSize = 32;

// uint256_t

bool is_dynamic(uint256_t) {
    return false;
}

std::size_t size(uint256_t) {
    return encodedIntSize;
}

void encode(uint256_t value, Data& data) {
    Data bytes = store(value);

    append(data, Data(encodedIntSize - bytes.size()));
    append(data, bytes);
}

std::string type_string(uint256_t value) {
    return "uint256";
}

// int256_t

bool is_dynamic(int256_t) {
    return false;
}

std::size_t size(int256_t) {
    return encodedIntSize;
}

void encode(int256_t value, Data& data) {
    encode(static_cast<uint256_t>(value), data);
}

std::string type_string(int256_t value) {
    return "int256";
}

// bool

bool is_dynamic(bool) {
    return false;
}

std::size_t size(bool) {
    return encodedIntSize;
}

void encode(bool v, Data& data) {
    append(data, Data(encodedIntSize - 1));
    data.push_back(v ? 1 : 0);
}

std::string type_string(bool value) {
    return "bool";
}

// int32

bool is_dynamic(int32_t) {
    return false;
}

std::size_t size(int32_t) {
    return encodedIntSize;
}

void encode(int32_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

std::string type_string(int32_t value) {
    return "int32";
}

// uint32

bool is_dynamic(uint32_t) {
    return false;
}

std::size_t size(uint32_t) {
    return encodedIntSize;
}

void encode(uint32_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

std::string type_string(uint32_t value) {
    return "uint32";
}

// int64

bool is_dynamic(int64_t) {
    return false;
}

std::size_t size(int64_t) {
    return encodedIntSize;
}

void encode(int64_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

std::string type_string(int64_t value) {
    return "int64";
}

// uint64

bool is_dynamic(uint64_t) {
    return false;
}

std::size_t size(uint64_t) {
    return encodedIntSize;
}

void encode(uint64_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

std::string type_string(uint64_t value) {
    return "uint64";
}

} // namespace Ethereum
} // namespace TW
