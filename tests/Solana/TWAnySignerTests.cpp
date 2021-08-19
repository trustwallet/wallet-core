// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "proto/Solana.pb.h"
#include "Solana/Address.h"
#include "PrivateKey.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

const auto expectedString1 =
    "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
    "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
    "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
    "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";

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

    ASSERT_EQ(output.encoded(), expectedString1);
}

TEST(TWAnySignerSolana, SignTransferToSelf) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "EKUmihvvUPKVN4GSCFwZRtz8WiyAuPvthW69Smo19SCjcPLQ6T7EVZd1HU71WAoe1bfgmPNS5JhU7ZLA9XKG3qbZqe"
        "EFJ1xmRwW9ZKw8SKMAL6VRWxp87oLu7PSmf5b8R34vCaww3XLKtZkoP49a7TUK31DqPN5xJCceMB3BZJyaojQaKU8n"
        "UkzSGf89LY6abZXp9krKAebvc6bSMzTP8SHSvbmZbf3VtejmpQeN9X6e7WVDn6oDa2bGT";
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
        "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7"
        "uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM7jtGoYQFb7eFVbujUb6tbeQ9UqLJq1sJ7uMZ4wqecmQP"
        "ouDmJnpmJk4CHMzLnPNTwyGmGio6sYAS3xKZ7DFXvjwGPuD8PyYHSfdPro1p3jy9igPZNAbQ6fgK7LL3sERKCUdvPy"
        "7k14xgHbtsVy2mu54LY5c8F9sFst2uzQiTsXRTdjPFAyCVwB5pccNVotCrJ6Q2aKSC2D2knVH7LgWzSBMSreJG75xy"
        "ATneu922wSzz7QJDieqhDtdePtSbPtoCdtPNmDfdaeDbHxVAxMios9F7RSRmH2dq86NfWDvF8TuEbYY7gPnygz6jGv"
        "wfqSSoSnY8TnUhhceC7wJSMc8Hcf1kyfi8dqKm7rF57YjnrQoMmL5bWqJLKoJtdfFu24ceQN21k38U2tUMWJaBASWu"
        "kgTJUbNSCemNPZt4P3cNbeB3L1wBj4GEYXVTbTFYKME5JscU5RsnkMJZZ1PgzU285SkncqNSgxkpZVhmenTXpuZv74"
        "rXzariX8P4sprRgKUoj4b7Nu72Pya1zr7k45isMwgxtLnnnTK5k7mrZRDw3jBSBuukJBja93zaidm8HCQdwQsBt5CN"
        "SgSXug1R2t6Sdm5tjJrsd1gyRv7udFbHCdbVEeatzULNSSGdwjwwJDy1DTC12ddBNHd8k5ic5TDwrWdfCxbDRoFYw8"
        "49YNNUuyNAPz1jDCkLG9af6KFFLxfuR9pnF8jSyTcQAq95YiiD9sC3mAUoe8AkYfy929XzTEatP1vasMvo";
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

TEST(TWAnySignerSolana, SignCreateTokenAccount1) {
    auto privateKeyData = Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_token_account_transaction();
    message.set_main_address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_token_address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVnH2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtCjAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzjB2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2TN3xXwu1";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateTokenAccount2) {
    auto privateKeyData = parse_hex("4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_token_account_transaction();
    message.set_main_address("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_token_address("ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf");
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("HxaCmxrXgzkzXYvDFTToENtf9rVKk7cbiuSUqnqNheHq");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        // https://explorer.solana.com/tx/5KtPn1LGuxhFiwjxErkxTb7XxtLVYUBe6Cn33ej7ATNVyorrkk3UAFJWDBUmzP8CZjmkocCxiMAdYnvrKoGpMsJx
        "EoJGDRFZdnjmx7rgwYSuDGTMTUdxCBeh8RggrQDzGht9bwzLPpCWkCrN4iQJqg3R6JxP7z2QZuf7dGCZcjMVBmmisYE8waRsohcvygRwmGr6nefbaujR5avm2x3EUvoTGyy8cMZJxX7URx45qQJyCgqFLNFCQzD1Kej3xCEPAJqCdGZgmqkryw2E2nkpGKXgRmbyEg2rFgd5kpvjG6jSLLYzGomxVnaKK2XyMQbcedkTMYJ8Ara71iWPRFUziWfgivZcA1qsQp92Fpao3FSsRprhoQz9u1VyAnh8zEM9jCKiE5s4dwCknqCJYeYsbMLn1be2vNP9bMQfu1jjGSHmbb9WR3E2vakTUEUByASXqSAJZuXYE5scopEzB28rC8nrC31ArLMZng5wWym3QbqEv2Syd6RHoEeoXR6vA5LPqvJKyvtH82p4hc4XbD18128aNrFG3GTD2P";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateTokenAccountForOther) {
    auto privateKeyData = parse_hex("4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_token_account_transaction();
    message.set_main_address("3xJ3MoUVFPNFEHfWdtNFa8ajXUHsJPzXcBSWMKLd76ft");
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_token_address("67BrwFYt7qUnbAcYBVx7sQ4jeD2KWN1ohP6bMikmmQV3");
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("HmWyvrif3QfZJnDiRyrojmH9iLr7eMxxqiC9RJWFeunr");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        // https://explorer.solana.com/tx/3E6UFVamHCm6Bgk8gXdZex7R7tJAVxqJm6t9ephAKu1PjcfZrD7CJqMwKu6RrvWSUESbZFqzdUyLXuxAFaawPHvJ
        "4BsrHedHuForcKDhLdnLYDXgtQgQEj3EQVDtEhqa7o6ukFjW3shpTWv6PeKQdMp6af4ASjD4xQeZvXxLK5WUjguVMUf3xdJn7RnFeM7hdDJ56RDBM5PRJbRJVHjz6FJ7SVNTvr9y3gVYQtWx7NfKRxiyEAfq9JG7nqxSWaW6raMr9t35aVcdAVuXE9iXj3rzhVfCS69vVzy5KcFEK3mvDYG6L12V2CfviCydmeCvPw5r3zBUrZSQv7Ti4XFNBrPbk28gcqQwsBknBqasHxHqD9VUyPmBTuUyXq75QN8rhqN55NjxKBUw37tEUS1jKVpWnTeLFq1eRAMdXvjftNuQ5Bmm8Zc12PGWj9vdorBaYyvZXexJST5xNjR4SCkXvXZoRScETck95chv3VBn54jP8DpB4GGUmATFKSxpdtnNV64i1SQXW13KJwswthJvAaDiqevQLKLkvrTEAdb4BxEfPkFjDVti6P58rTZCMg5CTVLqdmWwpTSW5V";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignTokenTransfer1) {
    auto privateKeyData = Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_token_transfer_transaction();
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_sender_token_address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    message.set_recipient_token_address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    message.set_amount(4000); // 0.004
    message.set_decimals(6);
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        // https://explorer.solana.com/tx/3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg
        "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jRe2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvPiPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMqDV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnotcrH7pUa94xCVvCPPaomF";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignTokenTransfer2) {
    auto privateKeyData = Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_token_transfer_transaction();
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_sender_token_address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    message.set_recipient_token_address("ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf");
    message.set_amount(6100); // 0.0061
    message.set_decimals(6);
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("zMEbroNLJ4vfDTdQyA72rk35c7nPo4K38efHLujbSuz");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        // https://explorer.solana.com/tx/2pMvzparE16evNgNhiexBfj15eurQgqFJXemYkuGasWV8RfT5tQseadqXA2VXbgGZPM1MpLcGwfkKKqvYvrKTmnR
        "LCtawaKHmvh9WEjYPFFMDQXsdKMQbVyK4Q3aRRfLCouqw6GE4p31PRPFoQqtazTziEj3ex3iLgnCspz1MN4SUE9d33g3HiiA6oCS6wGMvB2i3ojtmJzndCiLoDmuZgiuGouVSeS2MAEUoS3CRjdnbNKbRwgKn8YsDe1bZ57ueipfBLJfiE7xr8ji678uAv8FcMgo8Mq88SBGxVCUhjMS2VGQZhRUHHzDmvnzxhbbUzsLDfApzjHExkUm7ws3cQ2i1cSpQNCQWJd6rcDv1sYwDAavPS571Ny3CUq4cZxABh45Gj88LkRpzBMRdoebrh9hPy8ZRnu7PocBVjZytCgdF4CuhzdYNsmdcuU2WN5CEmv5zQ7pBrFdLZ8bBifP";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateAndTransferToken) {
    auto privateKeyData = Base58::bitcoin.decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    message.set_recipient_main_address("71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd");
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_recipient_token_address("EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY");
    message.set_sender_token_address("ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf");
    message.set_amount(2900);
    message.set_decimals(6);
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        //https://explorer.solana.com/tx/449VaYo48LrkMJF6XVKt9sJwVQN6Seqrmh9erDCLtiuj6BgFG3wpF5TwjNkxgJ7qzNa6NTj3TFsU3h9hKszfkA7w
        "3Y2MVz2VVi7aEyC9q1awwdk1ModDBPHRSacKmTYnSgkmbbJeZ62Fub1bVPSHaTy4LUcQpzCQYhHAKtTKXUDYijEeLsMAUqPBEMAq1w8zCdqDpdXy6M4PuwNtYVV1WgqeiEsiMWpPp4BGWKfcziwFbmYueUGituacJq4wTnt92fho8mFi49XW64gEG4iNGScDtJkY7Geq8PKiLh1E9JMJoceiHxKbmxzCmmLTxEHdhySYHcDUSXnXWogZskeZNBMtR9dNjEMkCzEjrxRpBtJPtUNshciY45mDPNmw4j3xyLCBTRikyfFLc5g11r3UgyVD4YokoPRvrEXsgt6W3yjBshropBm6mY2eJYvfY2eZz4Yq8kLcUatCHVKtjcb1mP9Ww57KisJ9bRhipC8sodFaMYhZARMEa4a1u9eH4MyNUATRGNXarwQSBY46PWS3nKP6QBK7Dw7Ppp9MmYkdPcXKaLScbyLF3jKu6dHWMkHw3WdXSsM1wwXjXnWF9LxdwaEVcDmySWybj6aKD9QCWTU5kdncqJU56f7SYNRTN289WdUFGNDmSh56tj2v1";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignJSON) {
    auto json = STRING(R"({"recentBlockhash":"11111111111111111111111111111111","transferTransaction":{"recipient":"EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd","value":"42"}})");
    Data keyData = Base58::bitcoin.decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    EXPECT_EQ(hex(keyData), "8778cc93c6596387e751d2dc693bbd93e434bd233bc5b68a826c56131821cb63");
    auto key = WRAPD(TWDataCreateWithBytes(keyData.data(), keyData.size()));

    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeSolana));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeSolana));
    assertStringsEqual(result, expectedString1);
}
