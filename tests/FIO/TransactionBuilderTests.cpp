// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Action.h"
#include "FIO/Transaction.h"
#include "FIO/TransactionBuilder.h"
#include "FIO/NewFundsRequest.h"

#include "HexCoding.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::FIO;
using namespace std;


const Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
// 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
const PrivateKey privKeyBA = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
const PublicKey pubKey6M = privKeyBA.getPublicKey(TWPublicKeyTypeSECP256k1);
const Address addr6M(pubKey6M);

TEST(FIOTransactionBuilder, RegisterFioAddressGeneric) {
    Proto::SigningInput input;
    input.set_expiry(1579784511);
    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
    input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(addr6M.string());
    input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);

    auto json = TransactionBuilder::sign(input);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", json);
}

TEST(FIOTransactionBuilder, RegisterFioAddress) {
    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 5000000000;

    string t = TransactionBuilder::createRegisterFioAddress(addr6M, privKeyBA, "adam@fiotestnet",
        chainParams, fee, "rewards@wallet", 1579784511);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", t);
}

TEST(FIOTransactionBuilder, AddPubAddress) {
    ChainParams chainParams{chainId, 11565, 4281229859};

    string t = TransactionBuilder::createAddPubAddress(addr6M, privKeyBA, "adam@fiotestnet", {
        {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"},
        {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"},
        {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
        chainParams, 0, "rewards@wallet", 1579729429);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232bd010f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K85BxXzJwvjPs3mFeKatWSjBHuMXTw634RRtf6ZMytpzLCdpHcJ7CQWPeXJvwm7aoz7XJJKapmoT4jzCLoVBv2cxP149Bx"]})", t);
}

TEST(FIOTransactionBuilder, Transfer) {
    ChainParams chainParams{chainId, 50000, 4000123456};
    string payee = "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE";
    uint64_t amount = 1000000000;
    uint64_t fee = 250000000;

    string t = TransactionBuilder::createTransfer(addr6M, privKeyBA, payee, amount,
        chainParams, fee, "rewards@wallet", 1579790000);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})", t);
}

TEST(FIOTransactionBuilder, RenewFioAddress) {
    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 3000000000;

    string t = TransactionBuilder::createRenewFioAddress(addr6M, privKeyBA, "nick@fiotestnet",
        chainParams, fee, "rewards@wallet", 1579785000);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff0000000001003056372503a85b80b1ba2919aea6ba01102b2f46fca756b200000000a8ed32322f0f6e69636b4066696f746573746e6574005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_Jxz7oCJ7Z4ECsxqb2utqBcyP3zPQCeQCBws9wWQjyptUKoWVk2AyCVEqtdMHJwqtLniio5Z7npMnaZB8E4pa2G75P9uGkb"]})", t);
}

TEST(FIOTransactionBuilder, NewFundsRequest) {
    ChainParams chainParams{chainId, 39881, 4279583376};
    uint64_t fee = 3000000000;

    const Data iv = parse_hex("000102030405060708090a0b0c0d0e0f"); // use fixed iv for testability
    string t = TransactionBuilder::createNewFundsRequest(addr6M, privKeyBA,
        "mario@fiotestnet", "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o", "alice@fiotestnet",
        "5", "BNB", "Memo", "Hash", "https://trustwallet.com",
        chainParams, fee, "rewards@wallet", 1579785000, iv);

    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff000000000100403ed4aa0ba85b00acba384dbdb89a01102b2f46fca756b200000000a8ed3232fd2201106d6172696f4066696f746573746e657410616c6963654066696f746573746e6574e03030303130323033303430353036303730383039306130623063306430653066376530323532643962326137396662373631373831613638633030643939306336633033306131313164366234306161633131393833323639323530386264343533396562303463633636306563363166643937636338393732376437386534613334383566653064353862613537336237613438633066326232636437636532653236333431336335626138366135386366373732303861613639363234363532386262346133326466326436363136353664343162626232383566353837005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K8ShtXKoUsXfAnvivzZkL9dysJtSutdeohmvfxoabm3C9mhyZjzEiPKUVKNq7YqSUgo7ZaK13w2iyk2LwnFZkbZKP79WqG"]})", t);
}

TEST(FIOTransaction, ActionRegisterFioAddressInternal) {
    RegisterFioAddressData radata("adam@fiotestnet", addr6M.string(),
        5000000000, "rewards@wallet", "qdfejz2a5wpl");
    Data ser1;
    radata.serialize(ser1);
    EXPECT_EQ(
        hex(parse_hex("0F6164616D4066696F746573746E65743546494F366D31664D645470526B52426E6564765973685843784C4669433573755255384B44667838787874587032686E7478706E6600F2052A01000000102B2F46FCA756B20E726577617264734077616C6C6574")),
        hex(ser1));

    Action raAction;
    raAction.account = "fio.address";
    raAction.name = "regaddress";
    raAction.includeExtra01BeforeData = false;
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
    AddPubAddressData aadata("adam@fiotestnet", {
        {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"},
        {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"},
        {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
        0, "rewards@wallet", "qdfejz2a5wpl");
    Data ser1;
    aadata.serialize(ser1);
    EXPECT_EQ(
        "0f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c6574",
        hex(ser1));

    Action aaAction;
    aaAction.account = "fio.address";
    aaAction.name = "addaddress";
    aaAction.includeExtra01BeforeData = true;
    aaAction.actionDataSer = ser1;
    aaAction.auth.authArray.push_back(Authorization{"qdfejz2a5wpl", "active"});
    Data ser2;
    aaAction.serialize(ser2);
    EXPECT_EQ(
        "003056372503a85b0000c6eaa66452320"
        "1102b2f46fca756b200000000a8ed3232"
        "bd01"
        "0f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400",
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
        "bd01"
        "0f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400",
        hex(ser3));
}

TEST(FIONewFundsContent, serialize) {
    {
        NewFundsContent newFunds {"mario@trust", "10", "BNB", "Memo", "Hash", "https://trustwallet.com"};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "0b6d6172696f40747275737402313003424e42044d656d6f04486173681768747470733a2f2f747275737477616c6c65742e636f6d");
    }
    {
        // empty struct 
        NewFundsContent newFunds {"", "", "", "", "", ""};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "000000000000");
    }
    {
        // test from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-fio.test.ts
        NewFundsContent newFunds {"purse.alice", "1", "fio.reqobt", "", "", ""};
        Data ser;
        newFunds.serialize(ser);
        EXPECT_EQ(hex(ser), "0b70757273652e616c69636501310a66696f2e7265716f6274000000");
    }
}

TEST(FIONewFundsContent, deserialize) {
    {
        const Data ser = parse_hex("0b6d6172696f40747275737402313003424e42044d656d6f001768747470733a2f2f747275737477616c6c65742e636f6d");
        size_t index = 0;
        const auto newFunds = NewFundsContent::deserialize(ser, index);
        EXPECT_EQ(newFunds.payeeFioName, "mario@trust");
        EXPECT_EQ(newFunds.amount, "10");
        EXPECT_EQ(newFunds.offlineUrl, "https://trustwallet.com");
    }
    {
        // incomplete inpiut
        const Data ser = parse_hex("0b6d");
        size_t index = 0;
        const auto newFunds = NewFundsContent::deserialize(ser, index);
        EXPECT_EQ(newFunds.payeeFioName, "");
        EXPECT_EQ(newFunds.amount, "");
        EXPECT_EQ(newFunds.offlineUrl, "");
    }
}

TEST(FIOTransactionBuilder, expirySetDefault) {
    uint32_t expiry = 1579790000;
    EXPECT_EQ(TransactionBuilder::expirySetDefaultIfNeeded(expiry), false);
    EXPECT_EQ(expiry, 1579790000);
    expiry = 0;
    EXPECT_EQ(expiry, 0);
    EXPECT_EQ(TransactionBuilder::expirySetDefaultIfNeeded(expiry), true);
    EXPECT_TRUE(expiry > 1579790000);
}