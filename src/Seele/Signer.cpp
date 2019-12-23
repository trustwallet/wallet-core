// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../Data.h"
#include <boost/multiprecision/cpp_int.hpp>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

using namespace TW;
using namespace TW::Seele;

using json = nlohmann::json;
using uint128_t = boost::multiprecision::uint128_t;

Signer::Signer(Proto::SigningInput&& input) {
    this->input = input;
}

std::vector<uint8_t> Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = this->hash(input.sign_transaction());
    auto signature = key.sign(hash, TWCurveSECP256k1);

    return std::vector<uint8_t>(signature.begin(), signature.end());
}

std::string Signer::signaturePreimage() const {
    return signaturePreimageJSON(input).dump();
}

json Signer::buildTransactionJSON(const Data& signature) const {
    auto sig = Seele::Proto::Signature();
    sig.set_sig(signature.data(), signature.size());
    auto privateKey = PrivateKey(input.private_key());

    auto transaction = Seele::Proto::Transaction();
    auto hash = this->hash(input.sign_transaction());


    *transaction.mutable_data() = input.sign_transaction();
    *transaction.mutable_signature() = sig;
    transaction.set_hash("0x"+hex(hash));

    return transactionJSON(transaction);
}

std::string Signer::buildTransaction() const {
    auto signature = sign();
    return buildTransactionJSON(signature).dump();
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();

    auto signature = sign();
    auto txJson = buildTransactionJSON(signature);

    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());

    return output;
}

Data Signer::hash(const Proto::SignTransaction& transaction) const noexcept {
    auto encoded = Data();

    append(encoded, RLP::encodeLong(transaction.type()));
    append(encoded, RLP::encode(parse_hex(transaction.from())));
    append(encoded, RLP::encode(parse_hex(transaction.to())));
    append(encoded, RLP::encode(transaction.amount()));
    append(encoded, RLP::encode(transaction.account_nonce()));
    append(encoded, RLP::encodeLong(transaction.gas_price()));
    append(encoded, RLP::encodeLong(transaction.gas_limit()));
    append(encoded, RLP::encodeLong(transaction.timestamp()));
    append(encoded, RLP::encode(transaction.payload()));

    return Hash::keccak256(RLP::encodeList(encoded));
}
