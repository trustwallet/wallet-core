// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include "SigData.h"

#include "../Hash.h"

#include <stdexcept>

using namespace TW;
using namespace TW::Ontology;

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
    auto signature = getPrivateKey().sign(Hash::sha256(tx.txHash()), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}

void Signer::addSign(Transaction& tx) const {
    if (tx.sigVec.size() >= Transaction::sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = getPrivateKey().sign(Hash::sha256(tx.txHash()), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}
