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

TEST(CosmosSigner, SignTx) {
    auto input = Proto::SigningInput();
    input.set_account_number(0);
    input.set_chain_id("mychainid");
    input.set_memo("");
    input.set_sequence(0);

    auto fromAddress = parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21");
    auto toAddress = parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9");

    auto message = Proto::SendTokensMessage();
    message.set_from_address(fromAddress.data(), fromAddress.size());
    message.set_to_address(toAddress.data(), toAddress.size());
    auto amount = message.add_amount();
    amount->set_denom("atom");
    amount->set_amount(10);
    
    *input.mutable_message() = message;
    
    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();

    ASSERT_EQ(hex(signature.begin(), signature.end()), "229f4a6102d33899ad212532778bd0e97a51863698f5df81314a6c0a70bd61ac3d8d73a7faeb2fc8b78a17fb45def0207594bd7e880914a694eac10c653edd53");
}

}}
