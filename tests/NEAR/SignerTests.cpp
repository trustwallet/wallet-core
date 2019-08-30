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
    auto publicKey = Base58::bitcoin.decode("22skMptHjFWNyuEWY22ftn2AbLPSYpmYwGJRGwpNHbTV");

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

    auto privateKey = Base58::bitcoin.decode("2wyRcSwSuHtRVmkMCGjPwnzZmQLeXLzLLyED1NDMt4BjnKgQL6tF85yBx6Jr26D2dUNeC716RBoTxntVHsegogYw");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = NEAR::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg==", signatureInBase64);

    auto output = signer.build();

    // TODO
}

}
