// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Nebulas/Address.h"
#include "Nebulas/Signer.h"
#include <time.h>

#include <gtest/gtest.h>

namespace TW::Nebulas {

using boost::multiprecision::uint256_t;

class SignerExposed : public Signer {
  public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

TEST(NebulasSigner, EmptyData) {
    Data data(0);
    uint256_t zero = load(data);

    ASSERT_EQ(zero, uint256_t(0));
}

TEST(NebulasSigner, Hash) {
    auto from = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    auto to = Address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto transaction = Transaction(
        /* to: */ from,
        /* nonce: */ 7,
        /* gasPrice: */ 1000000,
        /* gasLimit: */ 200000,
        /* to: */ to,
        /* amount: */ 11000000000000000000ULL,
        /* timestamp: */ 1560052938,
        /* payload: */ std::string());
    auto signer = SignerExposed(1);
    auto hash = signer.hash(transaction);

    ASSERT_EQ(hex(hash), "505dd4769de32a9c4bb6d6afd4f8e1ea6474815fd43484d8917cbd9e0993b885");
}

TEST(NebulasSigner, Sign) {
    auto input = Proto::SigningInput();
    input.set_from_address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    input.set_to_address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto value = store(uint256_t(7));
    input.set_nonce(value.data(),value.size());
    value = store(uint256_t(1000000));
    input.set_gas_price(value.data(),value.size());
    value = store(uint256_t(200000));
    input.set_gas_limit(value.data(),value.size());
    value = store(uint256_t(11000000000000000000ULL));
    input.set_amount(value.data(),value.size());
    input.set_payload("");
    value = store(uint256_t(1560052938));
    input.set_timestamp(value.data(),value.size());

    const auto privateKey = PrivateKey(parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_chain_id("1");
    auto signer = SignerExposed(1);
    Proto::SigningOutput output = signer.sign(input);

    auto signature = output.signature();
    ASSERT_EQ(hex(signature.begin(),signature.end()), "f53f4a9141ff8e462b094138eccd8c3a5d7865f9e9ab509626c78460a9e0b0fc35f7ed5ba1795ceb81a5e46b7580a6f7fb431d44fdba92515399cf6a8e47e71500");
}

} // namespace TW::Nebulas