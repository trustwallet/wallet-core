// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Algorand/Address.h"
#include "Algorand/Signer.h"
#include "Algorand/BinaryCoding.h"
#include "HexCoding.h"
#include "Base64.h"
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
        Data data;
        encodeNumber(std::get<0>(test), data);
        ASSERT_EQ(hex(data), std::get<1>(test));
    }
}

TEST(AlgorandSigner, EncodeStrings) {
    auto tests = {
        std::make_tuple("algo", "a4616c676f"),
        std::make_tuple("It's like JSON. but fast and small.", "d92349742773206c696b65204a534f4e2e20627574206661737420616e6420736d616c6c2e"),
        std::make_tuple(
            "MessagePack is an efficient binary serialization format. It lets you exchange data among multiple languages like JSON. But it's faster and smaller. Small integers are encoded into a single byte, and typical short strings require only one extra byte in addition to the strings themselves.",
            "da011f4d6573736167655061636b20697320616e20656666696369656e742062696e6172792073657269616c697a6174696f6e20666f726d61742e204974206c65747320796f752065786368616e6765206461746120616d6f6e67206d756c7469706c65206c616e677561676573206c696b65204a534f4e2e2042757420697427732066617374657220616e6420736d616c6c65722e20536d616c6c20696e7465676572732061726520656e636f64656420696e746f20612073696e676c6520627974652c20616e64207479706963616c2073686f727420737472696e67732072657175697265206f6e6c79206f6e65206578747261206279746520696e206164646974696f6e20746f2074686520737472696e6773207468656d73656c7665732e"
        )
    };

    for (auto &test : tests) {
        Data data;
        encodeString(std::get<0>(test), data);
        ASSERT_EQ(hex(data), std::get<1>(test));
    }
}

TEST(AlgorandSigner, EncodeBytes) {
    auto rawtx = "010000000001029294c2b3bd4d25483c4c12432df01a856a38cc0cb48da1a7dd590b7d893392a90000000000ffffffffded892ea55bf1c6ccc495d3493767d7c24497f612b9edc9ab8d30eb671ea76750000000000ffffffff021027000000000000160014b96bacd6f729ef8ac1dd30d159433c0917ba8d3db00f00000000000016001476cd9d430de6db162fc3db509920255ff6d2bdb002483045022100eb8675ff6775e9c399dddba9f178002b745872e541617d690cbce7c933adb87602205de8074c173696de65d4c644a84ea1337c9e9928c7052fddcf9d99e35815e2f20121032858d3a5f9825408ea3959800c5daf22e7a91e459ef168df45071266501d28e102473044022025f1cf362a9c09bd351769f1918ab9f0a6c3f6c4682f29fdbfc08354554ea37b02203f62345b3da4d7a29f58c7c741682be4108a0fb2013980332cc3e081aad7423f01210237d83670da2d3947a58752dab95d59b592c78f2e734d1c14dbf75b29bbe4116100000000";
    Data data;
    encodeBytes(parse_hex(rawtx), data);
    ASSERT_EQ(hex(data), "c50173010000000001029294c2b3bd4d25483c4c12432df01a856a38cc0cb48da1a7dd590b7d893392a90000000000ffffffffded892ea55bf1c6ccc495d3493767d7c24497f612b9edc9ab8d30eb671ea76750000000000ffffffff021027000000000000160014b96bacd6f729ef8ac1dd30d159433c0917ba8d3db00f00000000000016001476cd9d430de6db162fc3db509920255ff6d2bdb002483045022100eb8675ff6775e9c399dddba9f178002b745872e541617d690cbce7c933adb87602205de8074c173696de65d4c644a84ea1337c9e9928c7052fddcf9d99e35815e2f20121032858d3a5f9825408ea3959800c5daf22e7a91e459ef168df45071266501d28e102473044022025f1cf362a9c09bd351769f1918ab9f0a6c3f6c4682f29fdbfc08354554ea37b02203f62345b3da4d7a29f58c7c741682be4108a0fb2013980332cc3e081aad7423f01210237d83670da2d3947a58752dab95d59b592c78f2e734d1c14dbf75b29bbe4116100000000");
}

TEST(AlgorandSigner, Sign) {
    auto key = PrivateKey(parse_hex("c9d3cc16fecabe2747eab86b81528c6ed8b65efc1d6906d86aabc27187a1fe7c"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(publicKey);
    auto to = Address("UCE2U2JC4O4ZR6W763GUQCG57HQCDZEUJY4J5I6VYY4HQZUJDF7AKZO5GM");
    Data note;
    std::string genesisId = "mainnet-v1.0";
    auto genesisHash = Base64::decode("wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=");
    auto transaction = Transaction(
        /* from */ from,
        /* to */ to,
        /* fee */ 488931,
        /* amount */ 847,
        /* first round */ 51,
        /* last round */ 61,
        /* note */ note,
        /* type */ "pay",
        /* genesis id*/ genesisId,
        /* genesis hash*/ genesisHash
    );

    auto serialized = transaction.serialize();
    auto signature = Signer::sign(key, transaction);
    auto result = transaction.serialize(signature);

    ASSERT_EQ(hex(serialized), "89a3616d74cd034fa3666565ce000775e3a2667633a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c763da3726376c420a089aa6922e3b998fadff6cd4808ddf9e021e4944e389ea3d5c638786689197ea3736e64c42074b000b6368551a6066d713e2866002e8dab34b69ede09a72e85a39bbb1f7928a474797065a3706179");
    ASSERT_EQ(hex(signature), "de73363dbdeda0682adca06f6268a16a6ec47253c94d5692dc1c49a84a05847812cf66d7c4cf07c7e2f50f143ec365d405e30b35117b264a994626054d2af604");
    ASSERT_EQ(hex(result), "82a3736967c440de73363dbdeda0682adca06f6268a16a6ec47253c94d5692dc1c49a84a05847812cf66d7c4cf07c7e2f50f143ec365d405e30b35117b264a994626054d2af604a374786e89a3616d74cd034fa3666565ce000775e3a2667633a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c763da3726376c420a089aa6922e3b998fadff6cd4808ddf9e021e4944e389ea3d5c638786689197ea3736e64c42074b000b6368551a6066d713e2866002e8dab34b69ede09a72e85a39bbb1f7928a474797065a3706179");
}
