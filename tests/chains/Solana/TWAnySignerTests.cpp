// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Solana/Address.h"
#include "Solana/Program.h"
#include "proto/Solana.pb.h"
#include "PrivateKey.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Solana::tests {

const auto expectedString1 =
    "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
    "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
    "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
    "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";

TEST(TWAnySignerSolana, SignTransfer) {
    auto privateKey = Base58::decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    ASSERT_EQ(output.encoded(), expectedString1);
    ASSERT_EQ(output.unsigned_tx(), "87PYsiS4MUU1UqXrsDoCBmD5FcKsXhwEBD8hc4zbq78yePu7bLENmbnmjmVbsj4VvaxnZhy4bERndPFzjSRH5WpwKwMLSCKvn9eSDmPESNcdkqne2UdMfWiFoq8ZeQBnF9h98dP8GM9kfzWPjvLmhjwuwA1E2k5WCtfii7LKQ34v6AtmFQGZqgdKiNqygP7ZKusHWGT8ZkTZ");
}

TEST(TWAnySignerSolana, SignV0Transfer) {
    // Successfully broadcasted: https://explorer.solana.com/tx/4ffBzXxLPYEEdCYpQGETkCTCCsH6iTdmKzwUZXZZgFemdhRpxQwboguFFoKCeGF3SsZPzuwwE7LbRwLgJbsyRqyP?cluster=testnet
    auto privateKey = parse_hex("833a053c59e78138a3ed090459bc6743cca6a9cbc2809a7bf5dbc7939b8775c8");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("6pEfiZjMycJY4VA2FtAbKgYvRwzXDpxY58Xp4b7FQCz9");
    message.set_value((uint64_t)5000L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("HxKwWFTHixCu8aw35J1uxAX6yUhLHkFCdJJdK4y98Gyj");
    input.set_v0_msg(true);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    ASSERT_EQ(output.encoded(), "6NijVxwQoDjqt6A41HXCK9kXwNDp48uLgvRyE8uz6NY5dEzaEDLzjzuMnc5TGatHZZUXehKrzUGzbg9jPSdn6pVsMc9TXNH6JGe5RJLmHwWey3MC1p8Hs2zhjw5P439P57NToatraDX9ZwvBtK4EzZzRjWbyGdicheTPjeYKCzvPCLxDkTFtPCM9VZGGXSN2Bne92NLDvf6ntNm5pxsPkZGxPe4w9Eq26gkE83hZyrYXKaiDh8TbqbHatSkw");
}

TEST(TWAnySignerSolana, SignTransferToSelf) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
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

TEST(TWAnySignerSolana, SignTransferWithMemoAndReference) {
    const auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd");
    message.set_value((uint64_t)10000000L);
    message.set_memo("HelloSolanaMemo");
    message.add_references("CuieVDEDtLo7FypA9SbLM9saXFdb1dsshEkyErMqkRQq");
    message.add_references("tFpP7tZUt6zb7YZPpQ11kXNmsc5YzpMXmahGMvCHhqS");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    const auto expectedString = "NfNH76sST3nJ4FmFGTZJBUpJou7DRuHM3YNprT1HeEau699CQF65xNf21Hoi491bbtVKUXfqCJyeZhfTCEnABuXNC1JrhGBeCv2AbQdaS9gpp9j4xHHomhCYdwYaBWFMcKkdMXrx9xHqL9Vkny4HezkwQfb3wGqcaE9XVRdkkNxsoJnVKddRnrQbjhsZGTcKdfmbTghoUeRECNPTm6nZTA1owWF1Dq6mfr6M3GZRh4ucqEquxKsQC2HQwNRrGZahsfyUvwspPWwMt78q5Jpjd9kHqkFDspZL6Pepv4dAA4uHhYDCHeP2bbDiFMBYxxWCVDDtRKSh3H92xUgh1GCSgNcjGdbVfQUhSDPX3k9xuuszPTsVZ2GnsavAsRp6Vf6fFEikBX6pVV9zjW1cx94EepQ2aGEBSsVu4RzX7rJjCLCq87h8cxxf1XnF8mvYGEK7wzF";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDelegateStakeTransaction_noStakeAccount) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_delegate_stake_transaction();
    message.set_validator_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message.set_value((uint64_t)42L);
    message.set_stake_account("");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "j24mVM9Zgu5vDZhPLGGuCRXQnP9djNtxdHh4txN3S7dwJsNNL5fbhzGpPgSUAcLGoMVCfF9TuqTYfpfJnb4sJFe1ahM8yPL5HwuKL6py5AZJFi8SWx9fvaVB699dCPo1GT3JoEBLPCZ9o2jQtnwzLkzTYJnKv2axqhKWFE2sz6TBA5J39eZcjMFUYgyxz6Q5S4MWqYQCb8UET2NAEZoKcfy7j8N25WXL6Gj4j3hBZjpHQQNaGaNEprEqyma3ZuVhpGiCALSsuzVLX3wZVo4icXwe952deMFA4tH3BK1jcSQCgfmcKDJ9nd7bdrnUUs4BoMdF1uDZB5LxE2UH8QiqtYvaUcorF4SJ3gPxM5ykbyPsNK1cSYZF9NMpW2GofyC17eELwnHQTQB2kqphxJZu7BahvkwiDPPeeydiXAkBspJ3nc3PCBujv6WJw22ZHw5j6zAP8ZGnCW44pqtWD5qifF9tTKhySKdANNiWifs3tSCCPQqjfJXu14drNinR6VG8rJxS1qgmRYiRQUa7m1vtoaZFRN5qKUeAfoFKkAVaNnMdwgsNqNH4dqBodTCJFs1LkYwhgRZdZGbwXTn1j7vpR3DSnv4g72i2H556srzK53jdUmdv6yfxt516XDSshqZtHnKZ1tudxKjBXwsqT3imDiZFVka9wKWUAYMCi4XZ79CY6Xpsd9c18U2e9TCngQmgkTATFgrqysfraokNffgqWxvsPMugksbvbPjJs3iCzByvphkC9p7hCf6LwbeF8XnVB91EAgRDA4VLE1f9wkcq5zjy879YWJ4r516h3PQszTz1EaJXNAXdbk5Em7eyuuabGP1Q3nijFTL2yhMDsXpgrjAuEAABNxFMd4J1JRMaic615mHrhwociksrsfQK";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDelegateStakeTransaction_withAccount) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_delegate_stake_transaction();
    message.set_validator_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message.set_stake_account("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "TKPiN35HzeD3zdwxDFvnkgoqud7CZsda15JkBwM4nDpr623rM7MZsH6QvMMyKpiz7MeRNTrfyHkRLQSBT9Tbg2mgTdfrbhhqeF3Suu5ECphqn8DFYPoMnFzeg5u9gaqevfjhuizzeo2YDJF8aVGy1pez8gMbp5vHz1SuvQUgfcvFctggUMwNiJorSmmp3N6TzQSd38CZrA8ZLhaJjuwDwVMjmj18rGTV1gkX19L7byTFrus2vNvPeUa2AawwUnFpYMPgvCKkHTrpnjvypjoLof9yMUFQ5M1S3Ntv53KJyXwXq6ejJnBDtisnDcdMDNSZp3VeKz6XCr8XVM5xNVh3LX12V4kc3ueqkokYJLP1JmuhA3nNZA1G5KTNno93HUoBkEa1x5h3haoCSgmQC97LoJbJM6B6C2NbaDj2J6iiTaVQdin4He4Jpj575WDhNTqsLjzFUHPUHQF1CRnuss8UpVyMsa4kdVqCDQGeh5DKbkikgcB8GKPBuC91DRxGEqgoygNsu5nnQy4o3YAJnBBK6HsKxpdjbYD8wCUdLw8muhjpEqeBTPShEaogm9zfehidiCcnxbeoX3gmW8oH9gpWoX7GrkJgF6Wn7iWohmrzqzAjoBz8hpeY5nkkhHrf9iswVGMpakdLGy3YxkGJVpsW8KJACwEKXGLq8SVLtXSUHG8EP16zfYHxKjkCSs8PkdFsA5esxsxppPTVZivuEPqJ5og55aNmugdNDrAFYWdcH1Q4rm7BXN6oHECdz2yY4HFVWh9u592oqozt2gQKu3vmhcNFzzQe1xgs6zKSv38kSGTnipd7Hx2VL3qNAR6XBRiwAi226qSTzxi6R82p7cMB7TMy6fk5AZ3sXDSXFNJ9S5SSU1V63ruw75QMtVio";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDeactivateStakeTransaction) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_deactivate_stake_transaction();
    message.set_stake_account("6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "6x3fSstNz4GpPxmT5jHXwyD62uyJMKaPWeBDNNcwXZA9NJ3E7KavCXPNUd8ZYTX5VpkfHKGszkwzM6AdAp4giLD29jvWdNYjkV1Nvb42xFwGD6ryMPZzXkJijaRTrA7SvPTDSRU2haGVmorqkywAXLQUCw47NmBUfLTb5gDcKoBeaAsahckv1eCE746thJVTg2dQNvUTULKF6xckUg7kwFkcUuRe4HCcRgrKcNAUKLR2rEM3brVQkUyAaAtMMtc3gVDXxxpbtW5Fa9wGaEnh31FdRo4z5YBzAUaz7vcrvzF2j81KCPTVnYyTmeJzCzJafzCVCtw";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDeactivateAllStakeTransaction) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_deactivate_all_stake_transaction();
    *message.add_stake_accounts() = "CJQStmfyoHbosX1GfVn64yWrNJAo214q2aqxwS6FGh4k";
    *message.add_stake_accounts() = "6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB";
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "U9azMJWRfDhypoDeQLYWyBYFZCwRNZy8sbrVX9awKK84zNGbSQfYTTJ3ZyzjNUVbU5npbw2MsWfmZGHZRvpfN7G7o3sVePyFRXrmLxrGZzGycFv25Zff4zPxDarbsugbCBgzVGpgwu8x7MdkwBAVHVtNsgMcHgArEAjEmk7YEGpZ15rjo39bCRvmuprWLqSv2SK1RyTZPpTPXVevAbA4i9vvcY8eUbwW29SZCoyGaagLU5EBV9vckMjzGa7gq2yMR6rbq8tDdWaXapYs8RavU49WN94yg4wdE4fzYq8DjqXHq3MuUBLxeYDKJnvj84ioeM4eR1EwjBNrGyz5GHTRuhbNg1nc57SpKsSMVSZW5Ra3tUk84YZXYFHxzeQ9Tv4o";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignWithdrawStakeTransaction) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_withdraw_transaction();
    message.set_stake_account("6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "gxr4o1trVP8DGG8UC21AA964YqAPFA3rBCF9MwmBQpn5fDtcujM9wp1gzT466MxWGR8wMciS6dSL771q29eURrEEuvhJzRaFDGPLgVB3UL4gd4T2amPQkR4Dzq5drKEtPJRBR86KVVc2kjDsbWNpdL8S7pZqW3VUijAbm9TS8ezG8NExSCkhxExKhUjXWWguEL4qXra7s2JZfhtmvuJneWnEY3isUVfC9knWtGNwpNFvRvzbH2sgHzwtSsD7mkYrBJoazLCwT8r9yypxycHL41XcGtH425MA16kVSunvvBfzG9PzBTS65YJBs64tzttasCU9uEphkwgmfrmoEC8iKt8xD47Ra79RyXd95yURsaxvpb1tVAH8kMNtj8iV1Pfm";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignWithdrawAllStakeTransaction) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_withdraw_all_transaction();
    message.add_stake_accounts();
    message.add_stake_accounts();
    message.mutable_stake_accounts(0)->set_stake_account("CJQStmfyoHbosX1GfVn64yWrNJAo214q2aqxwS6FGh4k");
    message.mutable_stake_accounts(0)->set_value((uint64_t)42L);
    message.mutable_stake_accounts(1)->set_stake_account("6XMLCn47d5kPi3g4YcjqFvDuxWnpVADpN2tXpeRc4XUB");
    message.mutable_stake_accounts(1)->set_value((uint64_t)67L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "cvBNusjtHkR74EfWsvFPEe2Mydcr7eoLeY2wJw2ZMZYViotbb63Adai7UD1PW9uLusoVHGLeJC5cPgVBC4F693P9tPAxLs9yiZj1ZJQ4DgnYbeXafqzjdWje1Ly5FgpDUJaaU2RnLCG51CcrmiTJ4KB5fwai6egZaNjbiqo1DEC1wJz4FgKug2aKQWLdeCiH9WhCuvqfhNV6mEE4qRCkU8uS2gfSqBd1AdrczvoDEbKQszosrwmawxqmvTE5EWaFzMb48x9nLqxvpQCvGQu1nX6FxZJjv2swekA7wGLEAA4uSdFLTHNrYSi8pn8hVYGwESEzth9oiPkJCvW7Y2KvGALeERUZn8knHiz2eqaaT72Ajp9UogMdZtiuFHufveLXpBLWUERchhB7eU1magYcPNHcZuEE4uQv5kZJhHAqYCGU6dyUFLVA9Edus7o6fTktYVCjoGb";
    EXPECT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignDeactivateStakeTransaction_1) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_deactivate_stake_transaction();
    message.set_stake_account("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");

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

TEST(TWAnySignerSolana, SignWithdrawStakeTransaction_1) {
    auto privateKey = Base58::decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto& message = *input.mutable_withdraw_transaction();
    message.set_stake_account("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "NL7WgagucfLd6AkTtcKe1dqd47xxzF356Q7tEhPrz1LRzZiAmokAaUkpwJ7X71Pmz97zZf9gZQU5BNswdcdpqUL8n1jwn4CoZMaPJhX5LF43Sj817cgreSG14TEWfKertpVpTtc5zY7vkDM7t9wjYhkaqgYz76HQtqAqRHnHF2Qr9EEfLj4zYRerWtyfS3EVyVUaasPxJ5vkcaonEfpGc6uWecaFr2A3YbzEBQpWXjMaXLqmMDtNS8rTNZmwvToa71ddFZKDgaHDcc6Lkg8qriZ3aQbUqL1TbeYp2mk9dWTKY62L1YFE2DyZV5P2qz5feywcMZ9JW6X1wBmiHFCseC42QbnbTibr1VdqLbGx7UWn5tHWk5jCN2aatEPfbFDZ";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateTokenAccount1) {
    auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
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

TEST(TWAnySignerSolana, SignCreateTokenAccount2_5KtPn1) {
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

TEST(TWAnySignerSolana, SignCreateTokenAccountForOther_3E6UFV) {
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

TEST(TWAnySignerSolana, SignTokenTransfer1_3vZ67C) {
    auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
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

TEST(TWAnySignerSolana, SignTokenTransfer2_2pMvzp) {
    auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
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

TEST(TWAnySignerSolana, SignCreateAndTransferToken_449VaY) {
    auto privateKeyData = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
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
        // https://explorer.solana.com/tx/449VaYo48LrkMJF6XVKt9sJwVQN6Seqrmh9erDCLtiuj6BgFG3wpF5TwjNkxgJ7qzNa6NTj3TFsU3h9hKszfkA7w
        "3Y2MVz2VVi7aEyC9q1awwdk1ModDBPHRSacKmTYnSgkmbbJeZ62Fub1bVPSHaTy4LUcQpzCQYhHAKtTKXUDYijEeLsMAUqPBEMAq1w8zCdqDpdXy6M4PuwNtYVV1WgqeiEsiMWpPp4BGWKfcziwFbmYueUGituacJq4wTnt92fho8mFi49XW64gEG4iNGScDtJkY7Geq8PKiLh1E9JMJoceiHxKbmxzCmmLTxEHdhySYHcDUSXnXWogZskeZNBMtR9dNjEMkCzEjrxRpBtJPtUNshciY45mDPNmw4j3xyLCBTRikyfFLc5g11r3UgyVD4YokoPRvrEXsgt6W3yjBshropBm6mY2eJYvfY2eZz4Yq8kLcUatCHVKtjcb1mP9Ww57KisJ9bRhipC8sodFaMYhZARMEa4a1u9eH4MyNUATRGNXarwQSBY46PWS3nKP6QBK7Dw7Ppp9MmYkdPcXKaLScbyLF3jKu6dHWMkHw3WdXSsM1wwXjXnWF9LxdwaEVcDmySWybj6aKD9QCWTU5kdncqJU56f7SYNRTN289WdUFGNDmSh56tj2v1";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateAndTransferTokenWithMemoReferences) {
    const auto privateKeyData = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    message.set_recipient_main_address("71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd");
    message.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    message.set_recipient_token_address("EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY");
    message.set_sender_token_address("ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf");
    message.set_amount(2900);
    message.set_decimals(6);
    message.set_memo("HelloSolanaMemo370");
    message.add_references("CuieVDEDtLo7FypA9SbLM9saXFdb1dsshEkyErMqkRQq");
    message.add_references("tFpP7tZUt6zb7YZPpQ11kXNmsc5YzpMXmahGMvCHhqS");
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString = "FuUw2MoEGPATE38roXAw9mGQhCfdsdpVDdhuf5h8LKc8iWj2HzNS3SteXqyUoZtQ7L1ufLvu7cTMwNzxT8snnVimcknsA52CeN7bgMz1Ad1hRTAr77zE5efzAi8B124kaQ1cBEb6nFMr5Zq4wwDRoJgBaiUaM1U9ZY6GofCKHGMQN7ZNqEFG4fFvPaMXB59dFtiqrtApBGzvDho3nGshyQWZVWfMY44hvVk45FqiGrXuqUwkiJqeRaDhooZdXiFR9ubwJLXo3Ux23ZyijWKXYNsx1Lm5zMFEgRz3kXhzxzb8uzHVSrFYNieXXCQEv1GtErMKeQWuAHcwS3zxC6avTnTWJhTz3kVSXfSTYEg4MF2MBWeGrzKZ7id88ZfbpG4ZwzsDsdUCSMV6YYRNmx9P3B6oC4DL7cbi2g8hwtBdeKojY4G6JMPeg629V9sPyg2KKeYxD3cjhMKAYtrsJEbixep4LZENtdQxmgZFouJVvGy9MVhiTzGEFVwm4G25p5FhWhiS9HxHWVRXpUFHi2K9K2ttoo4Ug39V9f8s9cG1Xb5A4bHhGSuKLeCCBcrBqPWEsuLdVhjxsKJrRBJhyrZ6mpxtDhUWivZa6skmEawTts9rN2aP3dXW3cNch3s3LTXZWXG9QPUARJJPy5QAYsBoR8GunF5FFgHVuEHVpjXAd8ku9f7aoF8RNiMnXAqQHxiM3ug6HZpLHLX8aGoUbJ7vVAnEDLH";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignJSON) {
    auto json = STRING(R"({"recentBlockhash":"11111111111111111111111111111111","transferTransaction":{"recipient":"EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd","value":"42"}})");
    Data keyData = Base58::decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    EXPECT_EQ(hex(keyData), "8778cc93c6596387e751d2dc693bbd93e434bd233bc5b68a826c56131821cb63");
    auto key = WRAPD(TWDataCreateWithBytes(keyData.data(), keyData.size()));

    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeSolana));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeSolana));
    assertStringsEqual(result, expectedString1);
}

TEST(TWAnySignerSolana, SignCreateNonceAccount) {
    const auto privateKeyData = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
    const auto privateKey = PrivateKey(privateKeyData);
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    const auto nonceAccountPrivateKeyData = parse_hex("2a9737aca3cde2dc0b4f3ae3487e3a90000490cb39fbc979da32b974ff5d7490");
    const auto nonceAccountPrivateKey = PrivateKey(nonceAccountPrivateKeyData);
    EXPECT_EQ(Address(PrivateKey(nonceAccountPrivateKey).getPublicKey(TWPublicKeyTypeED25519)).string(), "6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    uint64_t rent = 10000000;
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash(std::string("mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat"));
    message.set_nonce_account_private_key(nonceAccountPrivateKeyData.data(), nonceAccountPrivateKeyData.size());
    message.set_rent(rent);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "3wu6xJSbb2NysVgi7pdfMgwVBT1knAdeCr9NR8EktJLoByzM4s9SMto2PPmrnbRqPtHwnpAKxXkC4vqyWY2dRBgdGG"
        "CC1bep6qN5nSLVzpPYAWUSq5cd4gfYMAVriFYRRNHmYUnEq8vMn4vjiECmZoHrpabBj8HpXGqYBo87sbZa8ZPCxUcB"
        "71hxXiHWZHj2rovx2kr75Uuv1buWXyW6M8uR4UNvQcPPvzVbwBG82RjDYTuancMSAxmrVNR8GLBQNhrCCYrZyte3EW"
        "gEyMQxxfW8T3xNXqnbgdfvFJ3UjRBxXj3hrmv17xEivTjfs81aG2AAi24yiYrk8ep7eQqwDHVSArsrynnwVKVNUcCQ"
        "CnSy7fuiuS7FweFX8DEN1K9BrfecHyWrF15fYzhkmWSs64aH6ZTYHWPv5znhFKYmAuopGwbsBEb2j5p8NS3iJZ2skb"
        "2wi47n1rpLZfoCHWKxNiikkDUJTGQNcSDrGUMfeW5aGubJrCfecPKEo9Wo9kd36iSsxYPYSWNKrz2HTooa1rCRhqjX"
        "D8dyX3bXGV8TK6W2sEgf4JkcDnNoWQLbindcP8XR";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignWithdrawNonceAccount) {
    const auto privateKeyData = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
    const auto privateKey = PrivateKey(privateKeyData);
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");

    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_withdraw_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto recipient = std::string("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    uint64_t value = 10000000;
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash(std::string("5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8"));
    message.set_nonce_account(nonceAccount);
    message.set_recipient(recipient);
    message.set_value(value);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3"
        "S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJ"
        "XMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5"
        "jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqK"
        "NuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateTokenAccountAndTransfer) {
    const auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    const auto privateKey = PrivateKey(privateKeyData);
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    auto signer = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = std::string("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    auto recipientTokenAddress = std::string("BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash(std::string("AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D"));
    message.set_recipient_main_address(recipientMainAddress);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7v"
        "SHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W"
        "2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXs"
        "GAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZM"
        "uGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4d"
        "cH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTa"
        "AKX32wo5xoMj9wJmmtcU6YbXN4KgZ";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignAdvanceNonceAccount) {
    const auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    const auto privateKey = PrivateKey(privateKeyData);
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_advance_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash(std::string("4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck"));
    message.set_nonce_account(nonceAccount);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKw"
        "tKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9a"
        "cwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73"
        "FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignTokenTransferWithExternalFeePayer) {
    const auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    ASSERT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    const auto feePayerPrivateKeyData = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
    ASSERT_EQ(Address(PrivateKey(feePayerPrivateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = Solana::Proto::SigningInput();
    auto& message = *input.mutable_token_transfer_transaction();
    message.set_token_mint_address("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    message.set_sender_token_address("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    message.set_recipient_token_address("AZapcpAZtEL1gQuC87F2L1hSfAZnAvNy1hHtJ8DJzySN");
    message.set_amount(4000); // 0.004
    message.set_decimals(6);
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash("H4gZ56AdmHfZj1F36oWrxDJMUJ8ph7XdTHtmsbtHZshG");
    input.set_fee_payer("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    input.set_fee_payer_private_key(feePayerPrivateKeyData.data(), feePayerPrivateKeyData.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    auto expectedString =
        // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
        "ushDP6dNZWq32FASGqdnw7E8x14zFDAEZBViTyevUptV9yb4WSgpYzEGCxt9sXkrEtHVyww4F4GdcGZbPEaQTjAeyX5KGvHHDhWoPeFNnECzjUTuPj35dpF7zJ75Jx3ADfLQtUyzu5w7812fQvhwBwP8XDm3btqSG4VLWeQU5XuVqpg33Mq1L9zkGHQ8PZ4WkgNuSrC584EVnDcFE4rZsUtAv2jFTMjinQJB1qQEGTCHbjgdtJt8PzmXGXeczNyisPsEDrhZUw3g7RFYsgBDB1RFe1TxspzbWmxwr6CNPkGVsopmS6cbvSG9ejXY8xRYaswP7knAoPXwYk26yetoA824mzdv9vJ2RYpyK72EyCqFfFidm8MrJjFR49KwV3HRQKxZzYcvhYuJhR15GKBUWAQvYJTQQWArTi7pr7m84wNsV1mCUjrYsCVK47QtMAYvWU4toTxfgThngfF47awnpcSxfy8ggbwamq7qcaSH6cQVk1LPGo1iB5YxitSbaXP";
    ASSERT_EQ(output.encoded(), expectedString);
}

TEST(TWAnySignerSolana, SignCreateTokenAccountAndTransferWithExternalFeePayer) {
    const auto privateKeyData = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    const auto privateKey = PrivateKey(privateKeyData);
    EXPECT_EQ(Address(PrivateKey(privateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    const auto feePayerPrivateKeyData = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
    EXPECT_EQ(Address(PrivateKey(feePayerPrivateKeyData).getPublicKey(TWPublicKeyTypeED25519)).string(), "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");

    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    auto signer = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = std::string("E54BymfdhXQtzDSGtgiJayauEMdB2gJjPqoDjzfbCXwj");
    auto recipientTokenAddress = std::string("Ay7G7Yb7ZCebCQdG39FxD1nFNDtqFWJCBgfd5Ek7DDcS");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_private_key(privateKeyData.data(), privateKeyData.size());
    input.set_recent_blockhash(std::string("EsnN3ksLV6RLBW7qqgrvm2diwVJNTzL9QCuzm6tqWsU8"));
    input.set_fee_payer("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    input.set_fee_payer_private_key(feePayerPrivateKeyData.data(), feePayerPrivateKeyData.size());
    message.set_recipient_main_address(recipientMainAddress);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    // https://explorer.solana.com/tx/7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2?cluster=devnet
    auto expectedString =
        "5sxFkQYd2FvqRU64N79A6xjJKNkgUsEEg2wKgai2NiK7A7hF3q5GYEbjQsYBG9S2MejwTENbwHzvypaa3D3cEkxvVTg19aJFWdCtXQiz42QF5fN2MuAb6eJR4KHFnzCtxxnYGtN9swZ5B5cMSPCffCRZeUTe3kooRmbTYPvSaemU6reVSM7X2beoFKPd2svrLFa8XnvhBwL9EiFWQ9WhHB2cDV7KozCnJAW9kdNDR4RbfFQxboANGo3ZGE5ddcZ6YdomATKze1TtHj2qzJEJRwxsRr3iM3iNFb4Eav5Q2n71KUriRf73mo44GQUPbQ2LvpZKf4V6M2PzxJwzBo7FiFZurPmsanT3U5efEsKnnueddbiLHedc8JXc1d3Z53sFxVGJpsGA8RR6thse9wUvaEWqXVtPbNA6NMao9DFGD6Dudza9pJXSobPc7mDHZmVmookf5vi6Lb9Y1Q4EgcEPQmbaDnKGGB6uGfZe629i3iKXRzAd2dB7mKfffhDadZ8S1eYGT3dhddV3ExRxcqDP9BAGQT3rkRw1JpeSSi7ziYMQ3vn4t3okdgQSq6rrpbPDUNG8tLSHFMAq3ydnh4Cb4ECKkYoz9SFAnXACUu4mWETxijuKMK9kHrTqPGk9weHTzobzCC8q8fcPWV3TcyUyMxsbVxh5q1p5h5tWfD9td5TZJ2HEUbTop2dA53ZF";
    ASSERT_EQ(output.encoded(), expectedString);
}
} // namespace TW::Solana::tests
