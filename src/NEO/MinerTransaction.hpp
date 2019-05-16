// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../ReadData.h"

namespace TW::NEO {
    class MinerTransaction : public Transaction {
    public:
        uint32_t nonce;

        virtual int deserializeExclusiveData(const Data &data, int initial_pos = 0) {
            nonce = read<uint32_t>(data, initial_pos);
            return initial_pos + 4;
        }

        virtual Data serializeExclusiveData() const {
            return write<uint32_t>(nonce);
        }

        bool operator==(const MinerTransaction &other) const {
            return this->nonce == other.nonce && Transaction::operator==(other);
        }
    };

} // namespace TW::NEO
