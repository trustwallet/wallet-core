// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "../proto/Tezos.pb.h"

#pragma once

namespace TW::Tezos {

enum PrimType : std::uint8_t {
    Pair = 7,
};

struct MichelsonValue;

struct PrimValue {
    std::string prim;
    std::vector<MichelsonValue> args;
    std::vector<std::string> anots;
};

struct BytesValue {
    std::string bytes;
};

struct StringValue {
    std::string string;
};

struct IntValue {
    std::string _int;
};

struct MichelsonValue {
    using MichelsonArray = std::vector<std::variant<PrimValue, BytesValue, StringValue, IntValue>>;
    using MichelsonVariant = std::variant<
        PrimValue,
        BytesValue,
        StringValue,
        IntValue,
        MichelsonArray>;
    MichelsonVariant value;
};

MichelsonValue::MichelsonVariant FA12ParameterToMichelson(const Proto::FA12Parameters& data);
MichelsonValue::MichelsonVariant FA2ParameterToMichelson(const Proto::FA2Parameters& data);

} // namespace TW::Tezos
