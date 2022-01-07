// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Ethereum;

// May throw
Data addressStringToData(const std::string& asString) {
    if (asString.empty()) {
        return {};
    }
    auto address = Address(asString);
    Data asData;
    asData.resize(20);
    std::copy(address.bytes.begin(), address.bytes.end(), asData.data());
    return asData;
}

Signature Signer::values(const uint256_t &chainID, const Data &signature, bool includeEip155) noexcept {
    boost::multiprecision::uint256_t r, s, v;
    import_bits(r, signature.begin(), signature.begin() + 32);
    import_bits(s, signature.begin() + 32, signature.begin() + 64);
    import_bits(v, signature.begin() + 64, signature.begin() + 65);
    auto rsv = Signature{r, s, v};

    // Embed chainID in V param, for replay protection, legacy (EIP155)
    if (includeEip155) {
        if (chainID != 0) {
            rsv.v += 35 + chainID + chainID;
        } else {
            rsv.v += 27;
        }
    }

    return rsv;
}


Signature Signer::sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash, bool includeEip155) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return Signer::values(chainID, signature, includeEip155);
}


std::shared_ptr<TransactionBase> Signer::build(const Proto::SigningInput &input) {
    Data toAddress = addressStringToData(input.to_address());
    uint256_t nonce = load(input.nonce());
    uint256_t gasPrice = load(input.gas_price());
    uint256_t gasLimit = load(input.gas_limit());
    uint256_t maxInclusionFeePerGas = load(input.max_inclusion_fee_per_gas());
    uint256_t maxFeePerGas = load(input.max_fee_per_gas());

    switch (input.tx_mode()) {
    case Proto::TransactionMode::Legacy:
    default:
        return TransactionNonTyped::buildNativeTransfer(
            nonce, gasPrice, gasLimit,
            /* to: */ toAddress,
            /* amount: */ load(input.amount()),
            /* optional data: */Data(input.payload().begin(),
                                      input.payload().end()));

    case Proto::TransactionMode::Enveloped: // Eip1559
        return TransactionEip1559::buildNativeTransfer(
            nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
            /* to: */ toAddress,
            /* amount: */ load(input.amount()),
            /* optional data: */
            Data(input.payload().begin(),
                 input.payload().end()));
    }

}

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) const noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Signer::build(input);

    auto signature = sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();

    auto encoded = transaction->encoded(signature, chainID);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto v = store(signature.v);
    protoOutput.set_v(v.data(), v.size());

    auto r = store(signature.r);
    protoOutput.set_r(r.data(), r.size());

    auto s = store(signature.s);
    protoOutput.set_s(s.data(), s.size());

    return protoOutput;
}

Signature Signer::sign(const PrivateKey &privateKey, const std::shared_ptr<TransactionBase> transaction) const noexcept {
    auto txHash = this->hash(transaction);
    return Signer::sign(this->chainID, privateKey, txHash, transaction->usesReplayProtection());
}

Data Signer::serialize(const std::shared_ptr<TransactionBase> transaction) const noexcept {
    return transaction->serialize(this->chainID);
}

Data Signer::hash(const std::shared_ptr<TransactionBase> transaction) const noexcept {
    return transaction->preHash(this->chainID);
}
