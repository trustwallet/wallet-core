// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "proto/Solana.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

TEST(TWAnySignerSolana, SignTransfer) {
    auto privateKey = Base58::bitcoin.decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
        "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
        "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
        "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDelegateStakeTransaction) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_stake_transaction();
    message.set_validator_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "7gcsGMV8fPkuSDegrBk5KF4eZtmGqdixEr5Kxx7m74nErH5EWxkKcvy5LKg3kqKHLMtzqGXrM8EdVskLu5k6cQUVmz"
        "nfd6iGpuDkn4BWi8HaqSoaeL7DCx6QHLUwcyvorfqPkgSaYopY9MeB6MGKS4bKCQi1paP7eEKZj9JgAZTHnpzAuFzq"
        "E5dC98MrbThoBmHqfybw1jHssp4NZjrxsuMz4oYcHe837WmnpinE68QEVf9FzuoXKrMDwYZQYogJHUZgxghvMmJu3X"
        "VZk7hL1h7SgE9ow2SvvNfAUYfUTuz8N9m6JcXgwmNL51ZweW9F1fekmSKE3vLEAaVvvYFsDxn1gt4bXuTdAMuUDKdH"
        "33YMLQr8eEsdUDZUm33KtuE7Ddy84NEg3KbpWAy8T4vkwiLyvoyjmMdzCkMuobyXWQV1rfRk5vTJ4x6dMgznfAeKEq"
        "md6xZ2hN8JBmTenP5dsvZCCSCf4G7cUkdndKu552KALqiNHd5msgLJvJKSHmZjFhS43fDCvkG7njF8yaZzWjmAknWU"
        "BbV6YaGmD3XmcWfJgvB1zivZJhiMbzopP8Nm5wL5iDbCrSZTGq2tzEsTvje75wv2RtuAcgiicEPTuPAin9fKyMbCpf"
        "67pGgWPwH5DwYumMwd8zwoJyuakyqFsFLnBKTvp8pFCijdj7fEhyC31xuMV7crwyrN5X3y7QKCE7PZcBP637YHEPtT"
        "vt1ECp4CqBSnvPc8vRD8EMhHe5jRFSDkQriUenEPFc51dTDTJWL26xuiTivktEm6ahHq5d6MPr4NRDvcRG2cZvEgxH"
        "BLpKfuB5XL3JfQZ3Nn3B916gaK8owz9Rk2e3";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDeactivateStakeTransaction) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_deactivate_stake_transaction();
    message.set_validator_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "AhfB77PTGTKBfbGPGuEz2khbBy8m8Kou1zqZST9dP7PLJNSeEze5NJuCh5qecPLa3S8xAQ6mTULmnAWiW81ib87nhy"
        "wFtx5nKiUvmhdXsvKCSX6NNtNXdRz5yZi3UEop4obco85SY2czS6n4SJwmtDedHLtg9urqdZVth7AUM8KAtrRsksyv"
        "ZRYXh64Z8QGyNY7ekj31ae11avGiSDNWYZZHqx7VPWRsKeatGyGk5zPmnRdL8ABMQgJ1Te7wAWwVnNn5QcoAxDuPw6"
        "uDctP8Q5S4TieRVatCnukQFj5BTJisez3E2ZJPWhVrMh4K3wEFkPHA7dR";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignWithdrawStakeTransaction) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_withdraw_transaction();
    message.set_validator_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "7Y1Wg1yHNs8MgWFiFSfcsRtqdMwZg8oGeQnTABYDfyDnof4VSFw63s3PuSxvUCJqqHKgYNVb8UTNcNiYHY8kng4NqT"
        "cVV5SA1KAWRzKHVGUxNWioAEXXVot5iJ1XbUWuuZUZBtsraaBjNyfmgWEDje3ESdGhiVL7vadU1uHeBuUKwM3nqB6y"
        "oeggeNyzmT34hs9utyehTFg48MAfrKEFKxaby7YZD6JbXFS1SyG1kxKWnCpoPgX3efwDwukmyDwxrKdABt9eTwmaiX"
        "KbTnK1hzBTatNfnJ9ePuWkhWFrjyDrGdx5S5KpybxET2vV9CSpExcD51BA6NPemTpjbhLYnJEzHWBGfYqfxu7p3257"
        "NHhpQQrSU56adk4dAQFjEYP";
    ASSERT_EQ(output.encoded(), expectedString);
}
