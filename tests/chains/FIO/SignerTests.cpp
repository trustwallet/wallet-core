// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "FIO/Actor.h"
#include "FIO/Signer.h"
#include "FIO/TransactionBuilder.h"

#include "Base58.h"
#include "Hash.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::FIO::tests {

using namespace std;

TEST(FIOSigner, SignEncode) {
    string sig1 = Signer::signatureToBase58(parse_hex("1f4fccc30bcba876963aef6de584daf7258306c02f4528fe25b116b517de8b349968bdc080cd6bef36f5a46d31a7c01ed0806ad215bb66a94f61e27a895d610983"));

    EXPECT_EQ("SIG_K1_K5hJTPeiV4bDkNR13mf66N2DY5AtVL4NU1iWE4G4dsczY2q68oCcUVxhzFdxjgV2eAeb2jNV1biqtCJ3SaNL8kkNgoZ43H", sig1);
}

TEST(FIOSigner, SignInternals) {
    // 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
    PrivateKey pk = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
    {
        Data pk2 = parse_hex("80");
        append(pk2, pk.bytes);
        EXPECT_EQ("5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri", Base58::encodeCheck(pk2));
    }
    Data rawData = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c6574000000000000000000000000000000000000000000000000000000000000000000");
    Data hash = Hash::sha256(rawData);
    EXPECT_EQ("6a82a57fb9bfc43918aa757d6094ba71fa2c7ece1691c4b8551a0607273771d7", hex(hash));
    Data sign2 = Signer::signData(pk, rawData);
    EXPECT_EQ("1f6ccee1f4cd188cc8aefa63f8fda8c90c0493ca1504806d3a26a7300a9687bb701f188337bc9a32f01ee0c2ecf030aee197b050460d72f7272cc6ce36ef14c95b", hex(sign2));

    string sigStr = Signer::signatureToBase58(sign2);
    EXPECT_EQ("SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9", sigStr);
    EXPECT_TRUE(Signer::verify(pk.getPublicKey(TWPublicKeyTypeSECP256k1), hash, sign2));
}

TEST(FIOSigner, Actor) {
    {
        const auto addr1 = "FIO6cDpi7vPnvRwMEdXtLnAmFwygaQ8CzD7vqKLBJ2GfgtHBQ4PPy";
        Address addr = Address(addr1);
        EXPECT_EQ(addr1, addr.string());

        uint64_t shortenedKey = Actor::shortenKey(addr.bytes);
        EXPECT_EQ(1518832697283783336U, shortenedKey);
        string name = Actor::name(shortenedKey);
        EXPECT_EQ("2odzomo2v4pec", name);
    }
    const int n = 4;
    const string addrArr[n] = {
        "FIO6cDpi7vPnvRwMEdXtLnAmFwygaQ8CzD7vqKLBJ2GfgtHBQ4PPy",
        "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE",
        "FIO7bxrQUTbQ4mqcoefhWPz1aFieN4fA9RQAiozRz7FrUChHZ7Rb8",
        "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf",
    };
    const string actorArr[n] = {
        "2odzomo2v4pe",
        "hhq2g4qgycfb",
        "5kmx4qbqlpld",
        "qdfejz2a5wpl",
    };
    for (int i = 0; i < n; ++i) {
        Address addr = Address(addrArr[i]);
        EXPECT_EQ(addrArr[i], addr.string());

        string actor = Actor::actor(addr);
        EXPECT_EQ(actorArr[i], actor);
    }
}

TEST(FIOSigner, compile) {
        const Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
        // 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
        const PrivateKey privKeyBA = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
        const PublicKey pubKey6M = privKeyBA.getPublicKey(TWPublicKeyTypeSECP256k1);
        const Address addr6M(pubKey6M);
        Proto::SigningInput input;
        input.set_expiry(1579790000);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(addr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(50000);
        input.mutable_chain_params()->set_ref_block_prefix(4000123456);

        input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
        input.mutable_action()->mutable_transfer_message()->set_fee(250000000);
        input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");

        auto txBytes = TransactionBuilder::buildUnsignedTxBytes(input);
        // create signature
        Data sigBuf(chainId);
        append(sigBuf, txBytes);
        append(sigBuf, TW::Data(32)); // context_free
        Data signature = Signer::signData(privKeyBA, sigBuf);
    
        Proto::SigningOutput result = Signer::compile(input, signature);
        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})"
        , result.json());
        EXPECT_EQ(result.action_name(), "trnsfiopubky");
}

} // namespace TW::FIO::tests
