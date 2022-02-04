// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/Address.h"
#include "Solana/Transaction.h"
#include "Solana/Program.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include "BinaryCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

TEST(SolanaTransaction, TransferMessageData) {
    auto from = Address("6eoo7i1khGhVm8tLBMAdq4ax2FxkKP4G7mCcfHyr3STN");
    auto to = Address("56B334QvCDMSirsmtEJGfanZm8GqeQarrSjdAb2MbeNM");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
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
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    Signature signature(
        "46SRiQGvtPb1iivDfnuC3dW1GzXkfQPTjdUyvFqF2sdPvFrsfx94fys2xpNKR6UiAj7RgKWdJG6mEfe85up6i1JT");
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
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    Signature signature(
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
    const Solana::Hash recentBlockhash("11111111111111111111111111111111");
    const auto memo = "HelloSolana73";
    std::vector<Address> references = {Address("GaeTAQZyhVEocTC7iY8GztSyY5cBAJTkAUUA1kLFLMV")};
    auto transaction = Transaction(from, to, 42, recentBlockhash, memo, references);
    const Signature signature("3CFWDEK51noPJP4v2t8JZ3qj7kC7kLKyws9akfHMyuJnQ35EtzBptHqvaHfeswiLsvUSxzMVNoj4CuRxWtDD9zB1");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString = "3pzQEdU38uMQgegTyRsRLi23NK4YokgZeSVLXYzFB7HShqZZH8FdBLqj6CeA2d2L8oR9KF2UaJPWbE8YBFmSdaafegoSXJtyj7ciwTjk5ieSXnPXtqH1TEcnMntZATg7gKpeFg6iehqdSUtZuQD1PGmHA1TrzzqLpRSRrc1sqPz8EpSJcQr1Y41B1XCEAfSJDfcuNKrfFrnQaVtRz6tseQfd9uXNYNuR1NQSepWdav5wQiohLUMDiZtxuwb7FQkQ68WE1FDsHmd4JpbWKmDEjz7HFyQY37vf6NBJyX5qWJpFMSg5qGKWvhNCDM32yM4A7HhPeoTWEywE5CXcNmQqdbRt4BzF1A11uqv4etWj";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, StakeSerializeTransactionV2) {
    auto signer = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromRecentBlockhash(signer, recentBlockhash, programId);
    auto message = Message::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);
    Signature signature(
        "2GXRrZMMWTaY8ycwFTLFojAVZ1EepFqnVGW7b5bBuuKPiVrpaPXMAwyYsSmYc2okCa1MuJjNguu1emSJRtZxVdwt");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString = "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM92cXrzUhKGWtQ6cATeQh8i8ZfHpmjyuik7Eg3SQ4sa2543CmcozzjmTTWThThuLdvFCZJzBeRBFWLujqjbs5mA66XVtiDwsEqYByznoo4BN45XUHxnZebmPfo4hi5sf27UkhzPHik371BGxbVDexQp4y5nCEHy8ybfNCvMPLr2SEBiWSifwPkmwYN3hGCkBpqLoHCCiRcyJuRHW8hSDFR4JPQ3Xe3FGfpgbayaawZigUnFuPGSpoGrURZRoLCzc6V4ApqcJbmzFhg5zJz2yTX5GvQSYWLFnTKbPYcgBNpdyMLJTivonrKtgkfdymZVKjDwnHUApC7WD4L9mqzTf1dzR61Fxhu3Rdh8ECiVEDgB1wkWZWkTKEdANmtaYLKCMUs3n4VhuZbSFLEiTg7yRWM2pjBgiBB4qywbF7SE75UtzSFCaDnn27mKkxRBqZEGEgfpEoK2AxjsiCZEZxfLeyZFbwWe7xasmNiXr6CnAQhwsmxJk79h7SYmaje76JLxHVX5gbQmLfn5bc1xthS3YhteSovQ8xYq1jiHCfsXRwbxKrNA4kVMiSa6spoU9AhFL8cDAZjAqoU4YRwBihZVhXSFCRnYAK8FabzEv1M44EeHX1sfMG8T1U7y3DEjom7jv6rqZfLumWpbXDTqanB7zTbTjGyDcBBf21edjpZzBZ7osS5fTVYJ5mZBSvjjhuGkUgZZWgYozAKvdyyrJH6UdcPvNm2XgMRYJxqyCin1zhCeQ25vK1H8Jj";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, StakeSerializeTransactionV1) {
    auto signer = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromValidatorSeed(signer, voteAddress, programId);
    auto message = Message::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);
    Signature signature(
        "2GXRrZMMWTaY8ycwFTLFojAVZ1EepFqnVGW7b5bBuuKPiVrpaPXMAwyYsSmYc2okCa1MuJjNguu1emSJRtZxVdwt");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedString = "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM7jtGoYQFb7eFVbujUb6tbeQ9UqLJq1sJ7uMZ4wqecmQPouDmJnpmJk4CHMzLnPNTwyGmGio6sYAS3xKZ7DFXvjwGPuD8PyYHSfdPro1p3jy9igPZNAbQ6fgK7LL3sERKCUdvPy7k14xgHbtsVy2mu54LY5c8F9sFst2uzQiTsXRTdjPFAyCVwB5pccNVotCrJ6Q2aKSC2D2knVH7LgWzSBMSreJG75xyATneu922wSzz7QJDieqhDtdePtSbPtoCdtPNmDfdaeDbHxVAxMios9F7RSRmH2dq86NfWDvF8TuEbYY7gPnygz6jGvwfqSSoSnY8TnUhhceC7wJSMc8Hcf1kyfi8dqKm7rF57YjnrQoMmL5bWqJLKoJtdfFu24ceQN21k38U2tUMWJaBASWukgTJUbNSCemNPZt4P3cNbeB3L1wBj4GEYXVTbTFYKME5JscU5RsnkMJZZ1PgxSi63HT4hwQLok4c18UdJgzMFu1njpZj3Sw76mwV3ea7ruHnP4yyM3YhUGbNjpx5fAcnvdLcXChdsgeUpJhutME6V86Rk2EEskoJeD3qNWi3hvfQx172hZRHyKyr29Ts1uLQxcMJq7oeQUxvTfXxSe6cBuPJUDFkAET3qpS7rWM7rvQQ8rDLQF5QvcJnrYTq12pVgw28WXdgi45811a7DWHGuwHRj5FJdLQAHkKe4EXVeTCdbYHREVwuyTJgAvb8SXjRE5a9n3qpRDr7iEd5UDZKB5HgvMsMYWh5";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, CreateTokenAccountTransaction) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto tokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    Solana::Hash recentBlockhash("9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");
    auto message = Message::createTokenCreateAccount(signer, signer, token, tokenAddress, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numCreditOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numCreditOnlyUnsignedAccounts, 5);
    ASSERT_EQ(message.accountKeys.size(), 7);
    EXPECT_EQ(message.accountKeys[0].string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.accountKeys[1].string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    EXPECT_EQ(message.accountKeys[2].string(), "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(message.accountKeys[3].string(), "11111111111111111111111111111111");
    EXPECT_EQ(message.accountKeys[4].string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    EXPECT_EQ(message.accountKeys[5].string(), "SysvarRent111111111111111111111111111111111");
    EXPECT_EQ(message.accountKeys[6].string(), "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    EXPECT_EQ(Base58::bitcoin.encode(message.recentBlockhash.bytes), "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");
    ASSERT_EQ(message.instructions.size(), 1);
    EXPECT_EQ(message.instructions[0].programId.string(), "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
    ASSERT_EQ(message.instructions[0].accounts.size(), 7);
    EXPECT_EQ(message.instructions[0].accounts[0].account.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.instructions[0].accounts[1].account.string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    EXPECT_EQ(message.instructions[0].accounts[2].account.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    EXPECT_EQ(message.instructions[0].accounts[3].account.string(), "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(message.instructions[0].accounts[4].account.string(), "11111111111111111111111111111111");
    EXPECT_EQ(message.instructions[0].accounts[5].account.string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    EXPECT_EQ(message.instructions[0].accounts[6].account.string(), "SysvarRent111111111111111111111111111111111");
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    Signature signature("3doYbPs5rES3TeDSrntqUvMgXCDE2ViJX2SFhLtiptVNkqPuixXs1SwU5LUZ3KwHnCzDUth6BRr3vU3gqnuUgRvQ");
    transaction.signatures.push_back(signature);

    auto expectedString =
        // test data obtained from spl-token create-account
        "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVnH2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtCjAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzjB2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2TN3xXwu1";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaTransaction, TransferTokenTransaction_3vZ67C) {
    auto signer = Address("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto senderTokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    auto recipientTokenAddress = Address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    Solana::Hash recentBlockhash("CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi");
    auto message = Message::createTokenTransfer(signer, token, senderTokenAddress, recipientTokenAddress, amount, decimals, recentBlockhash);
    EXPECT_EQ(message.header.numRequiredSignatures, 1);
    EXPECT_EQ(message.header.numCreditOnlySignedAccounts, 0);
    EXPECT_EQ(message.header.numCreditOnlyUnsignedAccounts, 2);
    ASSERT_EQ(message.accountKeys.size(), 5);
    ASSERT_EQ(message.instructions.size(), 1);
    EXPECT_EQ(message.instructions[0].programId.string(), "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    ASSERT_EQ(message.instructions[0].accounts.size(), 4);
    auto transaction = Transaction(message);
    transaction.signatures.clear();
    Signature signature("3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg");
    transaction.signatures.push_back(signature);

    auto expectedString =
        // https://explorer.solana.com/tx/3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg
        // test data obtained from spl-token transfer
        "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jRe2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvPiPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMqDV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnotcrH7pUa94xCVvCPPaomF";
    EXPECT_EQ(transaction.serialize(), expectedString);
}
