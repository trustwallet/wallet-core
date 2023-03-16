// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "PrivateKey.h"
#include "Solana/Address.h"
#include "Solana/Program.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Solana::tests {

TEST(SolanaStakeProgram, addressFromValidatorSeed) {
    auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto validator = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto expected = Address("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    EXPECT_EQ(StakeProgram::addressFromValidatorSeed(user, validator, programId), expected);
}

TEST(SolanaStakeProgram, addressFromRecentBlockhash) {
    {
        auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
        Data recentBlockhash = Base58::decode("11111111111111111111111111111111");
        auto programId = Address("Stake11111111111111111111111111111111111111");
        auto expected = Address("GQDDc5EVGJZFC7AvpEJ8eoCQ75Yy4gr7eu17frCjvQRQ");
        EXPECT_EQ(StakeProgram::addressFromRecentBlockhash(user, recentBlockhash, programId), expected);
    }
    {
        auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
        Data recentBlockhash = Base58::decode("9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K");
        auto programId = Address("Stake11111111111111111111111111111111111111");
        auto expected = Address("2Kos1xJRBq3Ae1GnVNBx7HgJhq8KvdUe2bXE4QGdNaXb");
        EXPECT_EQ(StakeProgram::addressFromRecentBlockhash(user, recentBlockhash, programId), expected);
    }
}

TEST(SolanaTokenProgram, defaultTokenAddress) {
    const Address serumToken = Address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(TokenProgram::defaultTokenAddress(Address("HBYC51YrGFAZ8rM7Sj8e9uqKggpSrDYrinQDZzvMtqQp"), serumToken).string(),
              "6X4X1Ae24mkoWeCEpktevySVG9jzeCufut5vtUW3wFrD");
}

TEST(SolanaTokenProgram, findProgramAddress) {
    std::vector<Data> seeds = {
        Base58::decode("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"),
        Base58::decode("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"),
        Base58::decode("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"),
    };
    {
        Address address = TokenProgram::findProgramAddress(seeds, Address("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"));
        EXPECT_EQ(address.string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    }
    {
        Address address = TokenProgram::findProgramAddress(seeds, Address("MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"));
        EXPECT_EQ(address.string(), "CuS1kE1wvGTmwGk3FYNQK85g4jU7gMySWwFRQQ9LFunp");
    }
}

TEST(SolanaTokenProgram, createProgramAddress) {
    std::vector<Data> seeds4 = {
        Base58::decode("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"),
        Base58::decode("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"),
        Base58::decode("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"),
        Data{255}};
    {
        Address address = TokenProgram::createProgramAddress(seeds4, Address("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"));
        EXPECT_EQ(address.string(), "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    }
    {
        Address address = TokenProgram::createProgramAddress(seeds4, Address("MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"));
        EXPECT_EQ(address.string(), "CuS1kE1wvGTmwGk3FYNQK85g4jU7gMySWwFRQQ9LFunp");
    }
    // https://github.com/solana-labs/solana/blob/f25c969ad87e64e6d1fd07d2d37096ac71cf8d06/sdk/program/src/pubkey.rs#L353-L435
    {
        std::vector<Data> seeds = {TW::data(""), {1}};
        Address address = TokenProgram::createProgramAddress(seeds, Address("BPFLoader1111111111111111111111111111111111"));
        EXPECT_EQ(address.string(), "3gF2KMe9KiC6FNVBmfg9i267aMPvK37FewCip4eGBFcT");
    }
    {
        std::vector<Data> seeds = {TW::data("Talking"), TW::data("Squirrels")};
        Address address = TokenProgram::createProgramAddress(seeds, Address("BPFLoader1111111111111111111111111111111111"));
        EXPECT_EQ(address.string(), "HwRVBufQ4haG5XSgpspwKtNd3PC9GM9m1196uJW36vds");
    }
    {
        std::vector<Data> seeds = {Base58::decode("SeedPubey1111111111111111111111111111111111")};
        Address address = TokenProgram::createProgramAddress(seeds, Address("BPFLoader1111111111111111111111111111111111"));
        EXPECT_EQ(address.string(), "GUs5qLUfsEHkcMB9T38vjr18ypEhRuNWiePW2LoK4E3K");
    }
}

} // namespace TW::Solana::tests
