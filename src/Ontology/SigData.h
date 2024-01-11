// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::Ontology {

class SigData {
  private:
    uint8_t m;

    std::vector<Data> pubKeys;

    std::vector<Data> sigs;

  public:
    explicit SigData(const Data& pubKey, const Data& sig, uint8_t m) : m(m) {
        sigs.push_back(sig);
        pubKeys.push_back(pubKey);
    }

    std::vector<uint8_t> serialize();
};

} // namespace TW::Ontology
