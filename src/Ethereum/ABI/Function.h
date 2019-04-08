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
bool is_dynamic(const Function<Params...>& f) {
    return is_dynaic(f.parameters);
}

template <typename... Params>
bool size(const Function<Params...>& f) {
    return 4 + size(f.parameters);
}

template <typename... Params>
void encode(const Function<Params...>& f, Data& data) {
    auto string = type_string(f);
    auto hash = Hash::keccak256(Data(string.begin(), string.end()));
    auto signature = Data(hash.begin(), hash.begin() + 4);
    append(data, signature);
    encode(f.parameters, data);
}

template <typename... Params>
std::string type_string(const Function<Params...>& f) {
    return f.name + "(" + type_string(f.parameters) + ")";
}

} // namespace TW::Ethereum
