// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"

#include "Ethereum/RLP.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace TW::Aion {

using boost::multiprecision::uint128_t;

/// Aion's RLP encoding for long numbers
/// https://github.com/aionnetwork/aion/issues/680
struct RLP {
    static EthereumRlp::Proto::RlpItem prepareLong(uint128_t l) {
        EthereumRlp::Proto::RlpItem item;

        if ((l & 0x00000000FFFFFFFFL) == l) {
            auto u256 = store(l);
            item.set_number_u256(u256.data(), u256.size());
        } else {
            Data result(9);
            result[0] = 0x80 + 8;
            for (int i = 8; i > 0; i--) {
                result[i] = (byte)(l & 0xFF);
                l >>= 8;
            }
            item.set_raw_encoded(result.data(), result.size());
        }

        return item;
    }
};

} // namespace TW::Aion
