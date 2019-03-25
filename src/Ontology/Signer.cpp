// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Hash.h"

#include "Signer.h"
#include "HexCoding.h"

using namespace TW;
using namespace TW::Ontology;

Signer::Signer(const std::string &priKey) {
    privateKey = parse_hex(priKey);
    auto pubKey = PrivateKey(privateKey).getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

Signer::Signer(const Data &priKey){
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

Data Signer::sign(const Data &msg) noexcept {
    auto digest = Hash::sha256(msg);
    auto sk = getPrivateKey();
    auto signature = sk.sign(digest, TWCurveNIST256p1);
    signature.pop_back();
    return signature;
}


