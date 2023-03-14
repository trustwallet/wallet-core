// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "Ethereum/Address.h"
#include "Ethereum/ABI.h"
#include "proto/Ethereum.pb.h"
#include "HexCoding.h"
#include "uint256.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Binance {

TEST(BinanceSmartChain, SignNativeTransfer) {
    // https://explorer.binance.org/smart-testnet/tx/0x6da28164f7b3bc255d749c3ae562e2a742be54c12bf1858b014cc2fe5700684e

    auto toAddress = parse_hex("0x31BE00EB1fc8e14A696DBC72f746ec3e95f49683");
    auto transaction = Ethereum::TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 0,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ toAddress,
        /* amount: */ 10000000000000000 // 0.01
    );

    // addr: 0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7  mnemonic: isolate dismiss ... cruel note
    auto privateKey = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));
    uint256_t chainID = 97;
    auto signature = Ethereum::Signer::sign(privateKey, chainID, transaction);

    auto encoded = transaction->encoded(signature, chainID);
    ASSERT_EQ(hex(encoded), "f86c808504a817c8008252089431be00eb1fc8e14a696dbc72f746ec3e95f49683872386f26fc100008081e5a057806b486844c5d0b7b5ce34b289f4e8776aa1fe24a3311cef5053995c51050ca07697aa0695de27da817625df0e7e4c64b0ab22d9df30aec92299a7b380be8db7");
}

TEST(BinanceSmartChain, SignTokenTransfer) {
    auto toAddress = parse_hex("0x31BE00EB1fc8e14A696DBC72f746ec3e95f49683");
    auto func = Ethereum::ABI::Function("transfer", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
        std::make_shared<Ethereum::ABI::ParamAddress>(toAddress),
        std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(10000000000000000))
    });
    Data payloadFunction;
    func.encode(payloadFunction);
    EXPECT_EQ(hex(payloadFunction), "a9059cbb00000000000000000000000031be00eb1fc8e14a696dbc72f746ec3e95f49683000000000000000000000000000000000000000000000000002386f26fc10000");

    auto input = Ethereum::Proto::SigningInput();
    auto chainId = store(uint256_t(97));
    auto nonce = store(uint256_t(30));
    auto gasPrice = store(uint256_t(20000000000));
    auto gasLimit = store(uint256_t(1000000));
    auto tokenContractAddress = "0xed24fc36d5ee211ea25a80239fb8c4cfd80f12ee";
    auto dummyAmount = store(uint256_t(0));
    // addr: 0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7  mnemonic: isolate dismiss ... cruel note
    auto privateKey = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_to_address(tokenContractAddress);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    transfer.set_data(payloadFunction.data(), payloadFunction.size());

    const std::string expected = "f8ab1e8504a817c800830f424094ed24fc36d5ee211ea25a80239fb8c4cfd80f12ee80b844a9059cbb00000000000000000000000031be00eb1fc8e14a696dbc72f746ec3e95f49683000000000000000000000000000000000000000000000000002386f26fc1000081e6a0aa9d5e9a947e96f728fe5d3e6467000cd31a693c00270c33ec64b4abddc29516a00bf1d5646139b2bcca1ad64e6e79f45b7d1255de603b5a3765cbd9544ae148d0";

    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSmartChain);

    EXPECT_EQ(hex(output.encoded()), expected);
}

} // namespace TW::Binance
