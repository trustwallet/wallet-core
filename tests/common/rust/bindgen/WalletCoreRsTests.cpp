// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TestUtilities.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "gtest/gtest.h"
#include "proto/Ethereum.pb.h"
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
    auto gasPrice = store(uint256_t(42000000000)); // 0x09c7652400
    auto gasLimit = store(uint256_t(78009));       // 130B9
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
