// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Ethereum;

std::tuple<uint256_t, uint256_t, uint256_t> Signer::values(const uint256_t& chainID, const std::array<byte, 65>& signature) noexcept {
  // TODO: implement
}

std::tuple<uint256_t, uint256_t, uint256_t> Signer::sign(const uint256_t& chainID, const PrivateKey& privateKey, const Data& hash) noexcept {
  // TODO: implement
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
  // TODO: implement
}

Data Signer::hash(const Transaction& transaction) const noexcept {
  // TODO: implement
}
