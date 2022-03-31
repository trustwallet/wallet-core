// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include "SigData.h"
#include "../Ontology/OngTxBuilder.h"
#include "../Ontology/OntTxBuilder.h"


#include "../Hash.h"

#include <stdexcept>

using namespace TW;
using namespace TW::Ontology;

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
        }
    } catch (...) {
    }
    return output;
}

Proto::TransactionInput Signer::signInput2TxInput(const Proto::SigningInput& input) noexcept {
    auto txInput = Proto::TransactionInput();
    txInput.set_contract(input.contract());
    txInput.set_method(input.method());
    txInput.set_owner_address(input.owner_address());
    txInput.set_to_address(input.to_address());
    txInput.set_amount(input.amount());
    txInput.set_gas_price(input.gas_price());
    txInput.set_gas_limit(input.gas_limit());
    txInput.set_nonce(input.nonce());

    return txInput;
}

Signer::Signer(TW::PrivateKey priKey) : privateKey(std::move(priKey)) {
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeNIST256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

PrivateKey Signer::getPrivateKey() const {
    return privateKey;
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

Data Signer::encodeTransaction(const Proto::SigningInput& input, const Data& signature, const PublicKey& publicKey) {
    auto txInput = Signer::signInput2TxInput(input);
    auto contract = std::string(input.contract().begin(), input.contract().end());
    auto tx = Transaction();

    if (contract == "ONT") {
            tx = OntTxBuilder::buildTransferTx(txInput);
    } else if (contract == "ONG") {
            tx = OngTxBuilder::buildTransferTx(txInput);
    } else {
        throw std::invalid_argument("invalid contract");
    }

    tx.sigVec.emplace_back(publicKey.bytes, signature, 1);

    return tx.serialize();
}