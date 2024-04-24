// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "uint256.h"
#include "HexCoding.h"
#include "NEO/ReadData.h"
#include "NEO/Witness.h"

#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOWitness, Serialize) {
    auto witness = Witness();
    string invocationScript = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    string verificationScript = "cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b";
    witness.invocationScript = parse_hex(invocationScript);
    witness.verificationScript = parse_hex(verificationScript);
    EXPECT_EQ("20" + invocationScript + "14" + verificationScript, hex(witness.serialize()));
    EXPECT_EQ((size_t)witness.size(), witness.invocationScript.size() + witness.verificationScript.size());

    invocationScript = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4aba";
    verificationScript = "cbb23e6f9ade28d5a8ff3eac9d73af039e821b";
    witness.invocationScript = parse_hex(invocationScript);
    witness.verificationScript = parse_hex(verificationScript);
    EXPECT_EQ("21" + invocationScript + "13" + verificationScript, hex(witness.serialize()));
    EXPECT_EQ((size_t)witness.size(), witness.invocationScript.size() + witness.verificationScript.size());
}

TEST(NEOWitness, Deserialize) {
    auto witness = Witness();
    string invocationScript = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    string verificationScript = "cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b";
    witness.deserialize(parse_hex("20" + invocationScript + "14" + verificationScript));
    EXPECT_EQ(invocationScript, hex(witness.invocationScript));
    EXPECT_EQ(verificationScript, hex(witness.verificationScript));

    invocationScript = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4aba";
    verificationScript = "cbb23e6f9ade28d5a8ff3eac9d73af039e821b";
    witness.deserialize(parse_hex("21" + invocationScript + "13" + verificationScript));
    EXPECT_EQ(invocationScript, hex(witness.invocationScript));
    EXPECT_EQ(verificationScript, hex(witness.verificationScript));
}

TEST(NEOWitness, SerializeDeserialize) {
    auto witness = Witness();
    string invocationScript = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    invocationScript += invocationScript + invocationScript;
    invocationScript += invocationScript + invocationScript;

    string verificationScript = "cbb23e6f9ade28d5a8ff3eac9d73af039e821b1b";
    verificationScript += verificationScript + verificationScript;
    verificationScript += verificationScript + verificationScript;

    witness.invocationScript = parse_hex(invocationScript);
    witness.verificationScript = parse_hex(verificationScript);

    auto deWitness = Witness();
    deWitness.deserialize(witness.serialize());
    EXPECT_EQ(witness, deWitness);
}

} // namespace TW::NEO::tests
