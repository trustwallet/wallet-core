// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../uint256.h"
#include <tuple>

namespace TW::Harmony {

class SignerUtils {
  public:
    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t> values(const uint256_t &chainID,
                                                              const Data &signature) noexcept {
        auto r = load(Data(signature.begin(), signature.begin() + 32));
        auto s = load(Data(signature.begin() + 32, signature.begin() + 64));
        auto v = load(Data(signature.begin() + 64, signature.begin() + 65));
        v += 35 + chainID + chainID;
        return std::make_tuple(r, s, v);
    }

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t>
    sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash) noexcept {
        auto signature = privateKey.sign(hash, TWCurveSECP256k1);
        return values(chainID, signature);
    }
};

} // namespace TW::Harmony