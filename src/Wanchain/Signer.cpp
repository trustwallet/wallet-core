// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/RLP.h"
#include "../Ethereum/Signer.h"

using namespace TW;
using namespace TW::Wanchain;

Data WrappedTransaction::hash(const uint256_t chainID) const {
    Data encoded;
    append(encoded, Ethereum::RLP::encode(1));
    append(encoded, Ethereum::RLP::encode(wrapped->nonce));
    append(encoded, Ethereum::RLP::encode(wrapped->gasPrice));
    append(encoded, Ethereum::RLP::encode(wrapped->gasLimit));
    append(encoded, Ethereum::RLP::encode(wrapped->to));
    append(encoded, Ethereum::RLP::encode(wrapped->amount));
    append(encoded, Ethereum::RLP::encode(wrapped->payload));
    append(encoded, Ethereum::RLP::encode(chainID));
    append(encoded, Ethereum::RLP::encode(0));
    append(encoded, Ethereum::RLP::encode(0));
    return Hash::keccak256(Ethereum::RLP::encodeList(encoded));
}

Data WrappedTransaction::encoded(const Ethereum::Signature& signature) const {
    Data encoded;
    append(encoded, Ethereum::RLP::encode(1));
    append(encoded, Ethereum::RLP::encode(wrapped->nonce));
    append(encoded, Ethereum::RLP::encode(wrapped->gasPrice));
    append(encoded, Ethereum::RLP::encode(wrapped->gasLimit));
    append(encoded, Ethereum::RLP::encode(wrapped->to));
    append(encoded, Ethereum::RLP::encode(wrapped->amount));
    append(encoded, Ethereum::RLP::encode(wrapped->payload));
    append(encoded, Ethereum::RLP::encode(signature.v));
    append(encoded, Ethereum::RLP::encode(signature.r));
    append(encoded, Ethereum::RLP::encode(signature.s));
    return Ethereum::RLP::encodeList(encoded);
}

Ethereum::Proto::SigningOutput Signer::sign(const Ethereum::Proto::SigningInput& input) noexcept {
    auto signer = Signer(load(input.chain_id()));
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = WrappedTransaction(Ethereum::Signer::buildNonTyped(input));

    auto signature = signer.sign(key, transaction);

    auto output = Ethereum::Proto::SigningOutput();

    auto encoded = transaction.encoded(signature);
    output.set_encoded(encoded.data(), encoded.size());

    auto v = store(signature.v);
    output.set_v(v.data(), v.size());

    auto r = store(signature.r);
    output.set_r(r.data(), r.size());

    auto s = store(signature.s);
    output.set_s(s.data(), s.size());

    return output;
}

Ethereum::Signature Signer::sign(const PrivateKey& privateKey, const WrappedTransaction& transaction) const noexcept {
    auto hash = transaction.hash(chainID);
    return Ethereum::Signer::sign(privateKey, chainID, hash);
}
