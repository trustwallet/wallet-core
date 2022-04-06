// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Hash.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

Data sha1(Data& data) {
    return Hash::sha1(data);
}

Data sha256(Data& data) {
    return Hash::sha256(data);
}

Data sha512(Data& data) {
    return Hash::sha512(data);
}

Data sha512_256(Data& data) {
    return Hash::sha512_256(data);
}

Data keccak256(Data& data) {
    return Hash::keccak256(data);
}

Data keccak512(Data& data) {
    return Hash::keccak512(data);
}

Data sha3_256(Data& data) {
    return Hash::sha3_256(data);
}

Data sha3_512(Data& data) {
    return Hash::sha3_512(data);
}

Data ripemd(Data& data) {
    return Hash::ripemd(data);
}

Data blake256(Data& data) {
    return Hash::blake256(data);
}

Data blake2b_size(Data& data, size_t size) {
    return Hash::blake2b(data, size);
}

Data blake2b_size_per(Data& data, size_t size, const Data& personal) {
    return Hash::blake2b(data, size, personal);
}

EMSCRIPTEN_BINDINGS(wallet_hash_module) {
    function("sha1", &sha1);
    function("sha256", &sha256);
    function("sha512", &sha512);
    function("sha512_256", &sha512_256);
    function("keccak256", &keccak256);
    function("keccak512", &keccak512);
    function("sha3_256", &sha3_256);
    function("sha3_512", &sha3_512);
    function("ripemd", &ripemd);
    function("blake256", &blake256);
    function("blake2b_size", &blake2b_size);
    function("blake2b_size_per", &blake2b_size_per);
}
