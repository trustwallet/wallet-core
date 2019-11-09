// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Tuple.h"

#include "../../Hash.h"

#include <string>
#include <tuple>

namespace TW::Ethereum {

template <typename... Params>
class Function {
  public:
    std::string name;
    std::tuple<Params...> parameters;

    Function() = default;
    Function(std::string name, std::tuple<Params...> parameters)
        : name(std::move(name)), parameters(std::move(parameters)) {}
};

template <typename... Params>
inline bool is_dynamic(const Function<Params...>& f) {
    return is_dynaic(f.parameters);
}

template <typename... Params>
inline bool size(const Function<Params...>& f) {
    return 4 + size(f.parameters);
}

template <typename... Params>
inline void encode(const Function<Params...>& f, Data& data) {
    auto string = type_string(f);
    auto hash = Hash::keccak256(Data(string.begin(), string.end()));
    auto signature = Data(hash.begin(), hash.begin() + 4);
    append(data, signature);
    encode(f.parameters, data);
}

template <typename... Params>
inline bool decode(const Data& encoded, Function<Params...>& function, size_t& offset_inout) {
    // read 4-byte hash
    std::array<byte, 4> hash;
    if (!decode(encoded, hash, offset_inout)) { return false; }
    // adjust offset; hash is NOT padded to 32 bytes
    offset_inout = offset_inout - 32 + 4;
    // verify hash
    auto string = type_string(function);
    Data hashExpect = Hash::keccak256(Data(string.begin(), string.end()));
    hashExpect = Data(hashExpect.begin(), hashExpect.begin() + 4);
    if (Data(hash.begin(), hash.end()) != hashExpect) {
        // invalid hash
        return false;
    }
    // read parameters
    if (!decode(encoded, function.parameters, offset_inout)) { return false; }
    return true;
}

template <typename... Params>
inline std::string type_string(const Function<Params...>& f) {
    return f.name + "(" + type_string(f.parameters) + ")";
}

} // namespace TW::Ethereum
