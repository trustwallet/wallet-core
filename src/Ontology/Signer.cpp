// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <stdexcept>

#include "../Hash.h"

#include "Signer.h"
#include "SigData.h"
#include "HexCoding.h"

using namespace TW;
using namespace TW::Ontology;

Signer::Signer(const std::string &priKey) {
    privateKey = parse_hex(priKey);
    auto pubKey = PrivateKey(privateKey).getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

Signer::Signer(const Data &priKey) {
    privateKey = priKey;
    auto pubKey = PrivateKey(privateKey).getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

PrivateKey Signer::getPrivateKey() const {
    return PrivateKey(privateKey);
}

PublicKey Signer::getPublicKey() const {
    return PublicKey(publicKey);
}

Address Signer::getAddress() const {
    return Address(address);
}

void Signer::sign(Transaction &tx) const {
    if (tx.sigVec.size() >= Transaction::sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = getPrivateKey().sign(Hash::sha256(tx.txHash()), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}

void Signer::addSign(Transaction &tx) const {
    if (tx.sigVec.size() >= Transaction::sigVecLimit) {
        throw std::runtime_error("the number of transaction signatures should not be over 16.");
    }
    auto signature = getPrivateKey().sign(Hash::sha256(tx.txHash()), TWCurveNIST256p1);
    signature.pop_back();
    tx.sigVec.emplace_back(publicKey, signature, 1);
}



