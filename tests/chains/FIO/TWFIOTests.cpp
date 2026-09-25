// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
const Data gChainIdMainnet = parse_hex("21dcae42c0182200e93f954a074011f9048a7624c6fe81d3c9541a614a88bd1c");
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
    EXPECT_EQ(output.action_name(), "regaddress");
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
    EXPECT_EQ(output.action_name(), "addaddress");
}

TEST(TWFIO, RemovePubAddress) {
    auto privateKey = parse_hex("93083dc4d9e8f613a57e3a862a1fa5d665c5e90141a8428990c945d1c2b56491");

    Proto::SigningInput input;
    input.set_expiry(1713269931);
    input.mutable_chain_params()->set_chain_id(string(gChainIdMainnet.begin(), gChainIdMainnet.end()));
    input.mutable_chain_params()->set_head_block_number(256054093);
    input.mutable_chain_params()->set_ref_block_prefix(2438027034);
    input.set_private_key(string(privateKey.begin(), privateKey.end()));
    input.set_tpid("trust@fiomembers");
    auto action = input.mutable_action()->mutable_remove_pub_address_message();
    action->set_fio_address("sergeitrust@wallet");
    action->add_public_addresses();
    action->mutable_public_addresses(0)->set_coin_symbol("BTC");
    action->mutable_public_addresses(0)->set_address("bc1q68caps3gqt2c9qxtnkhmzf3whxenrs9cav4wlm");
    action->set_fee(4878336459);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    std::cout << output.json() << std::endl;
    // Successfully broadcasted: https://fio.bloks.io/transaction/0bb6da24a3ea9e3ee57906de1cfa8bad18709acd64bf30908713dd61c54cfaea
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"ab6c1e664d131a5751910000000001003056372503a85b0000c6eaa664a4ba01b038b9d6c13372f700000000a8ed3232681273657267656974727573744077616c6c65740103425443034254432a62633171363863617073336771743263397178746e6b686d7a6633776878656e72733963617634776c6dcb81c52201000000b038b9d6c13372f71074727573744066696f6d656d6265727300","signatures":["SIG_K1_K3cKHXCFYYB9aLFc9qk2idmWgEA4Q9192fECc3cF7MYHXkw9kZamdeHv3qbVoifG9oS8h6nVAJwJvj5YcnhHmnd3u89ND7"]})", output.json());
    EXPECT_EQ(output.action_name(), "remaddress");
}

TEST(TWFIO, RemoveAllPubAddresses) {
    auto privateKey = parse_hex("93083dc4d9e8f613a57e3a862a1fa5d665c5e90141a8428990c945d1c2b56491");

    Proto::SigningInput input;
    input.set_expiry(1713458993);
    input.mutable_chain_params()->set_chain_id(string(gChainIdMainnet.begin(), gChainIdMainnet.end()));
    input.mutable_chain_params()->set_head_block_number(256432311);
    input.mutable_chain_params()->set_ref_block_prefix(2287536876);
    input.set_private_key(string(privateKey.begin(), privateKey.end()));
    input.set_tpid("trust@fiomembers");
    auto action = input.mutable_action()->mutable_remove_all_pub_addresses_message();
    action->set_fio_address("sergeitrust@wallet");
    action->set_fee(0);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    // Successfully broadcasted: https://fio.bloks.io/transaction/f2facdebfcba1981377537424a6d7b7e7ebd8222c87ba4d25a480d1b968704b2
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"314f2166b7d8ec0a59880000000001003056372503a85b00c04dc9c468a4ba01b038b9d6c13372f700000000a8ed3232341273657267656974727573744077616c6c65740000000000000000b038b9d6c13372f71074727573744066696f6d656d6265727300","signatures":["SIG_K1_KXXtpz7NWhzCms7Dj54nSwwtCw6w4zLCyTLxs3tqqgLscrz91cMjcbN4yxcySvZ7t4MER8HPteeJZUnR16uLyDa1gFGzrx"]})", output.json());
    EXPECT_EQ(output.action_name(), "remalladdr");
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
    EXPECT_EQ(output.action_name(), "trnsfiopubky");
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
    EXPECT_EQ(output.action_name(), "renewaddress");
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
    EXPECT_EQ(output.action_name(), "newfundsreq");
}

TEST(TWFIO, AddBundledTransactions) {
    auto privateKey = parse_hex("93083dc4d9e8f613a57e3a862a1fa5d665c5e90141a8428990c945d1c2b56491");

    Proto::SigningInput input;
    input.set_expiry(1713458594);
    input.mutable_chain_params()->set_chain_id(string(gChainIdMainnet.begin(), gChainIdMainnet.end()));
    input.mutable_chain_params()->set_head_block_number(256431437);
    input.mutable_chain_params()->set_ref_block_prefix(791306279);
    input.set_private_key(string(privateKey.begin(), privateKey.end()));
    input.set_tpid("trust@fiomembers");
    auto action = input.mutable_action()->mutable_add_bundled_transactions_message();
    action->set_fio_address("sergeitrust@wallet");
    action->set_bundle_sets(1);
    action->set_fee(100000000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeFIO);
    EXPECT_EQ(Common::Proto::OK, output.error());
    // Successfully broadcasted: https://fio.bloks.io/transaction/2c00f2051ca3738c4fe03ceddb82c48fefd9c534d8bb793dc7dce5d12f4f4f9c
    EXPECT_EQ(R"({"compression":"none","packed_context_free_data":"","packed_trx":"a24d21664dd527602a2f0000000001003056372503a85b000056314d7d523201b038b9d6c13372f700000000a8ed32323c1273657267656974727573744077616c6c6574010000000000000000e87648170000001074727573744066696f6d656d62657273b038b9d6c13372f700","signatures":["SIG_K1_KjWGZ4Yd48VJcTAgox3HYVQhXeLhpRCgz2WqiF5WHRFSnbHouKxPgLQmymoABHC8EX51G1jU4ocWg2RKU17UYm4L5kTXP6"]})", output.json());
    EXPECT_EQ(output.action_name(), "addbundles");
}

} // namespace TW::FIO::TWFIOTests
