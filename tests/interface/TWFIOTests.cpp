// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWFIOSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "proto/FIO.pb.h"
#include "FIO/Address.h"
#include "Data.h"
#include "TWTestUtilities.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FIO;
using namespace std;


TEST(TWFIO, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false);
    ASSERT_NE(nullptr, publicKey);
    ASSERT_EQ(65, publicKey->impl.bytes.size());
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey, TWCoinTypeFIO));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf");

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf").get(), TWCoinTypeFIO));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));
    assertStringsEqual(address2String, "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf");

    ASSERT_TRUE(TWAnyAddressEqual(address.get(), address2.get()));
}

const Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
// 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
const PrivateKey privKeyBA = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
const PublicKey pubKey6M = privKeyBA.getPublicKey(TWPublicKeyTypeSECP256k1);
const Address addr6M(pubKey6M);

TEST(TWFIO, RegisterFioAddress) {
    Proto::SigningInput input;
    input.set_expiry(1579784511);
    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
    input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(addr6M.string());
    input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);
    input.mutable_action()->mutable_register_fio_address_message()->set_tpid("rewards@wallet");

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const TW::byte *)inputString.data(), inputString.size());

    TW_FIO_Proto_SigningOutput outputData = TWFIOSignerSign(inputData);
    auto out = Proto::SigningOutput();
    ASSERT_TRUE(out.ParseFromArray(TWDataBytes(outputData), TWDataSize(outputData)));
    EXPECT_EQ("", out.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", out.json());
}

TEST(TWFIO, AddPubAddress) {
    Proto::SigningInput input;
    input.set_expiry(1579729429);
    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(11565);
    input.mutable_chain_params()->set_ref_block_prefix(4281229859);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    auto action = input.mutable_action()->mutable_add_pub_address_message();
    action->set_fio_address("adam@fiotestnet");
    action->add_public_addresses();
    action->add_public_addresses();
    action->add_public_addresses();
    action->mutable_public_addresses(0)->set_token_code("BTC");
    action->mutable_public_addresses(0)->set_address("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
    action->mutable_public_addresses(1)->set_token_code("ETH");
    action->mutable_public_addresses(1)->set_address("0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51");
    action->mutable_public_addresses(2)->set_token_code("BNB");
    action->mutable_public_addresses(2)->set_address("bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s");
    action->set_fee(0);
    action->set_tpid("rewards@wallet");

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const TW::byte *)inputString.data(), inputString.size());

    TW_FIO_Proto_SigningOutput outputData = TWFIOSignerSign(inputData);
    auto out = Proto::SigningOutput();
    ASSERT_TRUE(out.ParseFromArray(TWDataBytes(outputData), TWDataSize(outputData)));
    EXPECT_EQ("", out.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232bd010f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K85BxXzJwvjPs3mFeKatWSjBHuMXTw634RRtf6ZMytpzLCdpHcJ7CQWPeXJvwm7aoz7XJJKapmoT4jzCLoVBv2cxP149Bx"]})", out.json());
}

TEST(TWFIO, Transfer) {
    Proto::SigningInput input;
    input.set_expiry(1579790000);
    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(50000);
    input.mutable_chain_params()->set_ref_block_prefix(4000123456);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");
    input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
    input.mutable_action()->mutable_transfer_message()->set_fee(250000000);
    input.mutable_action()->mutable_transfer_message()->set_tpid("rewards@wallet");

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const TW::byte *)inputString.data(), inputString.size());

    TW_FIO_Proto_SigningOutput outputData = TWFIOSignerSign(inputData);
    auto out = Proto::SigningOutput();
    ASSERT_TRUE(out.ParseFromArray(TWDataBytes(outputData), TWDataSize(outputData)));
    EXPECT_EQ("", out.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})", out.json());
}
