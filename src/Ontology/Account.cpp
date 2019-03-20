// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "Account.h"
#include "HexCoding.h"

#include <iostream>

using namespace TW;
using namespace TW::Ontology;

Account::Account(const std::string &priKey) {
    privateKey = parse_hex(priKey);
    auto pubKey = PrivateKey(privateKey).getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

Account::Account(const Data &priKey){
    privateKey = priKey;
    auto pubKey = PrivateKey(privateKey).getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

PrivateKey Account::getPrivateKey() const {
    return PrivateKey(privateKey);
}

PublicKey Account::getPublicKey() const {
    return PublicKey(publicKey);
}

Address Account::getAddress() const {
    return Address(address);
}

Data Account::sign(const Data &msg) noexcept {
    auto digest = Hash::sha256(msg);
    auto sk = getPrivateKey();
    auto signature = sk.sign(digest, TWCurveNIST256p1);
    signature.pop_back();
    return signature;
}


