// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"

#include "HexCoding.h"
#include "SigData.h"
#include "../Ontology/Oep4TxBuilder.h"
#include "../Ontology/OngTxBuilder.h"
#include "../Ontology/OntTxBuilder.h"

#include <stdexcept>

namespace TW::Ontology {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto contract = std::string(input.contract().begin(), input.contract().end());
    auto output = Proto::SigningOutput();
    try {
        if (contract == "ONT") {
            auto encoded = OntTxBuilder::build(input);
            output.set_encoded(encoded.data(), encoded.size());
        } else if (contract == "ONG") {
            auto encoded = OngTxBuilder::build(input);
            output.set_encoded(encoded.data(), encoded.size());
        } else {
            // then assume it's oep4 address
            auto encoded = Oep4TxBuilder::build(input);
            output.set_encoded(encoded.data(), encoded.size());
        }
    } catch (...) {
    }
    return output;
}

Signer::Signer(TW::PrivateKey priKey) : privKey(std::move(priKey)) {
    auto pubKey = privKey.getPublicKey(TWPublicKeyTypeNIST256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

PrivateKey Signer::getPrivateKey() const {
    return privKey;
}

PublicKey Signer::getPublicKey() const {
    return PublicKey(publicKey, TWPublicKeyTypeNIST256p1);
}

Address Signer::getAddress() const {
    return Address(address);
}

void Signer::sign(Transaction& tx) const {
    if (tx.sigVec.size() >= Transaction::sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = getPrivateKey().sign(tx.txHash(), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}

void Signer::addSign(Transaction& tx) const {
    if (tx.sigVec.size() >= Transaction::sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = getPrivateKey().sign(tx.txHash(), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}

Data Signer::encodeTransaction(const Proto::SigningInput& input, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys) {
    assert(signatures.size() > 0 && signatures.size() == publicKeys.size());

    auto contract = std::string(input.contract().begin(), input.contract().end());
    auto tx = Transaction();

    if (contract == "ONT") {
            tx = OntTxBuilder::buildTransferTx(input);
    } else if (contract == "ONG") {
            tx = OngTxBuilder::buildTransferTx(input);
    } else {
            tx = Oep4TxBuilder::buildTx(input);
    }

    for (auto i = 0u; i < signatures.size(); ++i) {
        tx.sigVec.emplace_back(publicKeys[i].bytes, signatures[i], 1);
    }

    return tx.serialize();
}

} // namespace TW::Ontology
