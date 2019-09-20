// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Algorand/Address.h"
#include "Algorand/Signer.h"
#include "Algorand/BinaryCoding.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Algorand;

TEST(AlgorandSigner, EncodeNumbers) {
    auto tests = {
        std::make_tuple(100ull, "64"),
        std::make_tuple(200ull, "ccc8"),
        std::make_tuple(55536ull, "cdd8f0"),
        std::make_tuple(3294967296ull, "cec4653600"),
        std::make_tuple(14294967296ull, "cf00000003540be400"),
    };

    for (auto &test : tests) {
        auto data = Data();
        encodeNumber(std::get<0>(test), data);
        ASSERT_EQ(hex(data), std::get<1>(test));
    }
}

TEST(AlgorandSigner, Sign) {
    auto key = PrivateKey(parse_hex("c9d3cc16fecabe2747eab86b81528c6ed8b65efc1d6906d86aabc27187a1fe7c"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(publicKey);
    auto to = Address("UCE2U2JC4O4ZR6W763GUQCG57HQCDZEUJY4J5I6VYY4HQZUJDF7AKZO5GM");
    auto note = Data();
    auto transaction = Transaction(
        /* from */ from,
        /* to */ to,
        /* fee */ 488931,
        /* amount */ 847,
        /* first round */ 51,
        /* last round */ 61,
        /* note */ note,
        /* type */ "pay"
    );

    auto serialized = transaction.serialize();
    auto signature = Signer::sign(key, transaction);
    auto result = transaction.serialize(signature);

    ASSERT_EQ(hex(serialized), "89a3616d74cd034fa3666565ce000775e3a2667633a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c763da3726376c420a089aa6922e3b998fadff6cd4808ddf9e021e4944e389ea3d5c638786689197ea3736e64c42074b000b6368551a6066d713e2866002e8dab34b69ede09a72e85a39bbb1f7928a474797065a3706179");
    ASSERT_EQ(hex(result), "82a3736967c440de73363dbdeda0682adca06f6268a16a6ec47253c94d5692dc1c49a84a05847812cf66d7c4cf07c7e2f50f143ec365d405e30b35117b264a994626054d2af604a374786e89a3616d74cd034fa3666565ce000775e3a2667633a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c763da3726376c420a089aa6922e3b998fadff6cd4808ddf9e021e4944e389ea3d5c638786689197ea3736e64c42074b000b6368551a6066d713e2866002e8dab34b69ede09a72e85a39bbb1f7928a474797065a3706179");
}
