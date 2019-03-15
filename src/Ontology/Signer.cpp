// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "Signer.h"
#include "HexCoding.h"

#include <iostream>

using namespace TW;
using namespace TW::Ontology;

PrivateKey Signer::getPrivateKey(const std::string &hexPrvKey) noexcept {
    return PrivateKey(parse_hex(hexPrvKey));
}

PublicKey Signer::getPublicKey(const PrivateKey &privateKey) noexcept {
    return privateKey.getPublicKey(PublicKeyType::nist256p1);
}

PublicKey Signer::getPublicKey(const std::string &hexPrvKey) noexcept {
    return PrivateKey(parse_hex(hexPrvKey)).getPublicKey(PublicKeyType::nist256p1);
}

Data Signer::sign(const PrivateKey &privateKey, const Data &msg) noexcept {
    auto digest = Hash::sha256(msg);
    std::cout << "digest: " << hex(digest) << std::endl;
    auto signature = privateKey.sign(digest, TWCurveNIST256p1);
    return signature;
}

bool Signer::verify(const PublicKey &publicKey, const Data &signature, const Data &msg) noexcept {
    auto digest = Hash::sha256(msg);
    return publicKey.verify(signature, digest);
}
