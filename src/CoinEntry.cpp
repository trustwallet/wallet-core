// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CoinEntry.h"
#include "Coin.h"
#include "rust/Wrapper.h"
#include <variant>

namespace TW {

const char* getFromPrefixHrpOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Bech32Prefix>(prefix)) {
        const char* fromPrefix = std::get<Bech32Prefix>(prefix);
        if (fromPrefix != nullptr && *fromPrefix != 0) {
            return fromPrefix;
        }
    }
    // Prefix contains no hrp or empty, return coin-default
    return stringForHRP(TW::hrp(coin));
}

byte getFromPrefixPkhOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Base58Prefix>(prefix)) {
        return std::get<Base58Prefix>(prefix).p2pkh;
    }
    // Prefix contains no base58 prefixes, return coin-default
    return TW::p2pkhPrefix(coin);
}

void signRust(const Data& dataIn, TWCoinType coin, Data& dataOut)  {
    Rust::TWDataWrapper input = Rust::tw_data_create_with_bytes(dataIn.data(), dataIn.size());
    Rust::TWDataWrapper output = Rust::tw_any_signer_sign(input.ptr, static_cast<uint32_t>(coin));

    dataOut = output.toDataOrDefault();
}

Data preImageHashesRust(TWCoinType coin, const Data& dataIn) {
    Rust::TWDataWrapper input = dataIn;
    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_pre_image_hashes(static_cast<uint32_t>(coin), input.ptr);

    return output.toDataOrDefault();
}

void compileRust(
    TWCoinType coin,
    const Data& dataIn,
    const std::vector<Data>& signatures,
    const std::vector<PublicKey>& publicKeys,
    Data& dataOut
) {
    Rust::TWDataWrapper input = dataIn;

    std::vector<Data> publicKeysData;
    for (const auto& publicKey : publicKeys) {
        publicKeysData.push_back(publicKey.bytes);
    }

    Rust::TWDataVectorWrapper signaturesVec = signatures;
    Rust::TWDataVectorWrapper publicKeysVec = publicKeysData;

    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_compile(static_cast<uint32_t>(coin), input.ptr, signaturesVec.ptr, publicKeysVec.ptr);
    dataOut = output.toDataOrDefault();
}

Data buildSigningInputRust(
    TWCoinType coinType,
    const std::string& from,
    const std::string& to,
    const uint256_t& amount,
    const std::string& asset,
    const std::string& memo,
    const std::string& chainId
) {
    Rust::TWStringWrapper fromPtr = from;
    Rust::TWStringWrapper toPtr = to;
    Rust::TWStringWrapper amountPtr = toString(amount);
    Rust::TWStringWrapper assetPtr = asset;
    Rust::TWStringWrapper memoPtr = memo;
    Rust::TWStringWrapper chainIdPtr = chainId;

    Rust::TWDataWrapper input = Rust::tw_transaction_compiler_build_input(
        static_cast<uint32_t>(coinType),
        fromPtr.ptr,
        toPtr.ptr,
        amountPtr.ptr,
        assetPtr.ptr,
        memoPtr.ptr,
        chainIdPtr.ptr
    );
    return input.toDataOrDefault();
}

} // namespace TW
