// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/Address.h"
#include "Solana/Program.h"
#include "Solana/Transaction.h"

#include "Base58.h"
#include "BinaryCoding.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Solana {

TEST(SolanaTransaction, TransferMessageData) {
    auto from = Address("6eoo7i1khGhVm8tLBMAdq4ax2FxkKP4G7mCcfHyr3STN");
    auto to = Address("56B334QvCDMSirsmtEJGfanZm8GqeQarrSjdAb2MbeNM");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    auto expectedHex =
        "0100010353f9d600fe925083bb399907ea648d23a6a081fc7e9059202fd725f7edd281dd3cc1ff9ba3c7a876c8"
        "082df2f8a36ea9342ce3819dd4b6fa72d4a18e04a5363a00000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000010202"
        "00010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.messageData()), expectedHex);
}

TEST(SolanaTransaction, TransferSerializeTransaction) {
    auto from = Address("41a5jYky56M6EWDsFfLaZRxoRtgAJSRWxJnxaJNJELn5");
    auto to = Address("4iSnyfDKaejniaPc2pBBckwQqV3mDS93go15NdxWJq2y");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    auto signature = Base58::decode("46SRiQGvtPb1iivDfnuC3dW1GzXkfQPTjdUyvFqF2sdPvFrsfx94fys2xpNKR6UiAj7RgKWdJG6mEfe85up6i1JT");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString =
        "5SiHeYyuDgjHxWHbYXSSPfmYc8s7EYZ8bdZ7j15z9Bj1yyZA3Bia9uWkRdXVkuqifXiiQj6fVKy"
        "7UkCL5kvv6iKrfjWTZ3szMVssTFxgJ7p8UJ7Mgg2uhHejVJvbzbiHHLbNVuJFs6kBxddnJ2yjWU"
        "Cp2dYJgjmphfA8hRHHdPH4Rv6znxEhD8q9XY4nByRPL7oMCo32oxeJn5rGbUZdCkapRUXG7zU9w"
        "hv6KjBktcUQZRCahhowGJT4UM5yCNCsUcqY9yan7UxqPyJgaFPuq4duqWJtQ39bTQ36X";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferTransactionPayToSelf) {
    auto from = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto to = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    auto signature = Base58::decode(
        "3CFWDEK51noPJP4v2t8JZ3qj7kC7kLKyws9akfHMyuJnQ35EtzBptHqvaHfeswiLsvUSxzMVNoj4CuRxWtDD9zB1");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString =
        "EKUmihvvUPKVN4GSCFwZRtz8WiyAuPvthW69Smo19SCjcPLQ6T7EVZd1HU71WAoe1bfgmPNS5JhU7ZLA9XKG3qbZqe"
        "EFJ1xmRwW9ZKw8SKMAL6VRWxp87oLu7PSmf5b8R34vCaww3XLKtZkoP49a7TUK31DqPN5xJCceMB3BZJyaojQaKU8n"
        "UkzSGf89LY6abZXp9krKAebvc6bSMzTP8SHSvbmZbf3VtejmpQeN9X6e7WVDn6oDa2bGT";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferWithMemoAndReferenceTransaction) {
    const auto from = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    const auto to = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    const auto memo = "HelloSolana73";
    std::vector<Address> references = {Address("GaeTAQZyhVEocTC7iY8GztSyY5cBAJTkAUUA1kLFLMV")};
    auto transaction = Transaction(from, to, 42, recentBlockhash, memo, references);
    auto signature = Base58::decode("3CFWDEK51noPJP4v2t8JZ3qj7kC7kLKyws9akfHMyuJnQ35EtzBptHqvaHfeswiLsvUSxzMVNoj4CuRxWtDD9zB1");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString =
        "3pzQEdU38uMQgegTyRsRLi23NK4YokgZeSVLXYzFB7HShqZZH8FdBLqj6CeA2d2L8oR9KF2UaJPWbE8YBFmSdaafeg"
        "oSXJtyj7ciwTjk5ieSXnPXtqH1TEcnMntZATg7gKpeFg6iehqdSUtZuQD1PGmHA1TrzzqLpRSRrc1sqPz8EpSJcQr1"
        "Y41B1XCEAfSJDfcuNKrfFrnQaVtRz6tseQfd9uXNYNuR1NQSepWdav5wQiohLUMDiZtxuwb7FQkQ68WE1FDsHmd4Jp"
        "bWKmDEjz7HFyQY37vf6NBJyX5qWJpFMSg5qGKWvhNCDM32yM4A7HhPeoTWEywE5CXcNmQqdbRt4BzF1A11uqv4etW"
        "j";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, StakeSerializeTransactionV2) {
    auto signer = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromRecentBlockhash(signer, recentBlockhash, programId);
    auto message = LegacyMessage::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);
    auto signature = Base58::decode(
        "2GXRrZMMWTaY8ycwFTLFojAVZ1EepFqnVGW7b5bBuuKPiVrpaPXMAwyYsSmYc2okCa1MuJjNguu1emSJRtZxVdwt");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString =
        "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7"
        "uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM92cXrzUhKGWtQ6cATeQh8i8ZfHpmjyuik7Eg3SQ4sa25"
        "43CmcozzjmTTWThThuLdvFCZJzBeRBFWLujqjbs5mA66XVtiDwsEqYByznoo4BN45XUHxnZebmPfo4hi5sf27UkhzP"
        "Hik371BGxbVDexQp4y5nCEHy8ybfNCvMPLr2SEBiWSifwPkmwYN3hGCkBpqLoHCCiRcyJuRHW8hSDFR4JPQ3Xe3FGf"
        "pgbayaawZigUnFuPGSpoGrURZRoLCzc6V4ApqcJbmzFhg5zJz2yTX5GvQSYWLFnTKbPYcgBNpdyMLJTivonrKtgkfd"
        "ymZVKjDwnHUApC7WD4L9mqzTf1dzR61Fxhu3Rdh8ECiVEDgB1wkWZWkTKEdANmtaYLKCMUs3n4VhuZbSFLEiTg7yRW"
        "M2pjBgiBB4qywbF7SE75UtzSFCaDnn27mKkxRBqZEGEgfpEoK2AxjsiCZEZxfLeyZFbwWe7xasmNiXr6CnAQhwsmxJ"
        "k79h7SYmaje76JLxHVX5gbQmLfn5bc1xthS3YhteSovQ8xYq1jiHCfsXRwbxKrNA4kVMiSa6spoU9AhFL8cDAZjAqo"
        "U4YRwBihZVhXSFCRnYAK8FabzEv1M44EeHX1sfMG8T1U7y3DEjom7jv6rqZfLumWpbXDTqanB7zTbTjGyDcBBf21ed"
        "jpZzBZ7osS5fTVYJ5mZBSvjjhuGkUgZZWgYozAKvdyyrJH6UdcPvNm2XgMRYJxqyCin1zhCeQ25vK1H8Jj";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, StakeSerializeTransactionV1) {
    auto signer = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto recentBlockhash = Base58::decode("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromValidatorSeed(signer, voteAddress, programId);
    auto message = LegacyMessage::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);
    auto signature = Base58::decode(
        "2GXRrZMMWTaY8ycwFTLFojAVZ1EepFqnVGW7b5bBuuKPiVrpaPXMAwyYsSmYc2okCa1MuJjNguu1emSJRtZxVdwt");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString =
        "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7"
        "uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM7jtGoYQFb7eFVbujUb6tbeQ9UqLJq1sJ7uMZ4wqecmQP"
        "ouDmJnpmJk4CHMzLnPNTwyGmGio6sYAS3xKZ7DFXvjwGPuD8PyYHSfdPro1p3jy9igPZNAbQ6fgK7LL3sERKCUdvPy"
        "7k14xgHbtsVy2mu54LY5c8F9sFst2uzQiTsXRTdjPFAyCVwB5pccNVotCrJ6Q2aKSC2D2knVH7LgWzSBMSreJG75xy"
        "ATneu922wSzz7QJDieqhDtdePtSbPtoCdtPNmDfdaeDbHxVAxMios9F7RSRmH2dq86NfWDvF8TuEbYY7gPnygz6jGv"
        "wfqSSoSnY8TnUhhceC7wJSMc8Hcf1kyfi8dqKm7rF57YjnrQoMmL5bWqJLKoJtdfFu24ceQN21k38U2tUMWJaBASWu"
        "kgTJUbNSCemNPZt4P3cNbeB3L1wBj4GEYXVTbTFYKME5JscU5RsnkMJZZ1PgxSi63HT4hwQLok4c18UdJgzMFu1njp"
        "Zj3Sw76mwV3ea7ruHnP4yyM3YhUGbNjpx5fAcnvdLcXChdsgeUpJhutME6V86Rk2EEskoJeD3qNWi3hvfQx172hZRH"
        "yKyr29Ts1uLQxcMJq7oeQUxvTfXxSe6cBuPJUDFkAET3qpS7rWM7rvQQ8rDLQF5QvcJnrYTq12pVgw28WXdgi45811"
        "a7DWHGuwHRj5FJdLQAHkKe4EXVeTCdbYHREVwuyTJgAvb8SXjRE5a9n3qpRDr7iEd5UDZKB5HgvMsMYWh5";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateTokenAccountTransaction) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto tokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    auto recentBlockhash = Base58::decode("9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");
    auto message = LegacyMessage::createTokenCreateAccount(signer, signer, token, tokenAddress, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 5);
    ASSERT_EQ(message.accountKeys.size(), 7ul);
    EXPECT_EQ(message.accountKeys[0].string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.accountKeys[1].string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    EXPECT_EQ(message.accountKeys[2].string(), "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(message.accountKeys[3].string(), "11111111111111111111111111111111");
    EXPECT_EQ(message.accountKeys[4].string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    EXPECT_EQ(message.accountKeys[5].string(), "SysvarRent111111111111111111111111111111111");
    EXPECT_EQ(message.accountKeys[6].string(), "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    EXPECT_EQ(Base58::encode(message.mRecentBlockHash), "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    ASSERT_EQ(message.instructions[0].accounts.size(), 7ul);
    EXPECT_EQ(message.instructions[0].accounts[0].account.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.instructions[0].accounts[1].account.string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    EXPECT_EQ(message.instructions[0].accounts[2].account.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.instructions[0].accounts[3].account.string(), "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(message.instructions[0].accounts[4].account.string(), "11111111111111111111111111111111");
    EXPECT_EQ(message.instructions[0].accounts[5].account.string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    EXPECT_EQ(message.instructions[0].accounts[6].account.string(), "SysvarRent111111111111111111111111111111111");
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    auto signature = Base58::decode("3doYbPs5rES3TeDSrntqUvMgXCDE2ViJX2SFhLtiptVNkqPuixXs1SwU5LUZ3KwHnCzDUth6BRr3vU3gqnuUgRvQ");
    transaction.signatures.push_back(signature);

    auto expectedString =
        // test data obtained from spl-token create-account
        "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVn"
        "H2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtC"
        "jAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzj"
        "B2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3"
        "W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2"
        "TN3xXwu1";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferTokenTransaction_3vZ67C) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto senderTokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    auto recipientTokenAddress = Address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    auto recentBlockhash = Base58::decode("CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi");
    auto message = LegacyMessage::createTokenTransfer(signer, token, senderTokenAddress, recipientTokenAddress, amount, decimals, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 2);
    ASSERT_EQ(message.accountKeys.size(), 5ul);
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[0].accounts.size(), 4ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    auto signature = Base58::decode("3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg");
    transaction.signatures.push_back(signature);

    auto expectedString =
        // https://explorer.solana.com/tx/3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg
        // test data obtained from spl-token transfer
        "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jR"
        "e2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvP"
        "iPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMq"
        "DV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnot"
        "crH7pUa94xCVvCPPaomF";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateNonceAccountTransaction) {
    auto sender = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto nonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    uint64_t rent = 10000000;
    auto recentBlockhash = Base58::decode("mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat");
    auto message = LegacyMessage::createNonceAccount(sender, nonceAccount, rent, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 2);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 5ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 2ul);
    EXPECT_EQ(message.instructions[1].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[1].accounts.size(), 3ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("3dbiGHLsFqnwA1PXx7xmoikzv6v9g9BXvZts2126qyE163Bypur"
                                               "kvgbDiF5RmrEZRiT2MG88v6xwyJTkhhDRuFc9"));
    transaction.signatures.push_back(Base58::decode(
        "jFq4PbbEM1fuPbq5CkUYgzs7a21g6rvFkfLJAUUGP5QMKYhHBE6nB1dqtwaJsABgyUvrR8QjT2Ej73cXNz7Vur1"));
    auto expectedString =
        "3wu6xJSbb2NysVgi7pdfMgwVBT1knAdeCr9NR8EktJLoByzM4s9SMto2PPmrnbRqPtHwnpAKxXkC4vqyWY2dRBgdGG"
        "CC1bep6qN5nSLVzpPYAWUSq5cd4gfYMAVriFYRRNHmYUnEq8vMn4vjiECmZoHrpabBj8HpXGqYBo87sbZa8ZPCxUcB"
        "71hxXiHWZHj2rovx2kr75Uuv1buWXyW6M8uR4UNvQcPPvzVbwBG82RjDYTuancMSAxmrVNR8GLBQNhrCCYrZyte3EW"
        "gEyMQxxfW8T3xNXqnbgdfvFJ3UjRBxXj3hrmv17xEivTjfs81aG2AAi24yiYrk8ep7eQqwDHVSArsrynnwVKVNUcCQ"
        "CnSy7fuiuS7FweFX8DEN1K9BrfecHyWrF15fYzhkmWSs64aH6ZTYHWPv5znhFKYmAuopGwbsBEb2j5p8NS3iJZ2skb"
        "2wi47n1rpLZfoCHWKxNiikkDUJTGQNcSDrGUMfeW5aGubJrCfecPKEo9Wo9kd36iSsxYPYSWNKrz2HTooa1rCRhqjX"
        "D8dyX3bXGV8TK6W2sEgf4JkcDnNoWQLbindcP8XR";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateWithdrawNonceAccountToSelf) {
    auto authorizer = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto nonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    uint64_t value = 10000000;
    auto recentBlockhash = Base58::decode("5W1cvn4AtWsm2NJfJ52DoXdG36pdS3wHY3Gmkut9sENH");
    auto message = LegacyMessage::createWithdrawNonceAccount(authorizer, nonceAccount, authorizer, value,
                                                       recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 5ul);
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 5ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("3JtHs3Ra2LyYCf7a3HFri3ZDa3D4c2G6x3EiyiCDpWjb11QPYfH"
                                               "nU6gqkQaHURdD2aKUzVSBa3JHfzsbiUPi4iUb"));
    auto expectedString =
        "XVVzGHC7YUTfBwbaQ8y8S7ypc8WCgjpnLd8Yxcp2Vg3SAgtYWsfiFBfrwS6CbwSQa5QBLQ997ohS9wHSX8wyzQdkN6"
        "k8ZidSMHdjkHSdFnHGoJBijCv3gURRti2XiaY7aQoKQUPKvwQn31TEKRBgGUQDMtLn9yFqVZ7UMGXtvwvwG7h55VCp"
        "Sb32Saus6rSVfNTAfc4UAmwvkbo2gk9rTF5eT9U81uzG2XsBZYSmyqYJQxqaf5ySQD77mwnJo9TpwCX6KfFXnjqyav"
        "LFMZb8FeEuXcq17MQKf55w6gqiSssXvQov62GtrUVaVqGg5KYvSTa98Kmu2XHPQcJpgCepft32F6wy2bfDUHjooLqL"
        "pp6ZjYqnnWfvVEuSqL6UFjiF";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateWithdrawNonceAccount) {
    auto authorizer = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto nonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto to = Address("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    uint64_t value = 10000000;
    auto recentBlockhash = Base58::decode("5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8");
    auto message =
        LegacyMessage::createWithdrawNonceAccount(authorizer, nonceAccount, to, value, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 6ul);
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 5ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode(
        "MxbTCAUmBLiESDLK1NiK5ab41mL2SpAPKSbvGdYQQD5eKgAJRdFEJ8MV9HqBhDQHdsS2LG3QMQQVJp51ekGu6KM"));
    auto expectedString =
        "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3"
        "S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJ"
        "XMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5"
        "jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqK"
        "NuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferWithDurableNonce) {
    auto from = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto to = Address("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    auto nonceAccount = "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc";
    uint64_t value = 1000;
    auto recentBlockhash = Base58::decode("5ycoKxPRpW2GdD4byZuMptHU3VU5MgUCh6NLGQ2U8VE5");
    auto message = LegacyMessage::createTransfer(from, to, value, recentBlockhash, "", {}, nonceAccount);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 2);
    ASSERT_EQ(message.accountKeys.size(), 5ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    EXPECT_EQ(message.instructions[1].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[1].accounts.size(), 2ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("4c4LhPFsDQdwZnVqKtgDC1GVR63GU5REFvonAW3RF7Quo4b1YbU"
                                               "rsXMmwYCfD3LGrUawThPaAixCUKnoGdMMQ7BQ"));

    auto expectedString =
        "6zRqmNP5waeyartbf8GuQrWxdSy4SCYBTEmGhiXfYNxQTuUrvrBjia18YoCM367AQZWZ5yTjcN6FaXuaPWju7aVZNF"
        "jyqpuMZLNEbpm8ZNmKP4Na2VzR59iAdSPEZGTPuesZEniNMAD7ZSux6fayxgwrEwMWjeiskFQEwdvFzKNHfNLbjoVp"
        "dSTxhKiqfbwxnFBpBxNE4nqMj3bUR37cYJAFoDFokxy23HGpV93V9mbGG89aLBNQnd9LKTjpYFv49VMd48mptUd7uy"
        "rRwZLMneew2Bxq3PLsj9SaJyCWbsnqYj6bBahhsErz67PJTJepx4BEhqRxHGUSbpeNiL7qyERri1GZsXhN8fgU3nPi"
        "Yr7tMMxuLAoUFRMJ79HCex7vxhf7SapvcP";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateNonceAccountWithDurableNonce) {
    auto from = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto newNonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto nonceAccount = "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc";
    uint64_t rent = 10000000;
    auto recentBlockhash = Base58::decode("E6hvnoXU9QmfWaibMk9NuT6QRZdfzbs96WGc2hhttqXQ");
    auto message =
        LegacyMessage::createNonceAccount(from, newNonceAccount, rent, recentBlockhash, nonceAccount);
    EXPECT_EQ(message.header.numRequiredSignatures, 2);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 6ul);
    ASSERT_EQ(message.instructions.size(), 3ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    EXPECT_EQ(message.instructions[1].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[1].accounts.size(), 2ul);
    EXPECT_EQ(message.instructions[2].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[2].accounts.size(), 3ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("5wTTTpoXKTzyhL3XbToCt1y12Cf5pcD8xH5NPmGW2y8Dcshgvs2"
                                               "rY2QQ2b23DvXbheBBr3UTixpT4vYyStgDFPdq"));
    transaction.signatures.push_back(Base58::decode("31nF9z8GG8ZkgUPSHmEqikMKhweo4LwVJpJACoLCEah8mJk469m"
                                               "tFA4DxEeiyJBnZSumgbR1aFS7p4kqX1F42brK"));

    auto expectedString =
        "Fr8FzXoH7h6Xo2La6SE49BEPzRX6f93Qn1cFA5E8n6z2GJtZdTU2BfyYGr1zv21Zkq7h68Z3Q96VnFyUVVd1hTWeq6"
        "tHDamF1JK5L23yEeUXpEWv1KziWvG9XbxfseHUyWETQck7SY2HbsT4KSjRX9suDaBh68Bu8c96CVN7KtgYPhUrKP62"
        "dAMHsf5qo7MESFN8wKJto94ANNCbQMzPmhig9nfiAfvfz9CqV4nbnSiqBGwo2XoyPknDK8RJ1UmA5ptfZ6w6Fy4UmJ"
        "bQZWuZwpUrkEkfgLMNJ36McHkGAnjpyzq9gMtzb33xSjx1BqnbWXkKJdi8HyQAHTtvtqPz7DMsW9qx5fu3TNz6iC8Y"
        "HG2HiynFCRjTtc2aH1rpJ9TLdFQEK8WrhdMFr3yW27cg6NB3JUFopUkDg2k5FwtzFyCdfifwebD7eswVNnqjoZxW59"
        "fHgY3BrBH8uNst8YAQWvRH77y5L6imVmFhezU5JUb5sF58gR1D8eAQhUcHueakZb5FkFCaMeioTpKrVGgcSNe9zkBM"
        "uquoUR3t4MVTiUSLa815qKoBCRmdexQDBt5RQbdQhYyVWn3ovjdhkwDGBU2zywRvottGCcEStQrUrSQDg1tMVKxX5G"
        "3sBtxYf";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, WithdrawNonceAccountToSelfWithDurableNonce) {
    auto authorizer = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto withdrawNonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto nonceAccount = "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc";
    uint64_t value = 10000000;
    auto recentBlockhash = Base58::decode("5EtRPR4sTWRSwNUE5a5SnKB46ZqTJH8vgF1qZFTKGHvw");
    auto message = LegacyMessage::createWithdrawNonceAccount(authorizer, withdrawNonceAccount, authorizer,
                                                       value, recentBlockhash, nonceAccount);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 6ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    EXPECT_EQ(message.instructions[1].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[1].accounts.size(), 5ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("5LsWV8w5CZT4uUDY63rxwcBgGczoVeNtYTPrchP7KZwPpJD7qB2"
                                               "jFEmWVUVtnJGUWn36TGM1bGpnoYGgGWNYZGhA"));
    auto expectedString =
        "3rxbwm6dSX4SbFa7yitVQnoUGWkmRuQtg3V13a2jEAPfZZACCXZX2UFgWFpPqE7KfZSYhd5QE9TLzyikCwcmSBHhKX"
        "jMp4oktQXwRT66YaCK8rJdNzBUuS1D9tgHMkLUWKAR7ZRyWd3XvtQhe7nWD6YF6TRGoKPSuwsZAArBxogA7YddmEUK"
        "Psr2qjSKbjg7X5BbNceFwjEFAiafuizdSt7eGJHB5m9zJeYct8LCanTwJwyEVu1T9HTsgjW9hqHehqhCiHP46KGo63"
        "o7WAoappZvM4EJZemu4GfM6F6H48bPXF2z1QJz17wE6BYeMXfXuGkCRt5jYxrjdKuqvTDYV34X1HjZYUdrkW6mQotW"
        "DY3bS6zyAt784Vwzk2uiA8ytmWMbC24coUVwPSPGwZ92WJ6BpVCCtGDxLzp4CkahRu78UNWzdcEwPG6AUf";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, WithdrawNonceAccountWithDurableNonce) {
    auto authorizer = Address("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto withdrawNonceAccount = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto nonceAccount = "ALAaqqt4Cc8hWH22GT2L16xKNAn6gv7XCTF7JkbfWsc";
    auto to = Address("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    uint64_t value = 10000000;
    auto recentBlockhash = Base58::decode("55F6jiKRh9Wyq7EZuur7nqTYwewNF6LcucuCHoK1N4HV");
    auto message = LegacyMessage::createWithdrawNonceAccount(authorizer, withdrawNonceAccount, to, value,
                                                       recentBlockhash, nonceAccount);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 3);
    ASSERT_EQ(message.accountKeys.size(), 7ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    EXPECT_EQ(message.instructions[1].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[1].accounts.size(), 5ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("2vzszn8rJbYAusA8F3aUo2x1pNeM8Us2uZm6ibeXiHA4XmZA557"
                                               "yafbA3YPJ991Usd8Jbw1ov4rNYNw1VRrVjDnz"));
    auto expectedString =
        "djXotMMJaCX6HXkAshsTVDJfiG8JgryAVzWtPcamqaZGnnZztXbRrf4SvTtyu15ohTDTVMYak6BvwaoQcsGepzNs6k"
        "QE68cgc1LngdtDJLaB5JceX2PmVC1WBK9nLYukdoCgmDrBFiJmkjSsouoFsyXqF7Cgswpq5PqdFqTboTvon6bbQt28"
        "ST5cZ87G832kFMmrLtFXzrQC5MaqKxFYFuiy6rKDKWHh9Xyc1aqTmiWiwPkuoCGsxYmWyAjGmG8mSyjRwuLRLfAmjB"
        "zAgSxFh36qKk5bZiwNwcktg4Ndsg6fdn39XdusfJYKvhWBjatmofk5PJ6eWjFaZbscmpdxcq15yNGRxKaD55e89JSa"
        "2n3pe4wwaJ9SyMaY1trvu1YXx6NxpfhPDEBniMnSE32Ko3tpoQhmxLoMVXbz8pngCYNZUwTbPYmT9K9DXysJYsfnk9"
        "FRoxVKJugskV91cyjKX1TeW4iosedQbiXdy";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateTokenAccountAndTransfer) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = Address("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = Address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    auto recipientTokenAddress = Address("BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    auto recentBlockhash = Base58::decode("AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D");
    auto message = LegacyMessage::createTokenCreateAndTransfer(signer, recipientMainAddress, token,
                                                         recipientTokenAddress, senderTokenAddress,
                                                         amount, decimals, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 6);
    ASSERT_EQ(message.accountKeys.size(), 9ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(),
              "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    ASSERT_EQ(message.instructions[0].accounts.size(), 7ul);
    EXPECT_EQ(message.instructions[1].programId.string(),
              "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[1].accounts.size(), 4ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode(
        "pL1m11UEDWn3jMkrNMqLeGwNpKzmhQzJiYaCocgPy7vXKA1tnvEjJbuVq9hTeM9kqMAmxhRpwRY157jDgkRdUZw"));
    auto expectedString =
        "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7v"
        "SHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W"
        "2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXs"
        "GAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZM"
        "uGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4d"
        "cH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTa"
        "AKX32wo5xoMj9wJmmtcU6YbXN4KgZ";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateTokenAccountAndTransferWithDurableNonce) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = Address("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = Address("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    auto recipientTokenAddress = Address("93hbN3brRjZqRQTT9Xx6rAHVDFZFWD9ragFDXvDbTEjr");
    auto nonceAccount = "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ";
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    auto recentBlockhash = Base58::decode("AaYfEmGQpfJWypZ8MNmBHTep1dwCHVYDRHuZ3gVFiJpY");
    auto message = LegacyMessage::createTokenCreateAndTransfer(
        signer, recipientMainAddress, token, recipientTokenAddress, senderTokenAddress, amount,
        decimals, recentBlockhash, "", {}, nonceAccount);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 7);
    ASSERT_EQ(message.accountKeys.size(), 11ul);
    ASSERT_EQ(message.instructions.size(), 3ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    EXPECT_EQ(message.instructions[1].programId.string(),
              "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    ASSERT_EQ(message.instructions[1].accounts.size(), 7ul);
    EXPECT_EQ(message.instructions[2].programId.string(),
              "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[2].accounts.size(), 4ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode(
        "Gw1REivWi3j3cm17evPbxcdoJ1YY1jC7xcQERcmAWCtLx8WKsFoDDWTun4WYfUqSzEfuKjxiYKPvv2eAJWs7N1b"));
    auto expectedString =
        "388uZws6GfA9aiH1LPsYBijGBEfLEgqe6q5NWVYhsmjXjrgZB4cScGuvja6nBL3i6qg6HA4a8ptW6aHsNKVdcBWKhj"
        "ZjaTPH5heEThzwEsMDfnH2PWAUbqfiFgMZQRCkhyCj57hGUR7hBFPELfz3DBw5qMz1tnP9gU6KTqHUomu5UaadLHb2"
        "v5mbgTRcsMm3yDp2tzMwrp53VqvFNmHSau4ot4kdNL1jqEJC68Fj4ku6fMQaFSPyAeLQRF45ofYsFCa65fmtb4gBpq"
        "WUdqWLv5Dy6xQUQUDsin8qpEVds6unXw5f63UjZeD7XQdC6Vz5aq3e6P9ug8L41xc1rbuRU3Kp4arUKyqTsHMQ2dxM"
        "hPwEJLkHd4mFqqUWpYFTdfLFaNGU22hEkvP1esHUzaaGDmzAozbS96oaFw2jbHRRJtL8VjoA1aokGFFThM6M6mExuy"
        "8GhUXdGjxDFU83Dan1URmHMGBRC4J9RMZip9s1sktJw9Rj5Std9KVT8T7m4MxTVTx4QoBw6KAf6PgNHyHPtZSc7kzo"
        "CxDYNo2Myxvy8D95zk9YMp1MxeZXTDQ2aJuhWvfHhhrwgcQasAxRzbnJ9oehebVUNEcZEFsfnCgYuUmxWUemoKZnE1"
        "bNMCvERVkT5fKQ36e1rt5vTC2iES9jzr3hDC1Pk1";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, AdvanceNonceAccount) {
    auto authorizer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto nonceAccountAddress = Address("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto recentBlockhash = Base58::decode("4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck");
    auto message = LegacyMessage::advanceNonceAccount(authorizer, nonceAccountAddress, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 2);
    ASSERT_EQ(message.accountKeys.size(), 4ul);
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "11111111111111111111111111111111");
    ASSERT_EQ(message.instructions[0].accounts.size(), 3ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode("2gwuvwJ3mdEsjA8Gid6FXYuSwa2AAyFY6Btw8ifwSc2SPsfKBnD"
                                               "859C5mX4tLy6zQFHhKxSMMsW49o3dbJNiXDMo"));
    auto expectedString =
        "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKw"
        "tKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9a"
        "cwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73"
        "FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferTokenTransactionWithExternalPayer) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto feePayer = Address("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    auto token = Address("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = Address("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientTokenAddress = Address("AZapcpAZtEL1gQuC87F2L1hSfAZnAvNy1hHtJ8DJzySN");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    auto recentBlockhash = Base58::decode("GwB5uixiTQUG2Pvo6fWAaNQmz41Jt4WMEPD7b83wvHkX");
    auto message =
        LegacyMessage::createTokenTransfer(signer, token, senderTokenAddress, recipientTokenAddress,
                                     amount, decimals, recentBlockhash, "", {}, "", feePayer.string());
    EXPECT_EQ(message.header.numRequiredSignatures, 2);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 2);
    ASSERT_EQ(message.accountKeys.size(), 6ul);
    ASSERT_EQ(message.instructions.size(), 1ul);
    EXPECT_EQ(message.instructions[0].programId.string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[0].accounts.size(), 4ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    auto signature1 = Base58::decode(
        "2LbovMDuKoR2LFcV5NbK9bCQZcTG99W6VE1urvdWFWvRhNg9ocDGhayyeBGisoqZgYZtcD3b6LJDTmPx9Gp3T6qd");
    auto signature2 = Base58::decode(
        "2hUHMS9rwbUbXrpC7sK7utL2M4soTyQ7EX3sBYvdee9wraJvYoPH2XjovHDn8eRFY8z5uCx9DCj2Zfjpmzfa81Db");
    transaction.signatures.push_back(signature1);
    transaction.signatures.push_back(signature2);

    auto expectedString =
        // https://explorer.solana.com/tx/2LbovMDuKoR2LFcV5NbK9bCQZcTG99W6VE1urvdWFWvRhNg9ocDGhayyeBGisoqZgYZtcD3b6LJDTmPx9Gp3T6qd?cluster=devnet
        "qjgNVBmoPDHNTN2ENQfxNVE57jWXpqdmu5GQX4msA7iK8ZRAnKpvbusQagv8CZGyNYti23p9jBsjTSx75ZU26UW5vgC8D88pusW8W5dp1ERo5DSfurMSYJ6afgQHdcuzn7exb8znSm6uV4y1cWgBRcuAGdg3wRpVhP8HEB1EeKgzjYVWvMSy6yR7qVrSL6BxHG6eiAMyahLFbEt4qBqLEdxxY7Dt4DyydVYmG2ZVtheaMHD3ACwCjpyPLXj399wxSgGXQQFGtzEJQw9awVezmJ4wZk6W4dDpXQvdKYaqUvwTwRZsQB5o2iekPWZXR9xvHiMLjMVBPzYgcU14ZSaCbqSNVv2pAJxP1sMvxZMNMzZPttPxCsDDGq9biC7exXwzesXSnZ3rsgEYeZtkUiBHAxR4rYqBpA6VzLs1bPx8MPTvr9mhNi2ezMBbg2nEfHV6Fz7H7rEY2g3jDtRz35Vmgits8s9RKi3kb73WtGUieRiXjiqkNhpvKkST1oEYRQ9";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateTokenAccountAndTransferWithExternalFeePayer) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto feePayer = Address("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    auto token = Address("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = Address("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = Address("E54BymfdhXQtzDSGtgiJayauEMdB2gJjPqoDjzfbCXwj");
    auto recipientTokenAddress = Address("Ay7G7Yb7ZCebCQdG39FxD1nFNDtqFWJCBgfd5Ek7DDcS");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    auto recentBlockhash = Base58::decode("EsnN3ksLV6RLBW7qqgrvm2diwVJNTzL9QCuzm6tqWsU8");
    auto message = LegacyMessage::createTokenCreateAndTransfer(
        signer, recipientMainAddress, token, recipientTokenAddress, senderTokenAddress, amount,
        decimals, recentBlockhash, "", {}, "", feePayer.string());
    EXPECT_EQ(message.header.numRequiredSignatures, 2);
    EXPECT_EQ(message.header.numReadOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numReadOnlyUnsignedAccounts, 6);
    ASSERT_EQ(message.accountKeys.size(), 10ul);
    ASSERT_EQ(message.instructions.size(), 2ul);
    EXPECT_EQ(message.instructions[0].programId.string(),
              "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    ASSERT_EQ(message.instructions[0].accounts.size(), 7ul);
    EXPECT_EQ(message.instructions[1].programId.string(),
              "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[1].accounts.size(), 4ul);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    transaction.signatures.push_back(Base58::decode(
        "7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2"));
    transaction.signatures.push_back(Base58::decode(
        "3n7RHTCBAtnFVuDn5eRbyQB24h6AqajJi5nGMPrfnUVFUDh2Cb8AoaJ7mVtjnv73V4HaJCzSwCLAj3zcGEaFftWZ"));

    // https://explorer.solana.com/tx/7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2?cluster=devnet
    auto expectedString =
        "5sxFkQYd2FvqRU64N79A6xjJKNkgUsEEg2wKgai2NiK7A7hF3q5GYEbjQsYBG9S2MejwTENbwHzvypaa3D3cEkxvVTg19aJFWdCtXQiz42QF5fN2MuAb6eJR4KHFnzCtxxnYGtN9swZ5B5cMSPCffCRZeUTe3kooRmbTYPvSaemU6reVSM7X2beoFKPd2svrLFa8XnvhBwL9EiFWQ9WhHB2cDV7KozCnJAW9kdNDR4RbfFQxboANGo3ZGE5ddcZ6YdomATKze1TtHj2qzJEJRwxsRr3iM3iNFb4Eav5Q2n71KUriRf73mo44GQUPbQ2LvpZKf4V6M2PzxJwzBo7FiFZurPmsanT3U5efEsKnnueddbiLHedc8JXc1d3Z53sFxVGJpsGA8RR6thse9wUvaEWqXVtPbNA6NMao9DFGD6Dudza9pJXSobPc7mDHZmVmookf5vi6Lb9Y1Q4EgcEPQmbaDnKGGB6uGfZe629i3iKXRzAd2dB7mKfffhDadZ8S1eYGT3dhddV3ExRxcqDP9BAGQT3rkRw1JpeSSi7ziYMQ3vn4t3okdgQSq6rrpbPDUNG8tLSHFMAq3ydnh4Cb4ECKkYoz9SFAnXACUu4mWETxijuKMK9kHrTqPGk9weHTzobzCC8q8fcPWV3TcyUyMxsbVxh5q1p5h5tWfD9td5TZJ2HEUbTop2dA53ZF";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

} // namespace TW::Solana
