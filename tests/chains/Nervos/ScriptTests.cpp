// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "Nervos/Script.h"

#include <gtest/gtest.h>

namespace TW::Nervos::tests {

static Script scriptWithHashType(HashType hashType) {
    return Script(parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8"),
                  hashType, parse_hex("b39bbc0b3673c7d36450bc14cfcdad2d559c6c64"));
}

TEST(NervosScript, HashTypeStringMapping) {
    ASSERT_EQ(scriptWithHashType(HashType::Data0).proto().hash_type(), "data");
    ASSERT_EQ(scriptWithHashType(HashType::Type1).proto().hash_type(), "type");
    ASSERT_EQ(scriptWithHashType(HashType::Data1).proto().hash_type(), "data1");
    ASSERT_EQ(scriptWithHashType(HashType::Data2).proto().hash_type(), "data2");
}

TEST(NervosScript, ProtoRoundTrip) {
    for (auto hashType : {HashType::Data0, HashType::Type1, HashType::Data1, HashType::Data2}) {
        const auto script = scriptWithHashType(hashType);
        ASSERT_EQ(Script(script.proto()), script);
    }
}

TEST(NervosScript, JsonHashType) {
    ASSERT_EQ(scriptWithHashType(HashType::Data2).json()["hash_type"], "data2");
}

TEST(NervosScript, UnknownProtoHashTypeFallsBackToData0) {
    auto proto = scriptWithHashType(HashType::Data2).proto();
    proto.set_hash_type("data3");
    ASSERT_EQ(Script(proto).hashType, HashType::Data0);
}

} // namespace TW::Nervos::tests
