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
    //input.set_fee();
    input.set_memo("");
    input.set_sequence(0);

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
}

}} // namespace
