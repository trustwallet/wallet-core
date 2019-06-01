// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aeternity/Address.cpp"
#include "Aeternity/Transaction.cpp"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWPKCS8.h>

#include "HexCoding.h"
#include <Aeternity/Signer.h>
#include <gtest/gtest.h>

TEST(AeternityTransaction, EncodeRlp) {
    std::string sender_id = "ak_2a1j2Mk9YSmC1gioUq4PWRm3bsv887MbuRVwyv4KaUGoR1eiKi";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint64_t amount = 10;
    uint64_t fee = 2e13;
    std::string payload = "Hello World";
    uint64_t ttl = 82757;
    uint64_t nonce = 49;

    auto tx = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto encodedTx = tx.encode();

    ASSERT_EQ(encodedTx, "tx_+F8MAaEBzqet5HDJ+Z2dTkAIgKhvHUm7REti8Rqeu2S7z+tz/vOhAR8To7CL8AFABmKmi2nYdfeAPOxMCGR/btXYTHiXvVCjCoYSMJzlQACDAUNFMYtIZWxsbyBXb3JsZNM5B3Q=");
}

