// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "FIO/Action.h"
#include "FIO/NewFundsRequest.h"
#include "FIO/Signer.h"
#include "FIO/Transaction.h"
#include "FIO/TransactionBuilder.h"

#include "BinaryCoding.h"
#include "HexCoding.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace TW::FIO::TransactionBuilderTests {
using namespace std;

const Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
// 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
const PrivateKey gPrivKeyBA = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
const PublicKey gPubKey6MA = gPrivKeyBA.getPublicKey(TWPublicKeyTypeSECP256k1);
const Address gAddr6M(gPubKey6MA);

TEST(FIOTransactionBuilder, RegisterFioAddressGeneric) {
    Proto::SigningInput input;
    input.set_expiry(1579784511);
    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(gPrivKeyBA.bytes.begin(), gPrivKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
    input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(gAddr6M.string());
    input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);

    auto json = TransactionBuilder::sign(input);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", json);
}

TEST(FIOTransactionBuilder, RegisterFioAddress) {
    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 5000000000;

    string t = TransactionBuilder::createRegisterFioAddress(gAddr6M, gPrivKeyBA, "adam@fiotestnet",
                                                            chainParams, fee, "rewards@wallet", 1579784511);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", t);
}

TEST(FIOTransactionBuilder, AddPubAddress) {
    ChainParams chainParams{chainId, 11565, 4281229859};

    string t = TransactionBuilder::createAddPubAddress(gAddr6M, gPrivKeyBA, "adam@fiotestnet", {{"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"}, {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"}, {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
                                                       chainParams, 0, "rewards@wallet", 1579729429);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K3zimaMKU8cBhVRPw46KM2u7uQWaAKXrnoeYZ7MEbp6sVJcDQmQR2RtdavpUPwkAnYUkd8NqLun8H48tcxZBcTUgkiPGVJ"]})", t);
}

TEST(FIOTransactionBuilder, Transfer) {
    ChainParams chainParams{chainId, 50000, 4000123456};
    string payee = "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE";
    uint64_t amount = 1000000000;
    uint64_t fee = 250000000;

    string t = TransactionBuilder::createTransfer(gAddr6M, gPrivKeyBA, payee, amount,
                                                  chainParams, fee, "rewards@wallet", 1579790000);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})", t);
}

TEST(FIOTransactionBuilder, RenewFioAddress) {
    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 3000000000;

    string t = TransactionBuilder::createRenewFioAddress(gAddr6M, gPrivKeyBA, "nick@fiotestnet",
                                                         chainParams, fee, "rewards@wallet", 1579785000);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff0000000001003056372503a85b80b1ba2919aea6ba01102b2f46fca756b200000000a8ed32322f0f6e69636b4066696f746573746e6574005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_Jxz7oCJ7Z4ECsxqb2utqBcyP3zPQCeQCBws9wWQjyptUKoWVk2AyCVEqtdMHJwqtLniio5Z7npMnaZB8E4pa2G75P9uGkb"]})", t);
}

TEST(FIOTransactionBuilder, NewFundsRequest) {
    {
        ChainParams chainParams{chainId, 18484, 3712870657};
        const Data iv = parse_hex("000102030405060708090a0b0c0d0e0f"); // use fixed iv for testability
        string t = TransactionBuilder::createNewFundsRequest(
            Address("FIO5NMm9Vf3NjYFnhoc7yxTCrLW963KPUCzeMGv3SJ6zR3GMez4ub"), gPrivKeyBA,
            "tag@fiotestnet", "FIO7iYHtDhs45smFgSqLyJ6Zi4D3YG8K5bZGyxmshLCDXXBPbbmJN", "dapixbp@fiotestnet", "14R4wEsGT58chmqo7nB65Dy4je6TiijDWc",
            "1", "BTC", "payment", "", "",
            chainParams, 800000000, "", 1583528215, iv);
        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"17b9625e344801e94ddd000000000100403ed4aa0ba85b00acba384dbdb89a01702096fedf5bf9f900000000a8ed3232d2010e7461674066696f746573746e657412646170697862704066696f746573746e6574980141414543417751464267634943516f4c4441304f447a684533513779504a4738592f52486a69545576436163734a444a516243612f41643763354e36354f56366d3441566974596379654e7a4749306d4c366b5a71567348737837537845623471724d346435567258364939746a6842447067566b3078596575325861676759516b323168684972306c76412b7535546977545661673d3d0008af2f000000000c7a62777072727a796d736b620000","signatures":["SIG_K1_K95jnXSBCf1BnQXQPZzxKYPGxugwpbeVp2NSjN1kmYd9SQibvnSfh2ggmSVXii4Jvq3dtRHFA8s7n3kcQdLhY4KMrkgDgp"]})", t);
    }

    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 3000000000;

    const Data iv = parse_hex("000102030405060708090a0b0c0d0e0f"); // use fixed iv for testability
    string t = TransactionBuilder::createNewFundsRequest(gAddr6M, gPrivKeyBA,
                                                         "mario@fiotestnet", "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o", "alice@fiotestnet", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v",
                                                         "5", "BTC", "Memo", "Hash", "https://trustwallet.com",
                                                         chainParams, fee, "rewards@wallet", 1579785000, iv);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff000000000100403ed4aa0ba85b00acba384dbdb89a01102b2f46fca756b200000000a8ed32328802106d6172696f4066696f746573746e657410616c6963654066696f746573746e6574c00141414543417751464267634943516f4c4441304f442f3575342f6b436b7042554c4a44682f546951334d31534f4e4938426668496c4e54766d39354249586d54396f616f7a55632f6c6c3942726e57426563464e767a76766f6d3751577a517250717241645035683433305732716b52355266416555446a704f514732364c347a6936767241553052764855474e382b685779736a6971506b2b7a455a444952534678426268796c69686d59334f4752342f5a46466358484967343241327834005ed0b2000000000c716466656a7a32613577706c0e726577617264734077616c6c657400","signatures":["SIG_K1_Kk79iVcQMpqpVgZwGTmC1rxgCTLy5BDFtHd8FvjRNm2FqNHR9dpeUmPTNqBKGMNG3BsPy4c5u26TuEDpS87SnyMpF43cZk"]})", t);
}

TEST(FIOTransaction, ActionRegisterFioAddressInternal) {
    RegisterFioAddressData radata("adam@fiotestnet", gAddr6M.string(),
                                  5000000000, "rewards@wallet", "qdfejz2a5wpl");
    Data ser1;
    radata.serialize(ser1);
    EXPECT_EQ(
        hex(parse_hex("0F6164616D4066696F746573746E65743546494F366D31664D645470526B52426E6564765973685843784C4669433573755255384B44667838787874587032686E7478706E6600F2052A01000000102B2F46FCA756B20E726577617264734077616C6C6574")),
        hex(ser1));

    Action raAction;
    raAction.account = "fio.address";
    raAction.name = "regaddress";
    raAction.actionDataSer = ser1;
    raAction.auth.authArray.push_back(Authorization{"qdfejz2a5wpl", "active"});
    Data ser2;
    raAction.serialize(ser2);
    EXPECT_EQ(
        "003056372503a85b0000c6eaa66498ba0"
        "1102b2f46fca756b200000000a8ed3232"
        "65"
        "0f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400",
        hex(ser2));

    Transaction tx;
    tx.expiration = 1579784511;
    tx.refBlockNumber = 39881;
    tx.refBlockPrefix = 4279583376;
    tx.actions.push_back(raAction);
    Data ser3;
    tx.serialize(ser3);
    EXPECT_EQ(
        "3f99295ec99b904215ff0000000001"
        "003056372503a85b0000c6eaa66498ba0"
        "1102b2f46fca756b200000000a8ed3232"
        "65"
        "0f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400",
        hex(ser3));
}

TEST(FIOTransaction, ActionAddPubAddressInternal) {
    AddPubAddressData aadata("adam@fiotestnet", {{"BTC", "BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"}, {"ETH", "ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"}, {"BNB", "BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
                             0, "rewards@wallet", "qdfejz2a5wpl");
    Data ser1;
    aadata.serialize(ser1);
    EXPECT_EQ(
        "0f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c6574",
        hex(ser1));

    Action aaAction;
    aaAction.account = "fio.address";
    aaAction.name = "addaddress";
    aaAction.actionDataSer = ser1;
    aaAction.auth.authArray.push_back(Authorization{"qdfejz2a5wpl", "active"});
    Data ser2;
    aaAction.serialize(ser2);
    EXPECT_EQ(
        "003056372503a85b0000c6eaa66452320"
        "1102b2f46fca756b200000000a8ed3232"
        "c901"
        "0f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400",
        hex(ser2));

    Transaction tx;
    tx.expiration = 1579729429;
    tx.refBlockNumber = 11565;
    tx.refBlockPrefix = 4281229859;
    tx.actions.push_back(aaAction);
    Data ser3;
    tx.serialize(ser3);
    EXPECT_EQ(
        "15c2285e2d2d23622eff0000000001"
        "003056372503a85b0000c6eaa66452320"
        "1102b2f46fca756b200000000a8ed3232"
        "c901"
        "0f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400",
        hex(ser3));
}

TEST(FIONewFundsContent, serialize) {
    {
        NewFundsContent newFunds{"bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v", "10", "BTC", "BTC", "Memo", "Hash", "https://trustwallet.com"};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "2a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c78777037306437760231300342544303425443044d656d6f04486173681768747470733a2f2f747275737477616c6c65742e636f6d0000000000");
    }
    {
        // empty struct
        NewFundsContent newFunds{"", "", "", "", "", "", ""};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "000000000000000000000000");
    }
    {
        // test from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-fio.test.ts
        NewFundsContent newFunds{"purse.alice", "1", "", "fio.reqobt", "", "", ""};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "0b70757273652e616c6963650131000a66696f2e7265716f62740000000000000000");
    }
}

TEST(FIONewFundsContent, deserialize) {
    {
        const Data ser = parse_hex("2a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c78777037306437760231300342544303425443044d656d6f04486173681768747470733a2f2f747275737477616c6c65742e636f6d0000000000");
        size_t index = 0;
        const auto newFunds = NewFundsContent::deserialize(ser, index);
        EXPECT_EQ(newFunds.payeePublicAddress, "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
        EXPECT_EQ(newFunds.amount, "10");
        EXPECT_EQ(newFunds.coinSymbol, "BTC");
        EXPECT_EQ(newFunds.offlineUrl, "https://trustwallet.com");
    }
    {
        // incomplete input
        const Data ser = parse_hex("0b6d");
        size_t index = 0;
        const auto newFunds = NewFundsContent::deserialize(ser, index);
        EXPECT_EQ(newFunds.payeePublicAddress, "");
        EXPECT_EQ(newFunds.amount, "");
        EXPECT_EQ(newFunds.offlineUrl, "");
    }
}

TEST(FIOTransactionBuilder, expirySetDefault) {
    uint32_t expiry = 1579790000;
    EXPECT_EQ(TransactionBuilder::expirySetDefaultIfNeeded(expiry), false);
    EXPECT_EQ(expiry, 1579790000ul);
    expiry = 0;
    EXPECT_EQ(expiry, 0ul);
    EXPECT_EQ(TransactionBuilder::expirySetDefaultIfNeeded(expiry), true);
    EXPECT_TRUE(expiry > 1579790000);
}

// May throw nlohmann::json::type_error
void createTxWithChainParam(const ChainParams& paramIn, ChainParams& paramOut) {
    string tx = TransactionBuilder::createAddPubAddress(gAddr6M, gPrivKeyBA, "adam@fiotestnet", {{"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"}},
                                                        paramIn, 0, "rewards@wallet", 1579729429);
    // retrieve chain params from encoded tx; parse out packed tx
    try {
        nlohmann::json txJson = nlohmann::json::parse(tx);
        Data txData = parse_hex(txJson.at("packed_trx").get<std::string>());
        // decode values
        ASSERT_TRUE(txData.size() >= 10);
        paramOut.headBlockNumber = decode16LE(txData.data() + 4);
        paramOut.refBlockPrefix = decode32LE(txData.data() + 4 + 2);
    } catch (nlohmann::json::type_error& e) {
        FAIL() << "Json parse error " << e.what();
    }
}

void checkBlockNum(uint64_t blockNumIn, uint64_t blockNumExpected) {
    ChainParams paramOut;
    createTxWithChainParam(ChainParams{chainId, blockNumIn, 4281229859}, paramOut);
    EXPECT_EQ(paramOut.headBlockNumber, blockNumExpected);
}

void checkRefBlockPrefix(uint64_t blockPrefixIn, uint64_t blockPrefixExpected) {
    ChainParams paramOut;
    createTxWithChainParam(ChainParams{chainId, 11565, blockPrefixIn}, paramOut);
    EXPECT_EQ(paramOut.refBlockPrefix, blockPrefixExpected);
}

TEST(FIOTransactionBuilder, chainParansRange) {
    // headBlockNumber, 2 bytes
    checkBlockNum(101, 101);
    checkBlockNum(0xFFFF, 0xFFFF);
    checkBlockNum(0x00011234, 0x1234);
    // large values truncated
    checkBlockNum(0xFFAB1234, 0x1234);
    checkBlockNum(0x0000000112345678, 0x5678);
    checkBlockNum(0xFFABCDEF12345678, 0x5678);

    // refBlockPrefix, 4 bytes; Large refBlockPrefix values used to cause problem
    checkRefBlockPrefix(101, 101);
    checkRefBlockPrefix(4281229859, 4281229859);
    checkRefBlockPrefix(0xFFFFFFFF, 0xFFFFFFFF);
    // large values truncated
    checkRefBlockPrefix(0x0000000112345678, 0x12345678);
    checkRefBlockPrefix(0xFFABCDEF12345678, 0x12345678);
}

TEST(FIOTransactionBuilder, encodeVarInt) {
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x11, data), 1);
        EXPECT_EQ(hex(data), "11");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x7F, data), 1);
        EXPECT_EQ(hex(data), "7f");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x80, data), 2);
        EXPECT_EQ(hex(data), "8001");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0xFF, data), 2);
        EXPECT_EQ(hex(data), "ff01");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x100, data), 2);
        EXPECT_EQ(hex(data), "8002");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x3FFF, data), 2);
        EXPECT_EQ(hex(data), "ff7f");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0x4000, data), 3);
        EXPECT_EQ(hex(data), "808001");
    }
    {
        Data data;
        EXPECT_EQ(TW::FIO::encodeVarInt(0xFFFFFFFF, data), 5);
        EXPECT_EQ(hex(data), "ffffffff0f");
    }
}

TEST(FIOTransactionBuilder, encodeString) {
    {
        Data data;
        const string text = "ABC";
        TW::FIO::encodeString(text, data);
        EXPECT_EQ(hex(data), "03" + hex(text));
    }
    {
        Data data;
        const string text = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
        EXPECT_EQ(text.length(), 130ul);
        TW::FIO::encodeString(text, data);
        // length on 2 bytes
        EXPECT_EQ(hex(data), "8201" + hex(text));
    }
}

TEST(FIOTransactionBuilder, buildUnsignedTxBytes) {
    {
        // Test register_fio_address_message
        Proto::SigningInput input;
        
        input.set_expiry(1579784511);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(39881);
        input.mutable_chain_params()->set_ref_block_prefix(4279583376);
        
        input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
        input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(gAddr6M.string());
        input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);

        auto result = TransactionBuilder::buildUnsignedTxBytes(input);
        //packed_trx: 3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400
        EXPECT_EQ("3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400"
        , hex(result));
    }
    {
        // Test add_pub_address_message
        Proto::SigningInput input;
        
        input.set_expiry(1579729429);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());
        
        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(11565);
        input.mutable_chain_params()->set_ref_block_prefix(4281229859);
        
        input.mutable_action()->mutable_add_pub_address_message()->set_fee(0);
        input.mutable_action()->mutable_add_pub_address_message()->set_fio_address("adam@fiotestnet");

        TW::FIO::Proto::PublicAddress *value;
        value = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value->set_address("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
        value->set_coin_symbol("BTC");
        TW::FIO::Proto::PublicAddress *value1;
        value1 = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value1->set_address("0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51");
        value1->set_coin_symbol("ETH");
        TW::FIO::Proto::PublicAddress *value2;
        value2 = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value2->set_address("bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s");
        value2->set_coin_symbol("BNB");

        auto result = TransactionBuilder::buildUnsignedTxBytes(input);
        
        EXPECT_EQ("15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400"
        , hex(result));
    }

    {
        // Test transfer_message
        Proto::SigningInput input;
        input.set_expiry(1579790000);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(50000);
        input.mutable_chain_params()->set_ref_block_prefix(4000123456);

        input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
        input.mutable_action()->mutable_transfer_message()->set_fee(250000000);
        input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");
        
        auto result = TransactionBuilder::buildUnsignedTxBytes(input);
        EXPECT_EQ("b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400"
        , hex(result));
    }

    {
        // Test renew_fio_address_message
        Proto::SigningInput input;
        input.set_expiry(1579785000);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(39881);
        input.mutable_chain_params()->set_ref_block_prefix(4279583376);

        input.mutable_action()->mutable_renew_fio_address_message()->set_fee(3000000000);
        input.mutable_action()->mutable_renew_fio_address_message()->set_fio_address("nick@fiotestnet");
        
        auto result = TransactionBuilder::buildUnsignedTxBytes(input);
        EXPECT_EQ("289b295ec99b904215ff0000000001003056372503a85b80b1ba2919aea6ba01102b2f46fca756b200000000a8ed32322f0f6e69636b4066696f746573746e6574005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400"
        , hex(result));
    }

}

TEST(FIOTransactionBuilder, buildSigningOutput) {
    {
        // Test register_fio_address_message
        Proto::SigningInput input;
        
        input.set_expiry(1579784511);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(39881);
        input.mutable_chain_params()->set_ref_block_prefix(4279583376);
        
        input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
        input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(gAddr6M.string());
        input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);

        Data txBytes = TransactionBuilder::buildUnsignedTxBytes(input);
        Data sigBuf = TransactionBuilder::buildPreSignTxData(chainId, txBytes);
        // create signature
        Data signature = Signer::signData(gPrivKeyBA, sigBuf);
        
        Proto::SigningOutput result = TransactionBuilder::buildSigningOutput(input, signature);

        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})"
        , result.json());

    }   
    {
        // Test add_pub_address_message
        Proto::SigningInput input;
        
        input.set_expiry(1579729429);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());
        
        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(11565);
        input.mutable_chain_params()->set_ref_block_prefix(4281229859);
        
        input.mutable_action()->mutable_add_pub_address_message()->set_fee(0);
        input.mutable_action()->mutable_add_pub_address_message()->set_fio_address("adam@fiotestnet");

        TW::FIO::Proto::PublicAddress *value;
        value = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value->set_address("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
        value->set_coin_symbol("BTC");
        TW::FIO::Proto::PublicAddress *value1;
        value1 = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value1->set_address("0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51");
        value1->set_coin_symbol("ETH");
        TW::FIO::Proto::PublicAddress *value2;
        value2 = input.mutable_action()->mutable_add_pub_address_message()->add_public_addresses();
        value2->set_address("bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s");
        value2->set_coin_symbol("BNB");

        Data txBytes = TransactionBuilder::buildUnsignedTxBytes(input);
        Data sigBuf = TransactionBuilder::buildPreSignTxData(chainId, txBytes);
        // create signature
        Data signature = Signer::signData(gPrivKeyBA, sigBuf);
    
        Proto::SigningOutput result = TransactionBuilder::buildSigningOutput(input, signature);
        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K3zimaMKU8cBhVRPw46KM2u7uQWaAKXrnoeYZ7MEbp6sVJcDQmQR2RtdavpUPwkAnYUkd8NqLun8H48tcxZBcTUgkiPGVJ"]})"
        , result.json());
    }
    {
        // Test transfer_message
        Proto::SigningInput input;
        input.set_expiry(1579790000);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(50000);
        input.mutable_chain_params()->set_ref_block_prefix(4000123456);

        input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
        input.mutable_action()->mutable_transfer_message()->set_fee(250000000);
        input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");

        Data txBytes = TransactionBuilder::buildUnsignedTxBytes(input);
        Data sigBuf = TransactionBuilder::buildPreSignTxData(chainId, txBytes);
        // create signature
        Data signature = Signer::signData(gPrivKeyBA, sigBuf);
    
        Proto::SigningOutput result = TransactionBuilder::buildSigningOutput(input, signature);
        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})"
        , result.json());
    }
    {   
        // Test renew_fio_address_message
        Proto::SigningInput input;
        input.set_expiry(1579785000);
        input.set_tpid("rewards@wallet");
        input.set_owner_public_key(gAddr6M.string());

        input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
        input.mutable_chain_params()->set_head_block_number(39881);
        input.mutable_chain_params()->set_ref_block_prefix(4279583376);

        input.mutable_action()->mutable_renew_fio_address_message()->set_fee(3000000000);
        input.mutable_action()->mutable_renew_fio_address_message()->set_fio_address("nick@fiotestnet");
    
        Data txBytes = TransactionBuilder::buildUnsignedTxBytes(input);
        Data sigBuf = TransactionBuilder::buildPreSignTxData(chainId, txBytes);
        // create signature
        Data signature = Signer::signData(gPrivKeyBA, sigBuf);
    
        Proto::SigningOutput result = TransactionBuilder::buildSigningOutput(input, signature);
        EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff0000000001003056372503a85b80b1ba2919aea6ba01102b2f46fca756b200000000a8ed32322f0f6e69636b4066696f746573746e6574005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_Jxz7oCJ7Z4ECsxqb2utqBcyP3zPQCeQCBws9wWQjyptUKoWVk2AyCVEqtdMHJwqtLniio5Z7npMnaZB8E4pa2G75P9uGkb"]})"
        , result.json());
    }
}

} // namespace TW::FIO::TransactionBuilderTests
