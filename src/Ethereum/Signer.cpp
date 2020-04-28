// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Ethereum;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(load(input.chain_id()));
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Signer::build(input);

    signer.sign(key, transaction);

    auto output = Proto::SigningOutput();

    auto encoded = RLP::encode(transaction);
    output.set_encoded(encoded.data(), encoded.size());

    auto v = store(transaction.v);
    output.set_v(v.data(), v.size());

    auto r = store(transaction.r);
    output.set_r(r.data(), r.size());

    auto s = store(transaction.s);
    output.set_s(s.data(), s.size());

    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return hex(output.encoded());
}

std::tuple<uint256_t, uint256_t, uint256_t> Signer::values(const uint256_t &chainID,
                                                           const Data& signature) noexcept {
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

std::tuple<uint256_t, uint256_t, uint256_t>
Signer::sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data& hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
}

Transaction Signer::build(const Proto::SigningInput &input) {
    Data toAddress;
    if (!input.to_address().empty()) {
        toAddress.resize(20);
        auto address = Address(input.to_address());
        std::copy(address.bytes.begin(), address.bytes.end(), toAddress.data());
    }
    auto transaction = Transaction(
        /* nonce: */ load(input.nonce()),
        /* gasPrice: */ load(input.gas_price()),
        /* gasLimit: */ load(input.gas_limit()),
        /* to: */ toAddress,
        /* amount: */ load(input.amount()),
        /* payload: */ Data(input.payload().begin(), input.payload().end()));
    return transaction;
}

void Signer::sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept {
    auto hash = this->hash(transaction);
    auto tuple = Signer::sign(chainID, privateKey, hash);

    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::hash(const Transaction &transaction) const noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(transaction.nonce));
    append(encoded, RLP::encode(transaction.gasPrice));
    append(encoded, RLP::encode(transaction.gasLimit));
    append(encoded, RLP::encode(transaction.to));
    append(encoded, RLP::encode(transaction.amount));
    append(encoded, RLP::encode(transaction.payload));
    append(encoded, RLP::encode(chainID));
    append(encoded, RLP::encode(0));
    append(encoded, RLP::encode(0));
    return Hash::keccak256(RLP::encodeList(encoded));
}
