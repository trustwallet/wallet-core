// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "Solana/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Solana;

TEST(SolanaAddress, addressFromValidatorSeed) {
    auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto validator = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto expected = Address("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    ASSERT_EQ(addressFromValidatorSeed(user, validator, programId), expected);
}

TEST(SolanaAddress, FromPublicKey) {
    const auto addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto publicKey = PublicKey(Base58::bitcoin.decode(addressString), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    ASSERT_EQ(addressString, address.string());
}

TEST(SolanaAddress, FromString) {
    string addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto address = Address(addressString);
    ASSERT_EQ(address.string(), addressString);
}

TEST(SolanaAddress, isValid) {
    ASSERT_TRUE(Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST"));
    ASSERT_FALSE(Address::isValid(
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdSl")); // Contains invalid base-58 character
    ASSERT_FALSE(
        Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpd")); // Is invalid length
}

PublicKey createProgramAddress(std::vector<Data> seeds, const PublicKey& programId) {
    /*
    // Perform the calculation inline, calling this from within a program is
    // not supported
    #[cfg(not(target_arch = "bpf"))]
    {
    */

    // append seed in one
    Data hashInput;
    for (auto seed: seeds) {
        //if seed.len() > MAX_SEED_LEN {
        //  return Err(PubkeyError::MaxSeedLengthExceeded);
        //}
        append(hashInput, seed);
    }
    append(hashInput, programId.bytes);
    append(hashInput, TW::data("ProgramDerivedAddress"));
    std::cout << "hashInput size " << hashInput.size() << std::endl;
    //let mut hasher = crate::hash::Hasher::default();
    Hash::Hasher hasher = Hash::sha256d;
    auto hash = Hash::sha256d(hashInput.data(), hashInput.size());
    std::cout << "hash " << hash.size() << " " << hex(hash) << std::endl;
/*
            if curve25519_dalek::edwards::CompressedEdwardsY::from_slice(hash.as_ref())
                .decompress()
                .is_some()
            {
                return Err(PubkeyError::InvalidSeeds);
            }

            Ok(Pubkey::new(hash.as_ref()))
        }
        // Call via a system call to perform the calculation
        #[cfg(target_arch = "bpf")]
        {
            extern "C" {
                fn sol_create_program_address(
                    seeds_addr: *const u8,
                    seeds_len: u64,
                    program_id_addr: *const u8,
                    address_bytes_addr: *const u8,
                ) -> u64;
            };
            let mut bytes = [0; 32];
            let result = unsafe {
                sol_create_program_address(
                    seeds as *const _ as *const u8,
                    seeds.len() as u64,
                    program_id as *const _ as *const u8,
                    &mut bytes as *mut _ as *mut u8,
                )
            };
            match result {
                crate::entrypoint::SUCCESS => Ok(Pubkey::new(&bytes)),
                _ => Err(result.into()),
            }
        }
    }
*/
}

TEST(SolanaAddress, deriveDefaultTokenAddress) {
    Address mainAccount1("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    Address tokenProgramAddress("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
    PublicKey tokenProgramId = PublicKey(data(tokenProgramAddress.bytes.begin(), tokenProgramAddress.bytes.size()), TWPublicKeyTypeED25519);
    Address serumTokenMintAddress("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    EXPECT_EQ(mainAccount1.string(), "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");

    std::vector<Data> seeds = {parse_hex("0011"), parse_hex("2233")};
    //createProgramAddress(seeds, tokenProgramId);
}
