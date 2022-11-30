// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "proto/Ethereum.pb.h"
#include "HexCoding.h"
#include "uint256.h"
#include "Ethereum/Address.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

namespace TW::Ethereum::tests {

TEST(TWAnySignerZksync, BridgeToL2) {
    Proto::SigningInput input;
    auto chainId = store(uint256_t(1));
    auto nonce = store(uint256_t(1));
    auto gasPrice = store(uint256_t(14502000000));
    auto gasLimit = store(uint256_t(600000));
    auto maxFeePerGas = store(uint256_t(15002000000));
    auto maxInclusionFeePerGas = store(uint256_t(1000000000));
    auto amount = store(uint256_t(10000000000000000));
    auto key = parse_hex("427ba3bda2af6475388aaf30343b8d944a5d04f3e05204c6cc27e8cef6981ce2");
    auto privKey = PrivateKey(key);
    const auto p = privKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto address = Ethereum::Address(p).string();
    ASSERT_EQ(address, "0x812a8b9262Cb34f36E6564b466d0f8084c96672D");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_tx_mode(Proto::TransactionMode::Enveloped);

    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    input.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());

    input.set_private_key(key.data(), key.size());
    input.set_to_address("0x027c8a79075f96a8cde315b495949e5f1d92f1d6");
    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    Data payload;
    {
        auto func = ABI::Function("deposit", std::vector<std::shared_ptr<ABI::ParamBase>>{
                                                 std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(address)),
                                                 std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0x0000000000000000000000000000000000000000")),
                                                 std::make_shared<ABI::ParamUInt256>(uint256_t(10000000000000000))
        });
        func.encode(payload);
    }
    transfer.set_amount(amount.data(), amount.size());
    transfer.set_data(payload.data(), payload.size());

    std::string expected = "f8d18085037e305a80830c350094027c8a79075f96a8cde315b495949e5f1d92f1d6872386f26fc10000b8648340f549000000000000000000000000812a8b9262cb34f36e6564b466d0f8084c96672d0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002386f26fc1000026a0901379270e5bac3ccd78bd2dfc52ecf73c190a7a10194a8294ec3ec1206fda06a02eade531ea8ef5a2b73d7d5752a92e1834e1ee4ca6541dff9b7862e717938e10";

    {
        // sign test
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEthereum);

        ASSERT_EQ(hex(output.encoded()), expected);
    }
}

} // namespace TW::Ethereum::tests
