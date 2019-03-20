// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <stdexcept>

#include "SigData.h"
#include "ParamsBuilder.h"

#include "HexCoding.h"

using namespace TW::Ontology;

std::vector<uint8_t> SigData::serialize() {
    ParamsBuilder builder;
    for (auto const &sig : sigs) {
        builder.push(sig);
    }
    auto sigInfo = builder.getBytes();
    builder.cleanUp();
    if (pubKeys.empty()) {
        throw std::runtime_error("Public key is empty.");
    }
    std::vector<uint8_t> verifyInfo;
    if (pubKeys.size() == 1) {
        builder.push(pubKeys[0]);
        builder.pushBack((uint8_t) 0xAC);
        verifyInfo = builder.getBytes();
        builder.cleanUp();
    } else {
        throw std::runtime_error("multi-signature is unsupported.");
    }
    builder.pushVar(sigInfo);
    builder.pushVar(verifyInfo);
    return builder.getBytes();
}