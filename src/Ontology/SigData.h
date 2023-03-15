// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
