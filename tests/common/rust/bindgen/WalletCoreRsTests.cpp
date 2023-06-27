// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Polkadot/Signer.h"
#include "TestUtilities.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "gtest/gtest.h"
#include "proto/Ethereum.pb.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

TEST(RustBindgen, MoveParseFunctionArgument) {
    using namespace TW;
    std::string arg = "10000000";
    auto str_result = Rust::parse_function_argument_to_bcs(arg.c_str());
    ASSERT_EQ(str_result.code, Rust::OK_CODE);
    ASSERT_EQ(std::string(str_result.result), "8096980000000000");
    Rust::free_string(str_result.result);
}

TEST(RustBindgen, EthSigningMessageProto) {
    using namespace TW;

    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(42000000000ul)); // 0x09c7652400
    auto gasLimit = store(uint256_t(78009ul));       // 130B9
    auto toAddress = "0x5322b34c88ed0691971bf52a7047448f0f4efc84";
    auto token = "0x6b175474e89094c44da98b954eedeac495271d0f"; // DAI
    auto amount = uint256_t(2000000000000000000);
    auto amountData = store(amount);
    auto key = parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151");

    Ethereum::Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    // tx_mode not set, Legacy is the default
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto& erc20 = *input.mutable_transaction()->mutable_erc20_transfer();
    erc20.set_to(toAddress);
    erc20.set_amount(amountData.data(), amountData.size());

    Data serialized = data(input.SerializeAsString());
    Rust::CByteArrayResultWrapper res = Rust::pass_eth_signing_msg_through(serialized.data(), serialized.size());
    ASSERT_TRUE(res.isOk());
    ASSERT_EQ(res.unwrap().data, serialized);
}

TEST(RustBindgen, PolkadotSignTxProto) {
    using namespace TW;

    auto blockHash = parse_hex("0x343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642");
    auto genesisHash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");
    auto toAddress = "14E5nqKAp3oAJcmzgZhUD2RcptBeUBScxKHgJKU4HPNcKVf3";
    auto privateKey = parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115");
    auto expectedEncoded = "29028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee003d91a06263956d8ce3ce5c55455baefff299d9cb2bb3f76866b6828ee4083770b6c03b05d7b6eb510ac78d047002c1fe5c6ee4b37c9c5a8b09ea07677f12e50d3200000005008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0";

    Rust::CByteArrayResultWrapper res = Rust::polkadot_test_signing_input();
    ASSERT_TRUE(res.isOk());
    auto serialized = std::move(res.unwrap().data);

    Polkadot::Proto::SigningInput input;
    input.ParseFromArray(serialized.data(), static_cast<int>(serialized.size()));

    ASSERT_EQ(input.nonce(), 0ul);
    ASSERT_EQ(input.spec_version(), 17ul);
    ASSERT_EQ(data(input.private_key()), privateKey);
    ASSERT_EQ(input.network(), 0ul);
    ASSERT_EQ(input.transaction_version(), 3ul);

    ASSERT_EQ(input.era().block_number(), 927699ul);
    ASSERT_EQ(input.era().period(), 8ul);

    auto transfer = input.balance_call().transfer();
    ASSERT_EQ(data(transfer.value()), store(uint256_t(12345ul)));
    ASSERT_EQ(transfer.to_address(), toAddress);

    auto output = Polkadot::Signer::sign(input);
    ASSERT_EQ(hex(output.encoded()), expectedEncoded);
}
