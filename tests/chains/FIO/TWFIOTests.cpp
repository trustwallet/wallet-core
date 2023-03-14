// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "proto/FIO.pb.h"
#include "FIO/Address.h"
#include "Data.h"
#include "TestUtilities.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::FIO::TWFIOTests {

using namespace std;

TEST(TWFIO, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false));
    ASSERT_NE(nullptr, publicKey.get());
    ASSERT_EQ(65ul, publicKey.get()->impl.bytes.size());
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeFIO));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf");

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf").get(), TWCoinTypeFIO));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));
    assertStringsEqual(address2String, "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf");

    ASSERT_TRUE(TWAnyAddressEqual(address.get(), address2.get()));
}

const Data gChainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
// 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
const PrivateKey privKeyBA = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
const PublicKey pubKey6M = privKeyBA.getPublicKey(TWPublicKeyTypeSECP256k1);
const Address addr6M(pubKey6M);

TEST(TWFIO, RegisterFioAddress) {
    Proto::SigningInput input;
    input.set_expiry(1579784511);
    input.mutable_chain_params()->set_chain_id(string(gChainId.begin(), gChainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_register_fio_address_message()->set_fio_address("adam@fiotestnet");
    input.mutable_action()->mutable_register_fio_address_message()->set_owner_fio_public_key(addr6M.string());
    input.mutable_action()->mutable_register_fio_address_message()->set_fee(5000000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"3f99295ec99b904215ff0000000001003056372503a85b0000c6eaa66498ba01102b2f46fca756b200000000a8ed3232650f6164616d4066696f746573746e65743546494f366d31664d645470526b52426e6564765973685843784c4669433573755255384b44667838787874587032686e7478706e6600f2052a01000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K19ugLriG3ApYgjJCRDsy21p9xgsjbDtqBuZrmAEix9XYzndR1kNbJ6fXCngMJMAhxUHfwHAsPnh58otXiJZkazaM1EkS5"]})", output.json());
}

TEST(TWFIO, AddPubAddress) {
    Proto::SigningInput input;
    input.set_expiry(1579729429);
    input.mutable_chain_params()->set_chain_id(string(gChainId.begin(), gChainId.end()));
    input.mutable_chain_params()->set_head_block_number(11565);
    input.mutable_chain_params()->set_ref_block_prefix(4281229859);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    auto action = input.mutable_action()->mutable_add_pub_address_message();
    action->set_fio_address("adam@fiotestnet");
    action->add_public_addresses();
    action->add_public_addresses();
    action->add_public_addresses();
    action->mutable_public_addresses(0)->set_coin_symbol("BTC");
    action->mutable_public_addresses(0)->set_address("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
    action->mutable_public_addresses(1)->set_coin_symbol("ETH");
    action->mutable_public_addresses(1)->set_address("0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51");
    action->mutable_public_addresses(2)->set_coin_symbol("BNB");
    action->mutable_public_addresses(2)->set_address("bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s");
    action->set_fee(0);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232c9010f6164616d4066696f746573746e65740303425443034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c787770373064377603455448034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e4203424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K3zimaMKU8cBhVRPw46KM2u7uQWaAKXrnoeYZ7MEbp6sVJcDQmQR2RtdavpUPwkAnYUkd8NqLun8H48tcxZBcTUgkiPGVJ"]})", output.json());
}

TEST(TWFIO, Transfer) {
    Proto::SigningInput input;
    input.set_expiry(1579790000);
    input.mutable_chain_params()->set_chain_id(string(gChainId.begin(), gChainId.end()));
    input.mutable_chain_params()->set_head_block_number(50000);
    input.mutable_chain_params()->set_ref_block_prefix(4000123456);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");
    input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
    input.mutable_action()->mutable_transfer_message()->set_fee(250000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})", output.json());
}

TEST(TWFIO, RenewFioAddress) {
    Proto::SigningInput input;
    input.set_expiry(1579785000);
    input.mutable_chain_params()->set_chain_id(string(gChainId.begin(), gChainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_renew_fio_address_message()->set_fio_address("nick@fiotestnet");
    input.mutable_action()->mutable_renew_fio_address_message()->set_owner_fio_public_key(addr6M.string());
    input.mutable_action()->mutable_renew_fio_address_message()->set_fee(3000000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff0000000001003056372503a85b80b1ba2919aea6ba01102b2f46fca756b200000000a8ed32322f0f6e69636b4066696f746573746e6574005ed0b200000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_Jxz7oCJ7Z4ECsxqb2utqBcyP3zPQCeQCBws9wWQjyptUKoWVk2AyCVEqtdMHJwqtLniio5Z7npMnaZB8E4pa2G75P9uGkb"]})", output.json());
}

TEST(TWFIO, NewFundsRequest) {
    Proto::SigningInput input;
    input.set_expiry(1579785000);
    input.mutable_chain_params()->set_chain_id(string(gChainId.begin(), gChainId.end()));
    input.mutable_chain_params()->set_head_block_number(39881);
    input.mutable_chain_params()->set_ref_block_prefix(4279583376);
    input.set_private_key(string(privKeyBA.bytes.begin(), privKeyBA.bytes.end()));
    input.set_tpid("rewards@wallet");
    input.mutable_action()->mutable_new_funds_request_message()->set_payer_fio_name("mario@fiotestnet");
    input.mutable_action()->mutable_new_funds_request_message()->set_payer_fio_address("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
    input.mutable_action()->mutable_new_funds_request_message()->set_payee_fio_name("alice@fiotestnet");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_payee_public_address("bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_amount("5");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_coin_symbol("BTC");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_memo("Memo");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_hash("Hash");
    input.mutable_action()->mutable_new_funds_request_message()->mutable_content()->set_offline_url("https://trustwallet.com");
    input.mutable_action()->mutable_new_funds_request_message()->set_fee(3000000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    // Packed transaction varies, as there is no way to control encryption IV parameter from this level.
    // Therefore full equality cannot be checked, tail is cut off.  The first N chars are checked, works in this case.
    EXPECT_EQ(
        R"({"compression":"none","packed_context_free_data":"","packed_trx":"289b295ec99b904215ff000000000100403ed4aa0ba85b00acba384dbdb89a01102b2f46fca756b200000000a8ed32328802106d6172696f4066696f746573746)",
        output.json().substr(0, 195));
}

} // namespace TW::FIO::TWFIOTests
