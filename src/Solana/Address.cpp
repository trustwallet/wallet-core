// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Transaction.h"
#include "../Base58.h"
#include "../Base58Address.h"
#include "../Hash.h"

#include "TrezorCrypto/ed25519-donna.h"

#include <cassert>

using namespace TW;
using namespace TW::Solana;

bool Address::isValid(const std::string& string) {
    const auto data = Base58::bitcoin.decode(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    const auto data = Base58::bitcoin.decode(string);
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    static_assert(PublicKey::ed25519Size == size);
    std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.data());
}

std::string Address::string() const {
    return Base58::bitcoin.encode(bytes);
}

Data Address::vector() const {
    Data vec(std::begin(bytes), std::end(bytes));
    return vec;
}

Address addressFromValidatorSeed(const Address& fromAddress, const Address& validatorAddress,
                                 const Address& programId) {
    Data extended = fromAddress.vector();
    std::string seed = validatorAddress.string();
    Data vecSeed(seed.begin(), seed.end());
    vecSeed.resize(32);
    Data additional = programId.vector();
    extended.insert(extended.end(), vecSeed.begin(), vecSeed.end());
    extended.insert(extended.end(), additional.begin(), additional.end());
    Data hash = TW::Hash::sha256(extended);
    return Address(PublicKey(hash, TWPublicKeyTypeED25519));
}

/*
 * Check if given address is on ed25519 curve,
 * Based on solana code, create_program_address()
 * if curve25519_dalek::edwards::CompressedEdwardsY::from_slice(hash.as_ref())
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L135
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L153
 */
bool Address::isValidOnCurve(const Data& data) {
    if (data.size() != 32) {
        return false;
    }
    assert(data.size() == 32);
    ge25519 r;
    int res = ge25519_unpack_negative_vartime(&r, data.data());
    return res == 0;
}

/*
 * Based on solana-program-library code, get_associated_token_address()
 * https://github.com/solana-labs/solana-program-library/blob/master/associated-token-account/program/src/lib.rs#L35
 * https://github.com/solana-labs/solana-program-library/blob/master/associated-token-account/program/src/lib.rs#L19
 */
std::string TokenProgram::defaultTokenAddress(const std::string& mainAddress, const std::string& tokenMintAddress) {
    Address main = Address(mainAddress);
    Address tokneMint = Address(tokenMintAddress);
    Address programId = Address(TOKEN_PROGRAM_ID_ADDRESS);
    std::vector<Data> seeds = {
        TW::data(main.bytes.data(), main.bytes.size()),
        TW::data(programId.bytes.data(), programId.bytes.size()),
        TW::data(tokneMint.bytes.data(), tokneMint.bytes.size())
    };
    return findProgramAddress(seeds, Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS));
}

/*
 * Based on solana code, find_program_address()
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L193
 */
std::string TokenProgram::findProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId) {
    std::string result;
    // cycle through seeds for the rare case when result is not valid
    for (uint8_t seed = 255; seed >= 0; --seed) {
        std::vector<Data> seedsCopy;
        for (auto& s: seeds) {
            seedsCopy.push_back(s);
        }
        // add extra seed
        seedsCopy.push_back({seed});
        Address address = createProgramAddress(seedsCopy, Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS));
        if (Address::isValidOnCurve(TW::data(address.bytes.data(), address.bytes.size()))) {
            result = address.string();
            break;
        }
        // try next seed
    }
    return result;
}

/*
 * Based on solana code, create_program_address()
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L135
 */
Address TokenProgram::createProgramAddress(const std::vector<TW::Data>& seeds, const Address& programId) {
    // concatenate seeds
    Data hashInput;
    for (auto& seed: seeds) {
        append(hashInput, seed);
    }
    // append programId
    append(hashInput, TW::data(programId.bytes.data(), programId.bytes.size()));
    append(hashInput, TW::data("ProgramDerivedAddress"));
    // compute hash
    Data hash = TW::Hash::sha256(hashInput.data(), hashInput.size());
    return Address(PublicKey(hash, TWPublicKeyTypeED25519));
}
