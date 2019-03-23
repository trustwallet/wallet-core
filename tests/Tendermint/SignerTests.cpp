// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "Tendermint/Address.h"
#include "Tendermint/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW {
namespace Cosmos {

TEST(CosmosSigner, Sign) {
    auto input = Proto::SigningInput();
    input.set_account_number(0);
    input.set_chain_id("mychainid");
    input.set_memo("");
    input.set_sequence(0);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();

    ASSERT_EQ(hex(signature.begin(), signature.end()), "a9c49e54f2889d7684a112946a010f4f6d75191439ca36ec5d569e9bfa9bf6db6df4340263796af6c896c915d67e671d77e77f9dd27329f04c1a4cb454a96779");
}

}} // namespace
