// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../proto/Tezos.pb.h"
#include "uint256.h"

#include <string>
#include <vector>
#include <variant>

using namespace TW;
using namespace TW::Tezos::Proto;

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


MichelsonValue::MichelsonVariant FA12ParameterToMichelson(const FA12TransactionOperationData& data);

Data forgeBool(bool input);
Data forgeOperation(const Operation& operation);
Data forgeAddress(const std::string& address);
Data forgeArray(const Data& data, int len = 4);
Data forgePublicKeyHash(const std::string& publicKeyHash);
Data forgePublicKey(PublicKey publicKey);
Data forgeZarith(uint64_t input);
Data forgeInt32(int value, int len = 4);
Data forgeString(const std::string& value, std::size_t len = 4);
Data forgeEntrypoint(const std::string& value);
Data forgeMichelson(const MichelsonValue::MichelsonVariant& value);
Data forgeMicheInt(const TW::int256_t& value);
Data forgePrim(const PrimValue& value);
