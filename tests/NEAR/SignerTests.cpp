// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "Base58.h"
#include "proto/NEAR.pb.h"
#include "NEAR/Address.h"
#include "NEAR/Signer.h"
#include <iostream>

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(NEARSigner, SignTx) {
    auto publicKey = Base58::bitcoin.decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.mutable_public_key()->set_key_type(0);
    input.mutable_public_key()->set_data(publicKey.data(), publicKey.size());
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto &transfer = *input.mutable_actions(0)->mutable_send_money();
    Data deposit(16, 0);
    deposit[0] = 1;
    transfer.mutable_deposit()->set_number(deposit.data(), deposit.size());

    auto privateKey = Base58::bitcoin.decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto signer = NEAR::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("v4UxPuX98S9+2dgnMwLL0nthTy1rdu5stLkqJ7SDQgm+LTvxi7482ZZ/RQMByShxGx+Uvu8CsnCz8jUhngQ3CQ==", signatureInBase64);

    auto output = signer.build();

    // TODO
}

}
