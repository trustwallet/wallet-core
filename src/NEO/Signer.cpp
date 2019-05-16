// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../HexCoding.h"
#include "../Hash.h"

using namespace TW;
using namespace TW::NEO;

Signer::Signer(TW::PrivateKey priKey) : privateKey(std::move(priKey)) {
    auto pubKey = privateKey.getPublicKey(PublicKeyType::nist256p1);
    publicKey = pubKey.bytes;
    address = Address(pubKey).string();
}

PrivateKey Signer::getPrivateKey() const {
    return privateKey;
}

PublicKey Signer::getPublicKey() const {
    return PublicKey(publicKey);
}

Address Signer::getAddress() const {
    return Address(address);
}

Data Signer::sign(Transaction &tx) const {
    return sign(tx.getHash());
}

Data Signer::sign(const Data &data) const {
    auto signature = getPrivateKey().sign(Hash::sha256(data), TWCurveNIST256p1);
    signature.pop_back();
    return signature;
}
