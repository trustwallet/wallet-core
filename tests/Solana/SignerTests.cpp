// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/Signer.h"
#include "Solana/Transaction.h"
#include "Solana/Program.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

TEST(SolanaSigner, CompiledInstruction) {
    const auto privateKey0 =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    const auto publicKey0 = privateKey0.getPublicKey(TWPublicKeyTypeED25519);
    const auto address0 = Address(publicKey0);
    ASSERT_EQ(Data(publicKey0.bytes.begin(), publicKey0.bytes.end()),
              Base58::bitcoin.decode("GymAh18wHuFTytfSJWi8eYTA9x5S3sNb9CJSGBWoPRE3"));
    const auto privateKey1 =
        PrivateKey(Base58::bitcoin.decode("GvGmNPMQLZE2VNx3KG2GdiC4ndS8uCqd7PjioPgm9Qhi"));
    const auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeED25519);
    const auto address1 = Address(publicKey1);
    ASSERT_EQ(Data(publicKey1.bytes.begin(), publicKey1.bytes.end()),
              Base58::bitcoin.decode("2oKoYSAHgveX91917v4DUEuN8BNKXDg8KJWpaGyEay9V"));
    Address programId("11111111111111111111111111111111");

    std::vector<Address> addresses = {address0, address1, programId};

    std::vector<AccountMeta> instrAddresses = {
        AccountMeta(address1, false, false),
        AccountMeta(address0, false, false),
        AccountMeta(programId, false, false),
        AccountMeta(address1, false, false),
        AccountMeta(address0, false, false),
    };
    Data data = {0, 1, 2, 4};
    Instruction instruction(programId, instrAddresses, data);

    auto compiledInstruction = CompiledInstruction(instruction, addresses);

    EXPECT_EQ(compiledInstruction.programIdIndex, 2);
    ASSERT_EQ(compiledInstruction.accounts.size(), 5);
    EXPECT_EQ(compiledInstruction.accounts[0], 1);
    EXPECT_EQ(compiledInstruction.accounts[1], 0);
    EXPECT_EQ(compiledInstruction.accounts[2], 2);
    EXPECT_EQ(compiledInstruction.accounts[3], 1);
    EXPECT_EQ(compiledInstruction.accounts[4], 0);
    ASSERT_EQ(compiledInstruction.data.size(), 4);
}

TEST(SolanaSigner, CompiledInstructionFindAccount) {
    Address address1 = Address(parse_hex("0102030405060708090a0102030405060708090a0102030405060708090a0101"));
    Address address2 = Address(parse_hex("0102030405060708090a0102030405060708090a0102030405060708090a0102"));
    Address address3 = Address(parse_hex("0102030405060708090a0102030405060708090a0102030405060708090a0103"));
    Address programId("11111111111111111111111111111111");
    Instruction instruction(programId, std::vector<AccountMeta>{
        AccountMeta(address1, true, false),
        AccountMeta(address2, false, false),
    }, Data{1, 2, 3, 4});
    std::vector<Address> addresses = {
        address1,
        address2,
        programId,
    };
    CompiledInstruction compiledInstruction = CompiledInstruction(instruction, addresses);
    ASSERT_EQ(compiledInstruction.findAccount(address1), 0);
    ASSERT_EQ(compiledInstruction.findAccount(address2), 1);
    ASSERT_EQ(compiledInstruction.findAccount(programId), 2);
    // negative case
    try {
        compiledInstruction.findAccount(address3);
        FAIL() << "Missing expected exception";
    } catch (...) {
        // ok
    }
}

TEST(SolanaSigner, SingleSignTransaction) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"));

    const auto from = Address(publicKey);
    auto to = Address("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "5T6uZBHnHFd8uWErDBTFRVkbKuhbcm94K5MJ2beTYDruzqv4FjS7EMKvC94ZfxNAiWUXZ6bZxS3WXUbhJwYNPWn");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
        "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
        "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
        "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";
    ASSERT_EQ(transaction.serialize(), expectedString);

    const auto additionalPrivateKey =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    signerKeys.push_back(additionalPrivateKey);
    try {
        Signer::sign(signerKeys, transaction);
        FAIL() << "publicKey not found in message.accountKeys";
    } catch (std::invalid_argument const& err) {
        EXPECT_EQ(err.what(), std::string("publicKey not found in message.accountKeys"));
    }
}

TEST(SolanaSigner, SignTransactionToSelf) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu"));

    const auto from = Address(publicKey);
    auto to = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "3CFWDEK51noPJP4v2t8JZ3qj7kC7kLKyws9akfHMyuJnQ35EtzBptHqvaHfeswiLsvUSxzMVNoj4CuRxWtDD9zB1");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "EKUmihvvUPKVN4GSCFwZRtz8WiyAuPvthW69Smo19SCjcPLQ6T7EVZd1HU71WAoe1bfgmPNS5JhU7ZLA9XKG3qbZqe"
        "EFJ1xmRwW9ZKw8SKMAL6VRWxp87oLu7PSmf5b8R34vCaww3XLKtZkoP49a7TUK31DqPN5xJCceMB3BZJyaojQaKU8n"
        "UkzSGf89LY6abZXp9krKAebvc6bSMzTP8SHSvbmZbf3VtejmpQeN9X6e7WVDn6oDa2bGT";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, MultipleSignTransaction) {
    const auto privateKey0 =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    const auto publicKey0 = privateKey0.getPublicKey(TWPublicKeyTypeED25519);
    const auto address0 = Address(publicKey0);
    ASSERT_EQ(Data(publicKey0.bytes.begin(), publicKey0.bytes.end()),
              Base58::bitcoin.decode("GymAh18wHuFTytfSJWi8eYTA9x5S3sNb9CJSGBWoPRE3"));
    const auto privateKey1 =
        PrivateKey(Base58::bitcoin.decode("GvGmNPMQLZE2VNx3KG2GdiC4ndS8uCqd7PjioPgm9Qhi"));
    const auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeED25519);
    const auto address1 = Address(publicKey1);
    ASSERT_EQ(Data(publicKey1.bytes.begin(), publicKey1.bytes.end()),
              Base58::bitcoin.decode("2oKoYSAHgveX91917v4DUEuN8BNKXDg8KJWpaGyEay9V"));

    Data data = {0, 0, 0, 0};
    Address programId("11111111111111111111111111111111");
    std::vector<AccountMeta> instrAddresses = {
        AccountMeta(address0, true, false),
        AccountMeta(address1, false, false),
    };
    Instruction instruction(programId, instrAddresses, data);
    std::vector<Instruction> instructions = {instruction};

    MessageHeader header = {2, 0, 1};
    std::vector<Address> accountKeys = {address0, address1, programId};
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    Message message;
    message.header = header;
    message.accountKeys = accountKeys;
    message.recentBlockhash = recentBlockhash;
    message.instructions = instructions;
    message.compileInstructions();

    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    // Sign order should not matter
    signerKeys.push_back(privateKey1);
    signerKeys.push_back(privateKey0);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature0(
        "37beWPhNMfWUz75Tb24TX3PCS89FZscbCgwwLpFnzVfZYPqDpAWruvqzc9eeQYft35H23Vm9Tv1dPwEKWT3vAVPb");
    expectedSignatures.push_back(expectedSignature0);
    Signature expectedSignature1(
        "5NxQshVaAXtQ8YVdcBtCanT62KbxnRfhubjGndFvetgn9AiaoLVZvRGutR5D7FJebRxq8bd6nQXn59LFzavEUrdQ");
    expectedSignatures.push_back(expectedSignature1);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "oL2CmkcP9xf2DiU7eo6hh3JdHnX3NGjunheXYo6SjVchzc8LtFJpPs4jccWUd7oPZUPQNTcR7Ee"
        "Hn259ror9A7aXgJdP4djhntoD8irF1kuBZCj7pubtoWfiAKzagSL4hChQsTSe7e9jaGtoXu58mP"
        "HCMKTz55TLjhdmCj7ixoWRowWEzkrF49MxXnurb4yf6ASru1XdHPFn3DdzkRHgypYwvRM6ci8p2"
        "7trQvXFukhWX6qG6JkxqsWYSzACcAAGGWfAxSi63Yx1RxkxGUzyxy5f2thQhWZ6Nx6pR1im65yV"
        "YMYPXj94kgtHxXw9h5V4p7xSAwRpmhw4jewYyQVX4jmnfro3gFNdX9AqpqMs4uGHA4rZM";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignUpdateBlockhash) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("G4VSzrknPBWZ1z2YwUnWTxD1td7wmqR5jMPEJRN6wm8S"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("41a5jYky56M6EWDsFfLaZRxoRtgAJSRWxJnxaJNJELn5"));

    const auto from = Address(publicKey);
    auto to = Address("4iSnyfDKaejniaPc2pBBckwQqV3mDS93go15NdxWJq2y");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Signer::sign(signerKeys, transaction);

    Solana::Hash newBlockhash("GgBaCs3NCBuZN12kCJgAW63ydqohFkHEdfdEXBPzLHq");
    Signer::signUpdateBlockhash(signerKeys, transaction, newBlockhash);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "5AFhXjvGdENXCAe9MPvUA2qjoL4XtZwZKG7kK2HmZf1ibpxjx5kzogHZjN39uYB9J33UFJN15KhSggBZhzyNQmta");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "62ABadDCoPfGGRnhLoBhfcPekMHyN5ee8DgTY8wD4iwKDjyFAsNbsaahTcqMWxmwa61q9iAGCQB"
        "v1bETcYzWsTwLKMVGLoEpwqA84mPjqHyr5sQD5dcghyQiQ1ckYNub9K7s8FspVwwowK8gJG69xe"
        "DEaqi7G1zrChBVbQYTmVUwJETyDmP1Vs8QU3CaxBs8qwcxoziU52KWLBpRj9o38QVBdxJtJ7hig"
        "hgPKJubfqUfTWdN94PzqEfyPqwoCpFD39nvBn8C5xe1caPKivicg6U7Lzm9s8RYTLCEB";
    ASSERT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignRawMessage) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("GjXseuD8JavBjKMdd6GEsPYZPV7tMMa46GS2JRS5tHRq"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("3BocAWPm1oNXN5qkAV4QeDUmAPpkTcN1rrmCMWAfsXJY"));

    auto rawMessageData =
        "01000203207be13c43c4528592eaf3fd34e064c641c5be3cb6691877d7ade94dff36734108eaea30723c33b525"
        "07bc54024910612f885e4c80c10b99a047fd42c0acbace00000000000000000000000000000000000000000000"
        "000000000000000000000404040404040404040404040404040404040404040404040404040404040404010202"
        "00010c020000002a00000000000000";

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Data rawTransaction = Signer::signRawMessage(signerKeys, parse_hex(rawMessageData));

    auto expectedHex =
        "016e7f8349977b482bccf0bfc202ad917295803831e59ccb865b97d657464791ebfe3336879b84b9f165e464a3"
        "4751fe30d54b01f3c9f33f969aafe1e85951b10901000203207be13c43c4528592eaf3fd34e064c641c5be3cb6"
        "691877d7ade94dff36734108eaea30723c33b52507bc54024910612f885e4c80c10b99a047fd42c0acbace0000"
        "000000000000000000000000000000000000000000000000000000000000040404040404040404040404040404"
        "040404040404040404040404040404040401020200010c020000002a00000000000000";
    ASSERT_EQ(hex(rawTransaction), expectedHex);
}

TEST(SolanaSigner, SignDelegateStakeV2) {
    const auto privateKeySigner =
        PrivateKey(Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    ASSERT_EQ(signer.string(), "zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");

    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromRecentBlockhash(signer, recentBlockhash, programId);

    auto message = Message::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature("58iogHzSJZmvTxi71W8k2yZXSPVfGAgtgqrk1RaBtfVFewU9yiJCkvSF1Hhjyax5DuexzR7ryWZDAWKQ73pyqvMs");
    expectedSignatures.push_back(expectedSignature);
    EXPECT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString = "j24mVM9Zgu5vDZhPLGGuCRXQnP9djNtxdHh4txN3S7dwJsNNL5fbhzGpPgSUAcLGoMVCfF9TuqTYfpfJnb4sJFe1ahM8yPL5HwuKL6py5AZJFi8SWx9fvaVB699dCPo1GT3JoEBLPCZ9o2jQtnwzLkzTYJnKv2axqhKWFE2sz6TBA5J39eZcjMFUYgyxz6Q5S4MWqYQCb8UET2NAEZoKcfy7j8N25WXL6Gj4j3hBZjpHQQNaGaNEprEqyma3ZuVhpGiCALSsuzVLX3wZVo4icXwe952deMFA4tH3BK1jcSQCgfmcKDJ9nd7bdrnUUs4BoMdF1uDZB5LxE2UH8QiqtYvaUcorF4SJ3gPxM5ykbyPsNK1cSYZF9NMpW2GofyC17eELwnHQTQB2kqphxJZu7BahvkwiDPPeeydiXAkBspJ3nc3PCBujv6WJw22ZHw5j6zAP8ZGnCW44pqtWD5qifF9tTKhySKdANNiWifs3tSCCPQqjfJXu14drNinR6VG8rJxS1qgmRYiRQUa7m1vtoaZFRN5qKUeAfoFKkAVaNnMdwgsNqNH4dqBodTCJFs1LkYwhgRZdZGbwXTn1j7vpR3DSnv4g72i2H556srzK53jdUmdv6yfxt516XDSshqZtHnKZ1tudxKjBXwsqT3imDiZFVka9wKWUAYMCi4XZ79CY6Xpsd9c18U2e9TCngQmgkTATFgrqysfraokNffgqWxvsPMugksbvbPjJs3iCzByvphkC9p7hCf6LwbeF8XnVB91EAgRDA4VLE1f9wkcq5zjy879YWJ4r516h3PQszTz1EaJXNAXdbk5Em7eyuuabGP1Q3nijFTL2yhMDsXpgrjAuEAABNxFMd4J1JRMaic615mHrhwociksrsfQK";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignDelegateStakeV1) {
    const auto privateKeySigner =
        PrivateKey(Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    ASSERT_EQ(signer.string(), "zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");

    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = StakeProgram::addressFromValidatorSeed(signer, voteAddress, programId);

    auto message = Message::createStake(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature("gDPbnakbktrASmnUwKGpmftvQRbcyAvxyAyVXq3oVLfAdTPDqY8hhLPHTgidEZGWcmiaXnEyKg2GQLkkAh3JYr3");
    expectedSignatures.push_back(expectedSignature);
    EXPECT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString = "TKPiN35HzeD3zdwxDFvnkgoqud7CZsda15JkBwM4nDpr623rM7MZsH6QvMMyKpiz7MeRNTrfyHkRLQSBT9Tbg2mgTdfrbhhqeF3Suu5ECphqn8DFYPoMnFzeg5u9gaqevfjhuizzeo2YDJF8aVGy1pez8gMbp5vHz1SuvQUgfcvFctggUMwNiJorSmmp3N6TzQSd38CZrA8ZLhaJjuwDwVMjmj18rGTV1gkX19L7byTFrus2vNvPeUa2AawwUnFpYMPgvCKkHTrpnjvypjoLof9yMUFQ5M1S3Ntv53KJyXwXq6ejJnBDtisnDcdMDNSZp3VeKz6XCr8XVM5xNVh3LX12V4kc3ueqkokYJLP1JmuhA3nNZA1G5KTNno93HUoBkEa1x5h3haoCSgmQC97LoJbJM6B6C2NbaDj2J6iiTaVQdin4He4Jpj575WDhNTqsLjzFUHPUHQF1CRnuss8UpVyMsa4kdVqCDQGeh5DKbkikgcB8GKPBuC91DRxGEqgoygNsu5nnQy4o3YAJnBBK6HsKxpdjbYD8wCUdLw8muhjpEqeBTPShEaogm9zfehidiCcnxbeoX3gmW8oH9gpWoX7GrkJgF6Wn7iWohmrzqzAjoBz8hpeY5nkkhHrf9iswVGMpakdLGy3YxkGJVpsW8KJACwEKXGLq8SVLtXSUHG8EP16zfYHxKjkCSs8PkdFsA5esxsxppPTVZivuEPqJ5og55aNmugdNDrAFYWdcH1Q4rm7BXN6oHECdz2yY4HFVWh9u592oqozt2gQKu3vmhcNFzzQe1xgs6zKSv38kSGTnipd7Hx2VL3qNAR6XBRiwAi226qSTzxi6R82p7cMB7TMy6fk5AZ3sXDSXFNJ9S5SSU1V63ruw75QMtVio";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignCreateTokenAccount) {
    const auto privateKeySigner =
        PrivateKey(Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    EXPECT_EQ(signer.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto tokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    Solana::Hash recentBlockhash("9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");

    auto message = Message::createTokenCreateAccount(signer, signer, token, tokenAddress, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature("3doYbPs5rES3TeDSrntqUvMgXCDE2ViJX2SFhLtiptVNkqPuixXs1SwU5LUZ3KwHnCzDUth6BRr3vU3gqnuUgRvQ");
    expectedSignatures.push_back(expectedSignature);
    EXPECT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        // test data obtained from spl-token create-account
        "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVnH2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtCjAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzjB2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2TN3xXwu1";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignCreateTokenAccountForOther_3E6UFV) {
    const auto privateKeySigner =
        PrivateKey(parse_hex("4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    EXPECT_EQ(signer.string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto otherMainAddress = Address("3xJ3MoUVFPNFEHfWdtNFa8ajXUHsJPzXcBSWMKLd76ft");
    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto tokenAddress = Address("67BrwFYt7qUnbAcYBVx7sQ4jeD2KWN1ohP6bMikmmQV3");
    Solana::Hash recentBlockhash("HmWyvrif3QfZJnDiRyrojmH9iLr7eMxxqiC9RJWFeunr");

    auto message = Message::createTokenCreateAccount(signer, otherMainAddress, token, tokenAddress, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    auto expectedString =
        // https://explorer.solana.com/tx/3E6UFVamHCm6Bgk8gXdZex7R7tJAVxqJm6t9ephAKu1PjcfZrD7CJqMwKu6RrvWSUESbZFqzdUyLXuxAFaawPHvJ
        "4BsrHedHuForcKDhLdnLYDXgtQgQEj3EQVDtEhqa7o6ukFjW3shpTWv6PeKQdMp6af4ASjD4xQeZvXxLK5WUjguVMUf3xdJn7RnFeM7hdDJ56RDBM5PRJbRJVHjz6FJ7SVNTvr9y3gVYQtWx7NfKRxiyEAfq9JG7nqxSWaW6raMr9t35aVcdAVuXE9iXj3rzhVfCS69vVzy5KcFEK3mvDYG6L12V2CfviCydmeCvPw5r3zBUrZSQv7Ti4XFNBrPbk28gcqQwsBknBqasHxHqD9VUyPmBTuUyXq75QN8rhqN55NjxKBUw37tEUS1jKVpWnTeLFq1eRAMdXvjftNuQ5Bmm8Zc12PGWj9vdorBaYyvZXexJST5xNjR4SCkXvXZoRScETck95chv3VBn54jP8DpB4GGUmATFKSxpdtnNV64i1SQXW13KJwswthJvAaDiqevQLKLkvrTEAdb4BxEfPkFjDVti6P58rTZCMg5CTVLqdmWwpTSW5V";
    EXPECT_EQ(transaction.serialize(), expectedString);
}

TEST(SolanaSigner, SignTransferToken_3vZ67C) {
    const auto privateKeySigner =
        PrivateKey(Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    EXPECT_EQ(signer.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto token = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    auto senderTokenAddress = Address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    auto recipientTokenAddress = Address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    Solana::Hash recentBlockhash("CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi");

    auto message = Message::createTokenTransfer(signer, token,
        senderTokenAddress, recipientTokenAddress, amount, decimals, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature("3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg");
    expectedSignatures.push_back(expectedSignature);
    EXPECT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        // https://explorer.solana.com/tx/3vZ67CGoRYkuT76TtpP2VrtTPBfnvG2xj6mUTvvux46qbnpThgQDgm27nC3yQVUZrABFjT9Qo7vA74tCjtV5P9Xg
        // test data obtained from spl-token transfer
        "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jRe2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvPiPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMqDV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnotcrH7pUa94xCVvCPPaomF";
    EXPECT_EQ(transaction.serialize(), expectedString);
}
