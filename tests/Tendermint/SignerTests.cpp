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

    auto fromAddress = TW::Tendermint::Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = TW::Tendermint::Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto message = Proto::SendTokensMessage();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amount = message.add_amount();
    amount->set_denom("atom");
    amount->set_amount(10);
    
    *input.mutable_message() = message;
    
    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();

    ASSERT_EQ(hex(signature.begin(), signature.end()), "761fbe8bb5f8a05026baacf360cf4163dad6bbcb5daeb877edf73ad934664f1e27cbf986370e530aa201b868b7a1308b932e2df060bfda88044b01c2b1bedb63");
}

}}
