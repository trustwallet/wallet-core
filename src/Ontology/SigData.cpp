// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <stdexcept>

#include "ParamsBuilder.h"
#include "SigData.h"

namespace TW::Ontology {

Data SigData::serialize() {
    auto sigInfo = ParamsBuilder::fromSigs(sigs);
    if (pubKeys.empty()) {
        throw std::runtime_error("Public key is empty.");
    }
    std::vector<uint8_t> verifyInfo;
    if (pubKeys.size() == 1) {
        verifyInfo = ParamsBuilder::fromPubkey(pubKeys[0]);
    } else {
        verifyInfo = ParamsBuilder::fromMultiPubkey(m, pubKeys);
    }
    ParamsBuilder builder;
    builder.pushVar(sigInfo);
    builder.pushVar(verifyInfo);
    return builder.getBytes();
}

} // namespace TW::Ontology
