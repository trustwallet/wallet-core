// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/RLP.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Harmony;

std::tuple<uint256_t, uint256_t, uint256_t> Signer::values(const uint256_t &chainID,
                                                           const Data &signature) noexcept {
    auto r = load(Data(signature.begin(), signature.begin() + 32));
    auto s = load(Data(signature.begin() + 32, signature.begin() + 64));
    auto v = load(Data(signature.begin() + 64, signature.begin() + 65));
    v += 35 + chainID + chainID;
    return std::make_tuple(r, s, v);
}

std::tuple<uint256_t, uint256_t, uint256_t>
Signer::sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
}

Proto::SigningOutput Signer::sign(const TW::Harmony::Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Address toAddr;
    if (!Address::decode(input.to_address(), toAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto transaction = Transaction(
        /* nonce: */ load(input.nonce()),
        /* gasPrice: */ load(input.gas_price()),
        /* gasLimit: */ load(input.gas_limit()),
        /* fromShardID */ load(input.from_shard_id()),
        /* toShardID */ load(input.to_shard_id()),
        /* to: */ toAddr,
        /* amount: */ load(input.amount()),
        /* payload: */ Data(input.payload().begin(), input.payload().end()));

    auto signer = Signer(uint256_t(load(input.chain_id())));
    signer.sign(key, transaction);
    auto protoOutput = Proto::SigningOutput();
    auto encoded = signer.rlpNoHash(transaction, true);
    protoOutput.set_encoded(encoded.data(), encoded.size());
    auto v = store(transaction.v);
    protoOutput.set_v(v.data(), v.size());
    auto r = store(transaction.r);
    protoOutput.set_r(r.data(), r.size());
    auto s = store(transaction.s);
    protoOutput.set_s(s.data(), s.size());
    return protoOutput;
}

void Signer::sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept {
    auto hash = this->hash(transaction);
    auto tuple = Signer::sign(chainID, privateKey, hash);
    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::rlpNoHash(const Transaction &transaction, const bool include_vrs) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.nonce));
    append(encoded, RLP::encode(transaction.gasPrice));
    append(encoded, RLP::encode(transaction.gasLimit));
    append(encoded, RLP::encode(transaction.fromShardID));
    append(encoded, RLP::encode(transaction.toShardID));
    append(encoded, RLP::encode(transaction.to.getKeyHash()));
    append(encoded, RLP::encode(transaction.amount));
    append(encoded, RLP::encode(transaction.payload));
    if (include_vrs) {
        append(encoded, RLP::encode(transaction.v));
        append(encoded, RLP::encode(transaction.r));
        append(encoded, RLP::encode(transaction.s));
    } else {
        append(encoded, RLP::encode(chainID));
        append(encoded, RLP::encode(0));
        append(encoded, RLP::encode(0));
    }
    return RLP::encodeList(encoded);
}

std::string Signer::txnAsRLPHex(Transaction &transaction) const noexcept {
    return TW::hex(rlpNoHash(transaction, false));
}

Data Signer::hash(const Transaction &transaction) const noexcept {
    return Hash::keccak256(rlpNoHash(transaction, false));
}
