// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Wanchain;

void Signer::sign(const PrivateKey& privateKey, Ethereum::Transaction& transaction) const noexcept {
    auto hash = this->hash(transaction);
    auto signature = privateKey.sign(hash);
    auto tuple = values(signature);

    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::hash(const Ethereum::Transaction& transaction) const noexcept {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(1));
    append(encoded, Ethereum::RLP::encode(transaction.nonce));
    append(encoded, Ethereum::RLP::encode(transaction.gasPrice));
    append(encoded, Ethereum::RLP::encode(transaction.gasLimit));
    append(encoded, Ethereum::RLP::encode(transaction.to.bytes));
    append(encoded, Ethereum::RLP::encode(transaction.amount));
    append(encoded, Ethereum::RLP::encode(transaction.payload));
    append(encoded, Ethereum::RLP::encode(chainID));
    append(encoded, Ethereum::RLP::encode(0));
    append(encoded, Ethereum::RLP::encode(0));
    return Hash::keccak256(Ethereum::RLP::encodeList(encoded));
}

std::tuple<boost::multiprecision::uint256_t, boost::multiprecision::uint256_t, boost::multiprecision::uint256_t> Signer::values(const std::array<byte, 65>& signature) const noexcept {
    boost::multiprecision::uint256_t r, s, v;
    import_bits(r, signature.begin(), signature.begin() + 32);
    import_bits(s, signature.begin() + 32, signature.begin() + 64);
    import_bits(v, signature.begin() + 64, signature.begin() + 65);
    v += 27;

    boost::multiprecision::uint256_t newV;
    if (chainID != 0) {
        import_bits(newV, signature.begin() + 64, signature.begin() + 65);
        newV += 35 + chainID + chainID;
    } else {
        newV = v;
    }
    return std::make_tuple(r, s, newV);
}
