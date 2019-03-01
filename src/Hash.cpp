// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"

#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/ripemd160.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>

#include <string>

using namespace TW;

template<typename T>
Data Hash::sha1(const T& data) {
    Data result(sha1Size);
    sha1_Raw(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}

template Data Hash::sha1(const Data& data);
template Data Hash::sha1(const std::string& data);

template<typename T>
Data Hash::sha256(const T& data) {
    Data result(sha256Size);
    sha256_Raw(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}

template Data Hash::sha256(const Data& data);
template Data Hash::sha256(const std::string& data);

template<typename T>
Data Hash::sha512(const T& data) {
    Data result(sha512Size);
    sha512_Raw(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::sha512(const Data& data);
template Data Hash::sha512(const std::string& data);

template<typename T>
Data Hash::keccak256(const T& data) {
    Data result(sha256Size);
    keccak_256(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::keccak256(const Data& data);
template Data Hash::keccak256(const std::string& data);

template<typename T>
Data Hash::keccak512(const T& data) {
    Data result(sha512Size);
    keccak_512(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::keccak512(const Data& data);
template Data Hash::keccak512(const std::string& data);

template<typename T>
Data Hash::sha3_256(const T& data) {
    Data result(sha256Size);
    ::sha3_256(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::sha3_256(const Data& data);
template Data Hash::sha3_256(const std::string& data);

template<typename T>
Data Hash::sha3_512(const T& data) {
    Data result(sha512Size);
    ::sha3_512(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::sha3_512(const Data& data);
template Data Hash::sha3_512(const std::string& data);

template<typename T>
Data Hash::ripemd(const T& data) {
    Data result(ripemdSize);
    ripemd160(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data());
    return result;
}
template Data Hash::ripemd(const Data& data);
template Data Hash::ripemd(const std::string& data);

template<typename T>
Data Hash::blake2b(const T& data, size_t size) {
    Data result(size);
    ::blake2b(reinterpret_cast<const uint8_t*>(data.data()), data.size(), result.data(), size);
    return result;
}
template Data Hash::blake2b(const Data& data, size_t size);
template Data Hash::blake2b(const std::string& data, size_t size);

template<typename T>
Data Hash::blake2b(const T& data, size_t size, const Data& personal) {
    Data result(size);
    ::blake2b_Personal(reinterpret_cast<const uint8_t*>(data.data()), data.size(), personal.data(), personal.size(), result.data(), size);
    return result;
}
template Data Hash::blake2b(const Data& data, size_t size, const Data& personal);
template Data Hash::blake2b(const std::string& data, size_t size, const Data& personal);
