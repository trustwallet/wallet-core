// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Substrate/Signer.h"
#include "Data.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "proto/Substrate.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Substrate::tests {

Proto::SigningInput buildSigningInput() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9a4283cc38f7e769c53ad2d1c5cf292fc85a740ec1c1aa80c180847e51928650

    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
    auto genesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(1ul);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4298130ul);
    era->set_period(64ul);

    auto* transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_module_index(0x05);
    transfer->set_method_index(0x01);
    transfer->set_to_address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");

    auto value = store(1000000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");
    return input;
}

TEST(SubstrateSigner, signaturePreImage) {
    auto input = buildSigningInput();
    auto preImage = Signer::signaturePreImage(input);
    ASSERT_EQ(hex(preImage), "050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f2050414444454420574954482053504143455300000000000000000025010400c20b0000020000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
}

TEST(SubstrateSigner, encodeTransaction) {
    auto input = buildSigningInput();
    auto publicKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("0791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "bd0284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee000791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f25010400050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f20504144444544205749544820535041434553000000000000000000");
}

TEST(SubstrateSigner, encodeTransaction_Add_authorization) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x7d9b9109027b36b72d37ba0648cb70e5254524d3d6752cc6b41601f4bdfb1af0

    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("ce0c2109db498e45abf8fd447580dcfa7b7a07ffc2bfb1a0fbdd1af3e8816d2b");
    auto genesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(5ul);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4395451ul);
    era->set_period(64ul);

    auto* ji = input.mutable_polymesh_call()->mutable_authorization_call()->mutable_join_identity();
    ji->set_module_index(0x07);
    ji->set_method_index(0x0d);
    ji->set_target("2HEVN4PHYKj7B1krQ9bctAQXZxHQQkANVNCcfbdYk2gZ4cBR");

    auto publicKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("81e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "490284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee0081e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01b5031400070d01d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d0610540501000100010000");
}

TEST(SubstrateSigner, encodeTransaction_JoinIdentityAsKey) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9d7297d8b38af5668861996cb115f321ed681989e87024fda64eae748c2dc542

    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("45c80153c47f5d16acc7a66d473870e8d4574437a7d8c813f47da74cae3812c2");
    auto genesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(0ul);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4395527ul);
    era->set_period(64ul);

    auto* key = input.mutable_polymesh_call()->mutable_identity_call()->mutable_join_identity_as_key();
    key->set_module_index(0x07);
    key->set_method_index(0x05);
    key->set_auth_id(21435);

    auto publicKey = parse_hex("d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054");
    auto signature = parse_hex("7f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "c5018400d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054007f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006750000000705bb53000000000000");
}

TEST(SubstrateSigner, Statemint_encodeTransaction_transfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686030-2

    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto genesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(0ul);
    input.set_spec_version(9320u);
    input.set_transaction_version(9u);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_module_index(0x32);
    transfer->set_method_index(0x05);
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(0x00); // native token

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "4102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000003205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(SubstrateSigner, Statemint_encodeTransaction_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686081-2

    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("e8f10f9a841dc73578148c763afa17638670c8655542172a80af2e03bf3cbe62");
    auto genesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(2ul);
    input.set_spec_version(9320u);
    input.set_transaction_version(9u);

    auto* era = input.mutable_era();
    era->set_block_number(2686056ul);
    era->set_period(64ul);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_module_index(0x32);
    transfer->set_method_index(0x06);
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(0x00); // native token

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("68c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "4502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c910068c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a85020800003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(SubstrateSigner, Statemint_encodeTransaction_batch_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2711054-2

    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("c8a2e9492f822f8c07f3717a00e36f68a3090a878b07998724ec1f178f4cf514");
    auto genesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(3ul);
    input.set_spec_version(9320u);
    input.set_transaction_version(9u);

    auto* era = input.mutable_era();
    era->set_block_number(2711016ul);
    era->set_period(64ul);

    auto* transfer = input.mutable_balance_call()->mutable_batch_asset_transfer();
    transfer->set_module_index(0x28);
    transfer->set_method_index(0x00);
    transfer->set_fee_asset_id(0x00);

    auto* t1 = transfer->add_transfers();
    t1->set_module_index(0x32);
    t1->set_method_index(0x06);
    t1->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");
    auto value = store(100000);
    t1->set_value(std::string(value.begin(), value.end()));
    t1->set_asset_id(1984);
    t1->set_fee_asset_id(0x00); // native token

    auto* t2 = transfer->add_transfers();
    t2->set_module_index(0x32);
    t2->set_method_index(0x06);
    t2->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");
    t2->set_value(std::string(value.begin(), value.end()));
    t2->set_asset_id(1984);
    t2->set_fee_asset_id(0x00); // native token

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "f502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c85020c00002800083206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a06003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(SubstrateSigner, Statemint_encodeTransaction_dot_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789245-2

    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto genesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(7ul);
    input.set_spec_version(9320u);
    input.set_transaction_version(9u);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_module_index(0x0a);
    transfer->set_method_index(0x03);
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(0x00);
    transfer->set_fee_asset_id(0x00); // native token

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "3902840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00001c00000a030050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(SubstrateSigner, Statemint_encodeTransaction_usdt_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789377-2

    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto genesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(8ul);
    input.set_spec_version(9320u);
    input.set_transaction_version(9u);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_module_index(0x32);
    transfer->set_method_index(0x06);
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(1984);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "5102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a00200001c00700003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

} // namespace TW::Substrate::tests