// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <vector>

namespace TW {
namespace Ontology {
class SigData {

private:
    uint8_t m;
    std::vector<std::vector<uint8_t>> pubKeys;
    std::vector<std::vector<uint8_t>> sigs;

public:

    explicit SigData(const std::vector<uint8_t> &pubKey, const std::vector<uint8_t> &sig, uint8_t m) : m(m) {
        sigs.push_back(sig);
        pubKeys.push_back(pubKey);
    }

    std::vector<uint8_t> serialize();

};

}}
